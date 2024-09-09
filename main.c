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
#include "pane.h"
#include "cl_helpers.h"

const size_t MAX_BOUNCES = 1;

// TODO: create a scene parser
scene generate_scene(void) {
    scene s;
    init_scene(&s, 2);

    // create emissive sphere
    emissive e_1;
    init_vec(&e_1.c, 1.f, 1.f, 1.f);

    material m_1;
    init_material_emissive(&m_1, &e_1);

    sphere sphere_1;
    point3 sphere_origin;
    init_vec(&sphere_origin, 0.f, 0.f, 1.f);
    init_sphere(&sphere_1, &sphere_origin, 0.5f);

    object o_1;
    init_object_sphere(&o_1, &sphere_1, &m_1);
    add_object(&s, &o_1);

    // create diffuse sphere
    diffuse d_1;
    init_vec(&d_1.albedo, .5f, .5f, .5f);

    material m_2;
    init_material_diffuse(&m_2, &d_1);

    sphere sphere_2;
    init_vec(&sphere_origin, 0.f, -101.f, 1.f); 
    init_sphere(&sphere_2, &sphere_origin, 100.f);
   
    object o_2;
    init_object_sphere(&o_2, &sphere_2, &m_2);
    add_object(&s, &o_2);
    
    return s;
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
    const size_t NUM_RAYS = img.width * img.height * img.samples;

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
        NUM_RAYS * sizeof(ray), 
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
        NUM_RAYS * sizeof(intersection)
    );
    cl_mem intersections_buffer = clCreateBuffer(
        context, 
        CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE,
        NUM_RAYS * sizeof(intersection), 
        (void *)intersections, 
        &cl_err
    );

    colour *ray_colours = malloc(NUM_RAYS * sizeof(colour));
    for (size_t i = 0; i < NUM_RAYS; ++i) {
        init_vec(&ray_colours[i], 1.f, 1.f, 1.f);
    }
    cl_mem colours_buffer = clCreateBuffer(
        context,
        CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE,
        NUM_RAYS * sizeof(colour),
        (void *)ray_colours,
        &cl_err
    );

    cl_mem material_queue_buffer = clCreateBuffer(
        context,
        CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY,
        NUM_RAYS * sizeof(size_t),
        NULL,
        &cl_err
    );

    if (cl_err != CL_SUCCESS) {
        printf("some bad, bad buffers\n");
        return 1;
    }

    size_t *emissive_queue = malloc(NUM_RAYS * sizeof(size_t));
    size_t *diffuse_queue = malloc(NUM_RAYS * sizeof(size_t));
    size_t *medium_queue = malloc(NUM_RAYS * sizeof(size_t));

    size_t emissive_queue_ind = 0;
    size_t diffuse_queue_ind = 0;
    size_t medium_queue_ind = 0;

    if (emissive_queue == NULL || diffuse_queue == NULL || medium_queue == NULL) {
        printf("queue allocation problem!\n");
        return 1;
    }

    cl_kernel intersect_scene_kernel;
    cl_err = compile_kernel(
        &intersect_scene_kernel,
        "intersect_scene",
        "src/kernels/intersect_scene.cl",
        &context,
        num_devices,
        devices
    );

    cl_kernel emissive_material_kernel;
    cl_err = compile_kernel(
        &emissive_material_kernel,
        "shade_emissive",
        "src/kernels/materials/emissive.cl",
        &context,
        num_devices,
        devices
    );

    for (size_t i = 0; i < MAX_BOUNCES; ++i) {
        // we just need to give the rays and objects to the kernel
        // don't need to include num rays
        cl_err = clSetKernelArg(intersect_scene_kernel, 0, sizeof(cl_mem), &camera_ray_buffer);
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

        // reset all shader queue indices
        emissive_queue_ind = 0;
        diffuse_queue_ind = 0;
        medium_queue_ind = 0;

        // shade ray based on what each one hit
        for (size_t j = 0; j < img.width * img.height * img.samples; ++j) {
            if (!intersections[j].hit) {
                mult_vec(&ray_colours[j], 0);
                continue;
            }

            size_t hit_object_ind = intersections[j].object_ptr;
            // add ray index based on material type
            switch (s.objects[hit_object_ind].m.type) {
                case EMISSIVE:
                    emissive_queue[emissive_queue_ind++] = j;
                    break;

                case DIFFUSE:
                    diffuse_queue[diffuse_queue_ind++] = j;
                    break;

                case MEDIUM:
                    medium_queue[medium_queue_ind++] = j;
                    break;

                default:
                    break;
            }
        }

        // call all shader kernels
        cl_err = clSetKernelArg(emissive_material_kernel, 0, sizeof(cl_mem), &camera_ray_buffer);

    }
 
    FILE *image_file = fopen(img.filename, "w");
    fprintf(image_file, "P3\n%lu %lu\n255\n", img.width, img.height);
    for (size_t i = 0; i < img.height; ++i) {
        for (size_t j = 0; j < img.width; ++j) {
            colour pixel;
            init_zero_vec(&pixel);
            for (size_t k = 0; k < img.samples; ++k) {
                add_vec(
                    &pixel, 
                    &ray_colours[i * img.width * img.samples + j * img.samples + k]
                );
            }
            mult_vec(&pixel, 1.f / ((float)img.samples));
            fprintf(image_file, "%d %d %d\n",
                (int)(255.999 * pixel.x),
                (int)(255.999 * pixel.y),
                (int)(255.999 * pixel.z)
            );
        }
    }
    fclose(image_file);

    // make sure you free all the memory, might need to do this before
    free(camera_rays);
    free(ray_colours);
    free(intersections);
    free(emissive_queue);
    free(diffuse_queue);
    free(medium_queue);
    destroy_scene(&s);

    printf("finished!\n");
    return 0;
}

