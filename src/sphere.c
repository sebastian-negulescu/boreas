#include <math.h>
#include "sphere.h"
#include "vec.h"
#include "util.h"

intersection intersect(sphere *s, ray *r) {
    // define intersection to return
    intersection i = {false, 0.f};

    vec3 diff = s->position;
    sub_vec(&diff, &r->origin);

    float a = dot_vec(&r->direction, &r->direction);
    float b = -2.f * dot_vec(&r->direction, &diff);
    float c = -(s->radius * s->radius) + dot_vec(&diff, &diff);

    if (is_within(a, 0.f, ERROR)) {
        // TODO: or is a really, really small value
        return i;
    }

    float discriminant = b * b - 4.f * a * c;
    if (is_within(discriminant, 0.f, ERROR)) {
        discriminant = 0.f;
    }
    if (discriminant < 0.f) {
        return i;
    }

    float discriminant_sqrt = sqrtf(discriminant);

    float t1 = (-b + discriminant_sqrt) / (2 * a);
    float t2 = (-b - discriminant_sqrt) / (2 * a);

    i.t = t1;

    // check which one is smaller
    if (t2 < t1 && t2 >= 0.f) {
        i.t = t2;
    }

    if (i.t < 0.f) {
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

