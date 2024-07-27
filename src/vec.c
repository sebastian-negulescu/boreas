#include <assert.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "vec.h"

vec3 *create_vec() {
    vec3 *v = malloc(sizeof(vec3));
    return v;
}
void destroy_vec(vec3 *v) {
    if (v != NULL) {
        free(v);
    }
}

void init_vec(vec3 *v, const float x, const float y, const float z) {
    assert(v != NULL);
    v->x = x;
    v->y = y;
    v->z = z;
}
void init_scalar_vec(vec3 *v, const float s) {
    assert(v != NULL);
    v->x = s;
    v->y = s;
    v->z = s;
}
void init_zero_vec(vec3 *v) {
    assert(v != NULL);
    v->x = 0.f;
    v->y = 0.f;
    v->z = 0.f;
}

void add_vec(vec3 *v, const vec3 *w) {
    assert((v != NULL) && (w != NULL));
    v->x += w->x;
    v->y += w->y;
    v->z += w->z;
}
void sub_vec(vec3 *v, const vec3 *w) {
    assert((v != NULL) && (w != NULL));
    v->x -= w->x;
    v->y -= w->y;
    v->z -= w->z;
}
void mult_vec(vec3 *v, const float s) {
    assert(v != NULL);
    v->x *= s;
    v->y *= s;
    v->z *= s;
}
void div_vec(vec3 *v, const float s) {
    assert(v != NULL);
    assert(!is_within(s, 0.f, ERROR));
    v->x /= s;
    v->y /= s;
    v->z /= s;
}
void normalize_vec(vec3 *v) {
    assert(v != NULL);
    float magnitude = magnitude_vec(v);
    assert(magnitude != 0.f);

    div_vec(v, magnitude);
}

float dot_vec(const vec3 *v, const vec3 *w) {
    assert((v != NULL) && (w != NULL));
    return v->x * w->x 
        + v->y * w->y 
        + v->z * w->z;
}
float magnitude_vec(const vec3 *v) {
    assert(v != NULL);
    return sqrtf(dot_vec(v, v));
}
vec3 *cross_vec(const vec3 *v, const vec3 *w) {
    assert((v != NULL) && (w != NULL));
    vec3 *r = create_vec();
    float x = v->y * w->z - v->z * w->y;
    float y = v->z * w->x - v->x * w->z;
    float z = v->x * w->y - v->y * w->x;
    init_vec(r, x, y, z);
    return r;
}

const char *string_vec(const vec3 *v) {
    // [ x, y, z ] (10 digits each) -> 8 + 3 * 21 + 1 (null) = 72
    size_t str_len = 72;
    char *vec_str = malloc(str_len * sizeof(char));

    snprintf(vec_str, str_len, "[ %10.10f, %10.10f, %10.10f ]", v->x, v->y, v->z);

    return vec_str;
}
void destroy_vec_str(const char *vec_str) {
    if (vec_str != NULL) {
        free((void *)vec_str);
    }
}

