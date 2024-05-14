#ifndef _ray_h_
#define _ray_h_

#include "vec.h"

typedef struct ray {
    point3 origin;
    vec3 direction;
} ray;

void init_ray(ray *r, point3 *origin, vec3 *direction);

#endif

