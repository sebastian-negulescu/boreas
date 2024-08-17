#include <CL/cl.h>
#include "sphere.h"
#include "vec.h"

void init_sphere(sphere *s, point3 *position, cl_float radius) {
    s->position = *position;
    s->radius = radius;
}

