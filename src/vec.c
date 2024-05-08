#include <assert.h>
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

void add_vec(vec3 *v, const vec3 *w) {
    v->x += w->x;
    v->y += w->y;
    v->z += w->z;
}
void sub_vec(vec3 *v, const vec3 *w) {
    v->x -= w->x;
    v->y -= w->y;
    v->z -= w->z;
}
void mult_vec(vec3 *v, const float s) {
    v->x *= s;
    v->y *= s;
    v->z *= s;
}
void div_vec(vec3 *v, const float s) {
    // eventually expand to check if it is within a threshold
    assert(s != 0.f);
    v->x /= s;
    v->y /= s;
    v->z /= s;
}

float dot_vec(const vec3 *v, const vec3 *w) {
    return v->x * w->x 
        + v->y * w->y 
        + v->z * w->z;
}
vec3 cross_vec(const vec3 *v, const vec3 *w) {
    vec3 r;
    float x = v->y * w->z - v->z * w->y;
    float y = v->z * w->x - v->x * w->z;
    float z = v->x * w->y - v->y * w->x;
    init_vec(&r, x, y, z);
    return r;
}

const char *string_vec(const vec3 *) {
    return (char *)NULL;
}

