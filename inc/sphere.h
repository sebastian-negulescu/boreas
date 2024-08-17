#ifndef _sphere_h_
#define _sphere_h_

#include <stdbool.h>
#include <CL/cl.h>

#include "vec.h"

typedef struct __attribute__ ((packed)) sphere {
    point3 position;
    cl_float radius;
} sphere;

void init_sphere(sphere *s, point3 *position, cl_float radius);

typedef struct __attribute__ ((packed)) intersection {
    cl_uchar hit;
    cl_float t;
    vec3 normal;
    point3 point;
} intersection;

#endif

