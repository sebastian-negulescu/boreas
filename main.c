#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "string.h"
#include "scene.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "camera.h"
#include "image.h"
#include "render.h"

// TODO: create a scene parser
scene generate_scene(void) {
    scene s;
    init_scene(&s, 2);

    emissive e_1;
    init_vec(&e_1.c, 1.f, 1.f, 1.f);

    diffuse d_2;
    init_vec(&d_2.albedo, .5f, .5f, .5f);

    material m_1;
    init_material_emissive(&m_1, &e_1);

    sphere sph_1;
    point3 sphere_origin = {0.f, 0.f, 1.f};
    init_sphere(&sph_1, &sphere_origin, 0.5f);

    object o_1;
    init_object_sphere(&o_1, &sph_1, &m_1);
    add_object(&s, &o_1);
    
    return s;
}

ray *generate_camera_rays(camera *c, image *img, pane *p) {
    size_t width = img->width;
    size_t height = img->height;

    ray *camera_rays = malloc(width * height * img->samples * sizeof(ray));
    if (camera_rays != NULL) {
        // shoot rays
        point3 ray_origin;
        copy_vec(&ray_origin, &c->look.o);
        vec3 ray_direction;

        for (size_t y = 0; y < height; ++y) {
            vec3 y_mod;
            copy_vec(&y_mod, &p->y_mod_base);
            mult_vec(&y_mod, y);

            for (size_t x = 0; x < width; ++x) {
                vec3 x_mod;
                copy_vec(&x_mod, &p->x_mod_base);
                mult_vec(&x_mod, x); 

                copy_vec(&ray_direction, &p->top_left);
                add_vec(&ray_direction, &x_mod);
                add_vec(&ray_direction, &y_mod);

                // RAY DIRECTION IS UNNORMALIZED FOR NOW
                ray r;
                init_ray(&r, &ray_origin, &ray_direction);

                for (size_t sample = 0; sample < img->samples; ++sample) {
                    ray sample_ray = r;

                    // jitter ray direction a bit per sample
                    vec3 x_jitter = p->x_mod_base; 
                    vec3 y_jitter = p->y_mod_base;
                    mult_vec(&x_jitter, .5f); // will be replaced with random
                    mult_vec(&y_jitter, .5f); // will be replaced with random
                    add_vec(&sample_ray.d, &x_jitter);
                    add_vec(&sample_ray.d, &y_jitter);

                    // HERE WE NORMALIZE THE DIRECTION VECTOR
                    normalize_vec(&sample_ray.d);

                    init_ray(
                        &camera_rays[
                            y * width * img->samples + 
                            x * img->samples + 
                            sample
                        ], 
                        &sample_ray.o, &sample_ray.d
                    );
                }
            }
        }
    }

    return camera_rays;
}

char *read_file(const char *path) {
    char *file_buffer = NULL;
    long length;
    FILE *f = fopen(path, "rb");
    if (f) {
        // get file length
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        // read file into string
        file_buffer = malloc((length + 1) * sizeof(char));
        if (file_buffer) {
            fread(file_buffer, sizeof(char), length, f);
            file_buffer[length] = '\0';
        }
        fclose(f);
    } else {
        return NULL;
    }

    return file_buffer;
}

