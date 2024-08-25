#ifndef _sphere_h_
#define _sphere_h_

#include <CL/cl.h>

#include "vec.h"

typedef struct __attribute__ ((packed)) sphere {
    point3 position;
    cl_float radius;
} sphere;

void init_sphere(sphere *s, point3 *position, cl_float radius);

#endif

