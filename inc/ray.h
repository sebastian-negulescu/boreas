#ifndef _ray_h_
#define _ray_h_

#include "vec.h"

typedef struct __attribute__ ((packed)) ray {
    point3 o;
    vec3 d;
} ray;

void init_ray(ray *r, point3 *o, vec3 *d);

#endif

