#include "ray.h"

void init_ray(ray *r, point3 *origin, vec3 *direction) {
    r->origin = *origin;
    r->direction = *direction;
}

