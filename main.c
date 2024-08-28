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
    colour *ray_colours = malloc(img.width * img.height * img.samples * sizeof(colour));
    for (size_t i = 0; i < img.width * img.height * img.samples; ++i) {
        init_vec(&ray_colours[i], 1.f, 1.f, 1.f);
    }
    cl_mem colours_buffer = clCreateBuffer(
        context,
        CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE,
        img.width * img.height * img.samples * sizeof(colour),
        (void *)ray_colours,
        &cl_err
    );

    if (cl_err != CL_SUCCESS) {
        printf("some bad, bad buffers\n");
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

        // shade ray based on what each one hit
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
    destroy_scene(&s);

    printf("finished!\n");
    return 0;
}

