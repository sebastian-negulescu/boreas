#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

/*
#include "vec.h"
#include "camera.h"
#include "image.h"
#include "scene.h"
#include "render.h"
*/

int main(void) {
    const size_t ROW_SIZE = 3;
    const size_t COL_SIZE = 3;
    const size_t MATRIX_SIZE = ROW_SIZE * COL_SIZE;
    // these are transposed, one column is an array
    const float A[3][3] = {{1.f, 2.f, 3.f},
                           {4.f, 5.f, 6.f},
                           {7.f, 8.f, 9.f}};
    const float B[3][3] = {{1.f, 2.f, 3.f},
                           {4.f, 5.f, 6.f},
                           {7.f, 8.f, 9.f}};

    cl_int cl_err = CL_SUCCESS;

    cl_uint num_platforms = 0;
    cl_platform_id platform;

    cl_err = clGetPlatformIDs( 1, &platform, &num_platforms );

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

    cl_mem mat_a = clCreateBuffer(
            context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            MATRIX_SIZE * sizeof(float), (void *)A, &cl_err);
    cl_mem mat_b = clCreateBuffer(
            context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            MATRIX_SIZE * sizeof(float), (void *)B, &cl_err);

    cl_mem mat_c = clCreateBuffer(
            context, CL_MEM_WRITE_ONLY, MATRIX_SIZE * sizeof(float), NULL, &cl_err);

    char *buffer = 0;
    long length;
    FILE *f = fopen ("src/kernels/mat_mul.cl", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    } else {
        return 1;
    }


    const char *program_strings[1] = {buffer};
    size_t lengths[1] = {length};

    cl_program mat_mul_program = clCreateProgramWithSource(
        context, 1, program_strings, lengths, &cl_err);

    cl_err = clBuildProgram(mat_mul_program, 1, devices, NULL, NULL, NULL);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_kernel mat_mul_kernel = clCreateKernel(
        mat_mul_program, "mat_mul", &cl_err); 

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_err = clSetKernelArg(mat_mul_kernel, 0, sizeof(int), &COL_SIZE);
    cl_err = clSetKernelArg(mat_mul_kernel, 1, sizeof(cl_mem), &mat_a);
    cl_err = clSetKernelArg(mat_mul_kernel, 2, sizeof(cl_mem), &mat_b);
    cl_err = clSetKernelArg(mat_mul_kernel, 3, sizeof(cl_mem), &mat_c);

    if (cl_err != CL_SUCCESS) {
        return 1;
    }

    cl_event kernel_progress;
    const size_t global_work_size[] = {3, 3};
    const size_t local_work_size[] = {1, 1};
    cl_err = clEnqueueNDRangeKernel(
        queue, mat_mul_kernel, 2, NULL, global_work_size, local_work_size, 
        0, NULL, &kernel_progress);

    if (cl_err != CL_SUCCESS) {
        printf("kernel issues\n");
        return 1;
    }

    clWaitForEvents(1, &kernel_progress);

    float result[ROW_SIZE][COL_SIZE];
    cl_err = clEnqueueReadBuffer(
        queue, mat_c, CL_TRUE, 0, sizeof(float) * MATRIX_SIZE, result, 0, NULL, NULL);

    for (size_t i = 0; i < ROW_SIZE; ++i) {
        for (size_t j = 0; j < COL_SIZE; ++j) {
            printf("%f ", result[j][i]);
        }
        printf("\n");
    }

    printf("we got there!\n");

    return 0;
}

/*
int main(void) {
    // create a camera
    point3 look_from = {0.f, 0.f, 0.f};
    point3 look_at = {0.f, 0.f, 1.f};
    vec3 up = {0.f, 1.f, 0.f};

    camera c;
    init_camera(&c, &look_from, &look_at, &up);

    // define image properties
    image img = {200, 200, "image.ppm"};

    scene s;
    init_scene(&s);

    // for testing purposes
    // defining scene, eventually will move to allow for heap allocation too
    emissive e_1;
    init_vec(&e_1.c, 1.f, 1.f, 1.f);

    diffuse d_2;
    init_vec(&d_2.albedo, .5f, .5f, .5f);

    material m_1;
    // init_material(&m_1, EMISSIVE, &e_1);
    init_material(&m_1, EMISSIVE, &e_1);

    sphere sph_1;
    point3 sphere_origin = {0.f, 0.f, 1.f};
    init_sphere(&sph_1, &sphere_origin, 0.5f);

    object o_1;
    init_object(&o_1, SPHERE, &sph_1, &m_1);
    add_object(&s, &o_1);
    
    material m_2;
    init_material(&m_2, DIFFUSE, &d_2);

    sphere_origin.z = 1.f;
    sphere_origin.y = -100.5f;
    sphere sph_2;
    init_sphere(&sph_2, &sphere_origin, 100.f);

    object o_2;
    init_object(&o_2, SPHERE, &sph_2, &m_2);
    add_object(&s, &o_2);

    render(&c, &s, &img);
    
    return 0;
}
*/

