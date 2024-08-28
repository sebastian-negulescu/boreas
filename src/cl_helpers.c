#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cl_helpers.h"

char *read_file(const char *path) {
    char *file_buffer = NULL;
    size_t length;
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

cl_int compile_kernel(
    cl_kernel *kernel_ptr, 
    const char *kernel_name, 
    const char *kernel_file_name,
    cl_context *context,
    cl_uint num_devices,
    cl_device_id *devices
) {
    cl_int cl_err;

    const char *kernel_file_buffer = read_file(kernel_file_name);
    if (kernel_file_buffer == NULL) {
        printf("could not read kernel file\n");
        cl_err = CL_INVALID_VALUE;
        return cl_err;
    }
    const char *kernel_strings[1] = {kernel_file_buffer};
    size_t lengths[1] = {strlen(kernel_file_buffer) + 1};

    cl_program kernel_program = clCreateProgramWithSource(
        *context,
        1,
        kernel_strings,
        lengths,
        &cl_err
    );

    free((void *)kernel_file_buffer);

    cl_err = clBuildProgram(kernel_program, 1, devices, "-I./inc", NULL, NULL);

    if (cl_err != CL_SUCCESS) {
        char error_msg[4096];
        clGetProgramBuildInfo(
            kernel_program, 
            devices[num_devices - 1], 
            CL_PROGRAM_BUILD_LOG, 
            4096, 
            error_msg, 
            NULL
        ); 
        printf("%s\n", error_msg);
        return cl_err;
    }

    cl_kernel kernel = clCreateKernel(kernel_program, kernel_name, &cl_err);
    if (cl_err != CL_SUCCESS) {
        printf("couldn't make the kernel\n");
        return cl_err;
    }

    memcpy(kernel_ptr, &kernel, sizeof(cl_kernel));

    return cl_err;
}

