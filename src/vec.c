#include <assert.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
void normalize_vec(vec3 *v) {
    float magnitude_sqr = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
    assert(magnitude_sqr >= 0.f);

    float magnitude = sqrtf(magnitude_sqr);
    assert(magnitude != 0.f);

    div_vec(v, magnitude);
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

const char *string_vec(const vec3 *v) {
    // [ x, y, z ] (10 digits each) -> 8 + 3 * 21 + 1 (null) = 72
    size_t str_len = 72;
    char *vec_str = malloc(str_len * sizeof(char));

    snprintf(vec_str, str_len, "[ %10.10f, %10.10f, %10.10f ]", v->x, v->y, v->z);

    return vec_str;
}

