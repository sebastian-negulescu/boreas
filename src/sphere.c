#include <math.h>
#include <CL/cl.h>
#include "sphere.h"
#include "vec.h"
#include "util.h"

void init_sphere(sphere *s, point3 *position, cl_float radius) {
    s->position = *position;
    s->radius = radius;
}

intersection intersect(sphere *s, ray *r) {
    // define intersection to return
    intersection i = {false, 0.f, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}};

    vec3 diff = s->position;
    sub_vec(&diff, &r->origin);

    cl_float a = dot_vec(&r->direction, &r->direction);
    cl_float b = -2.f * dot_vec(&r->direction, &diff);
    cl_float c = -(s->radius * s->radius) + dot_vec(&diff, &diff);

    if (is_within(a, 0.f, ERROR)) {
        return i;
    }

    cl_float discriminant = b * b - 4.f * a * c;
    if (is_within(discriminant, 0.f, ERROR)) {
        discriminant = 0.f;
    }
    if (discriminant < 0.f) {
        return i;
    }

    cl_float discriminant_sqrt = sqrtf(discriminant);

    cl_float t1 = (-b + discriminant_sqrt) / (2 * a);
    cl_float t2 = (-b - discriminant_sqrt) / (2 * a);

    i.t = t1;

    // check which one is smaller
    if (t2 < t1 && t2 >= 0.f) {
        i.t = t2;
    }

    if (i.t < ERROR) {
        // intersection is behind us
        return i;
    }

    i.hit = true;

    point3 p = r->direction;
    mult_vec(&p, i.t);
    add_vec(&p, &r->origin);

    vec3 normal = p;
    sub_vec(&normal, &s->position);
    normalize_vec(&normal);

    i.normal = normal;
    i.point = p;

    return i;
}