int main(void) {
    point3 look_from;
    point3 look_at;
    vec3 up;

    init_zero_vec(&look_from);
    init_vec(&look_at, 0.f, 0.f, 1.f);
    init_vec(&up, 0.f, 1.f, 0.f);

    camera c;
    init_camera(&c, &look_from, &look_at, &up, 90.f);

    image img = {1000, 1000, 1, "image.ppm"};

    pane p;
    init_pane(&p, &c, img.width, img.height);
    
    scene s = generate_scene();
    ray *camera_rays = generate_camera_rays(&c, &img, &p);
    if (camera_rays == NULL) {
        printf("issue with generating camera rays\n");
    }

    cl_int cl_err = CL_SUCCESS;

    cl_uint num_platforms = 0;
    cl_platform_id platform;

    cl_err = clGetPlatformIDs(1, &platform, &num_platforms);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_uint num_devices = 0;
    cl_device_id devices[1];

    cl_err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 
        1, devices, &num_devices);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, &cl_err);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_command_queue queue = clCreateCommandQueueWithProperties(
            context, devices[0], NULL, &cl_err);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_mem camera_ray_buffer = clCreateBuffer(
        context, 
        CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY,
        img.width * img.height * img.samples * sizeof(ray), 
        (void *)camera_rays, 
        &cl_err
    );
    cl_mem objects_buffer = clCreateBuffer(
        context, 
        CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, 
        s.num_objects * sizeof(object), 
        (void *)s.objects, 
        &cl_err
    );
    intersection *intersections = malloc(
        img.width * img.height * img.samples * sizeof(intersection)
    );
    cl_mem intersections_buffer = clCreateBuffer(
        context, 
        CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE,
        img.width * img.height * img.samples * sizeof(intersection), 
        (void *)intersections, 
        &cl_err
    );

    if (cl_err != CL_SUCCESS) {
        printf("some bad, bad buffers\n");
        return 1;
    }

    const char *program_file_buffer = read_file("src/kernels/intersect_scene.cl");
    const char *program_strings[1] = {program_file_buffer};
    // program_file_buffer is OPTIONALLY null-terminated
    size_t lengths[1] = {strlen(program_file_buffer) + 1};

    cl_program intersect_scene_program = clCreateProgramWithSource(
        context, 1, program_strings, lengths, &cl_err);

    cl_err = clBuildProgram(intersect_scene_program, 1, devices, "-I./inc", NULL, NULL);

    if (cl_err != CL_SUCCESS) {
        char error_msg[4096];
        clGetProgramBuildInfo(
            intersect_scene_program, 
            devices[num_devices - 1], 
            CL_PROGRAM_BUILD_LOG, 
            4096, 
            error_msg, 
            NULL
        ); 
        printf("%s\n", error_msg);
        return 1;
    }

    cl_kernel intersect_scene_kernel = clCreateKernel(
        intersect_scene_program, "intersect_scene", &cl_err); 

    if (cl_err != CL_SUCCESS) {
        printf("couldn't make the kernel\n");
        return 1;
    }

    // we just need to give the rays and objects to the kernel
    // don't need to include num rays
    cl_err = clSetKernelArg(intersect_scene_kernel, 0, sizeof(cl_mem), &camera_ray_buffer);
    // to loop through objects
    cl_err = clSetKernelArg(intersect_scene_kernel, 1, sizeof(cl_uint), &s.num_objects);
    cl_err = clSetKernelArg(intersect_scene_kernel, 2, sizeof(cl_mem), &objects_buffer);
    cl_err = clSetKernelArg(intersect_scene_kernel, 3, sizeof(cl_mem), &intersections_buffer);

    if (cl_err != CL_SUCCESS) {
        printf("problem setting args???\n");
        return 1;
    }

    cl_event kernel_progress;
    const size_t global_work_size[] = {img.width * img.height * img.samples};
    const size_t local_work_size[] = {1};
    cl_err = clEnqueueNDRangeKernel(
        queue, intersect_scene_kernel, 1, NULL, global_work_size, local_work_size, 
        0, NULL, &kernel_progress);

    if (cl_err != CL_SUCCESS) {
        printf("there were some issues... %d\n", cl_err);
        return 1;
    }

    cl_err = clWaitForEvents(1, &kernel_progress);

    if (cl_err != CL_SUCCESS) {
        printf("there were some issues...\n");
        return 1;
    }

    // read back intersections buffer into intersections
    cl_err = clEnqueueReadBuffer(queue, intersections_buffer, CL_TRUE, 0,
        img.width * img.height * img.samples * sizeof(intersection), intersections,
        0, NULL, NULL);

    FILE *image_file = fopen(img.filename, "w");
    fprintf(image_file, "P3\n%lu %lu\n255\n", img.width, img.height);
    for (size_t i = 0; i < img.height; ++i) {
        for (size_t j = 0; j < img.width; ++j) {
            if ((intersections[i * img.width * img.samples + j * img.samples].hit & 1)) {
                fprintf(image_file, "%d %d %d\n", 255, 255, 255);
            } else {
                fprintf(image_file, "%d %d %d\n", 0, 0, 0);
            }
        }
    }
    fclose(image_file);

    // make sure you free all the memory, might need to do this before
    free((void *)program_file_buffer);
    free(camera_rays);
    free(intersections);
    destroy_scene(&s);

    printf("finished!\n");
    return 0;
}

