#include <stddef.h>
#include "vec.h"

void init_vec(vec3 *v, const float x, const float y, const float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}
void init_scalar_vec(vec3 *v, const float s) {
    v->x = s;
    v->y = s;
    v->z = s;
}
void init_zero_vec(vec3 *v) {
    v->x = 0.f;
    v->y = 0.f;
    v->z = 0.f;
}

void add_vec(vec3 *v, const vec3 *w) {}
void sub_vec(vec3 *v, const vec3 *w) {}
void mult_vec(vec3 *v, const float s) {}
void div_vec(vec3 *v, const float s) {}

float dot_vec(const vec3 *v, const vec3 *w) { return 0.f; }
vec3 cross_vec(const vec3 *v, const vec3 *w) {
    vec3 ret = {0, 0, 0};
    return ret;
}

char *string_vec(const vec3 *v) { return (char *)NULL; }

