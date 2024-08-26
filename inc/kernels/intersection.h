#ifndef _kernel_intersection_h_
#define _kernel_intersection_h_

#include "vec.h"

typedef struct __attribute__ ((packed)) intersection {
    unsigned char hit;
    float t;
    vec3 normal;
    point3 point;

    unsigned int object_ptr;
} intersection;

#endif
