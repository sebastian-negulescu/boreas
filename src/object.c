#include <stddef.h>
#include "object.h"

void init_object_sphere(object *o, sphere *s, material *m) {
    o->type = SPHERE;
    o->s = *s;
    o->m = *m;
}

