#ifndef _vec_h_
#define _vec_h_

// getters and setters are implicit
typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

void init_vec(vec3 *v, const float x, const float y, const float z);
void init_scalar_vec(vec3 *v, const float s);
void init_zero_vec(vec3 *v);

void add_vec(vec3 *v, const vec3 *w);
void sub_vec(vec3 *v, const vec3 *w);
void mult_vec(vec3 *v, const float s);
void div_vec(vec3 *v, const float s);

float dot_vec(const vec3 *v, const vec3 *w);
vec3 cross_vec(const vec3 *v, const vec3 *w);

char *string_vec(const vec3 *v);

#endif
