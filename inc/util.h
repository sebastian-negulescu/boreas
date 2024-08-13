#ifndef _util_h_
#define _util_h_

#include <CL/cl.h>

extern const cl_float ERROR;

cl_bool is_within(cl_float x, cl_float check, cl_float margin);

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#endif

