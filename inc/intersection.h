#ifndef _intersection_h_
#define _intersection_h_

#include <CL/cl.h>
#include "vec.h"

typedef struct __attribute__ ((packed)) intersection {
    cl_uchar hit;
    cl_float t;
    vec3 normal;
    point3 point;

    cl_uint object_ind;
} intersection;

#endif

