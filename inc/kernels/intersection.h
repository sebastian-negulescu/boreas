#ifndef _kernel_intersection_h_
#define _kernel_intersection_h_

#include "vec.h"

typedef struct __attribute__ ((packed)) intersection {
    bool hit;
    float t;
    vec3 normal;
    point3 point;
} intersection;

#endif
