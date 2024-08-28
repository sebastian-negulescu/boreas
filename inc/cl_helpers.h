#ifndef _cl_helpers_h_
#define _cl_helpers_h_

#include <CL/cl.h>

char *read_file(const char *path);

cl_int compile_kernel(
    cl_kernel *kernel_ptr,
    const char *kernel_name, 
    const char *kernel_file_name,
    cl_context *context,
    cl_uint num_devices,
    cl_device_id *devices
);

#endif

