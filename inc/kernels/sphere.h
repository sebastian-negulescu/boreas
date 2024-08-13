#ifndef _kernel_sphere_h_
#define _kernel_sphere_h_

#include "vec.h"

typedef struct __attribute__ ((packed)) sphere {
    point3 position;
    float radius;
} sphere;

#endif
