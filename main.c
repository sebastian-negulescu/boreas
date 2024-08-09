#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

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
    
    material m_2;
    init_material_diffuse(&m_2, &d_2);

    sphere_origin.z = 1.f;
    sphere_origin.y = -100.5f;
    sphere sph_2;
    init_sphere(&sph_2, &sphere_origin, 100.f);

    object o_2;
    init_object_sphere(&o_2, &sph_2, &m_2);
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

    image img = {200, 200, "image.ppm"};

    pane p;
    init_pane(&p, &c, img.width, img.height);
    
    scene s = generate_scene();

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

    // needs to be swapped
    cl_mem objects_buffer = clCreateBuffer(
            context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            s.num_objects * sizeof(object), (void *)s.objects, &cl_err);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    char *file_buffer = 0;
    long length;
    FILE *f = fopen("src/kernels/colour.cl", "rb");
    if (f) {
        // get file length
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        // read file into string
        file_buffer = malloc(length);
        if (file_buffer) {
            fread(file_buffer, 1, length, f);
        }
        fclose(f);
    } else {
        return 1;
    }

    const char *program_strings[1] = {file_buffer};
    size_t lengths[1] = {length};

    cl_program colour_path_program = clCreateProgramWithSource(
        context, 1, program_strings, lengths, &cl_err);

    cl_err = clBuildProgram(colour_path_program, 1, devices, "-I./inc/kernels", NULL, NULL);

    if (cl_err != CL_SUCCESS) {
        char error_msg[2048];
        clGetProgramBuildInfo(
            colour_path_program, 
            devices[num_devices - 1], 
            CL_PROGRAM_BUILD_LOG, 
            2048, 
            error_msg, 
            NULL
        ); 
        printf("%s\n", error_msg);
        return 1;
    }

    cl_kernel colour_path_kernel = clCreateKernel(
        colour_path_program, "colour_path", &cl_err); 

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    // to construct ray direction from camera given work index
    cl_err = clSetKernelArg(colour_path_kernel, 0, sizeof(vec3), &c.look.origin);
    cl_err = clSetKernelArg(colour_path_kernel, 1, sizeof(pane), &p);
    // to loop through objects
    cl_err = clSetKernelArg(colour_path_kernel, 2, sizeof(size_t), &s.num_objects);
    cl_err = clSetKernelArg(colour_path_kernel, 3, sizeof(cl_mem), &objects_buffer);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_event kernel_progress;
    const size_t global_work_size[] = {img.width, img.height};
    const size_t local_work_size[] = {1, 1};
    cl_err = clEnqueueNDRangeKernel(
        queue, colour_path_kernel, 2, NULL, global_work_size, local_work_size, 
        0, NULL, &kernel_progress);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_err = clWaitForEvents(1, &kernel_progress);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    printf("finished!\n");
    return 0;
}

