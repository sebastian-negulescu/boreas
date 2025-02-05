#ifndef _vec_h_
#define _vec_h_

// getters and setters are implicit
typedef struct vec3 {
    float x; // r
    float y; // g
    float z; // b
} vec3;

typedef vec3 point3;
typedef vec3 colour; // don't want to redefine the same stuff just for colour

void init_vec(vec3 *v, const float x, const float y, const float z);
void init_scalar_vec(vec3 *v, const float s);
void init_zero_vec(vec3 *v);

void add_vec(vec3 *v, const vec3 *w);
void sub_vec(vec3 *v, const vec3 *w);
void mult_vec(vec3 *v, const float s);
void div_vec(vec3 *v, const float s);
void normalize_vec(vec3 *v);

float dot_vec(const vec3 *v, const vec3 *w);
float magnitude_vec(const vec3 *v);
vec3 cross_vec(const vec3 *v, const vec3 *w);

const char *string_vec(const vec3 *v);

#endif
