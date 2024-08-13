#ifndef _kernel_ray_h_
#define _kernel_ray_h_

#include "vec.h"

typedef struct __attribute__ ((packed)) ray {
    point3 o; // origin
    vec3 d; // direction
} ray;

#endif

