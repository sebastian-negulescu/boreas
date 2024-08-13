#include "ray.h"

void init_ray(ray *r, point3 *o, vec3 *d) {
    copy_vec(&r->o, o);
    copy_vec(&r->d, d);
}

