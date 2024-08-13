#ifndef _vec_h_
#define _vec_h_

#include <CL/cl.h>

// getters and setters are implicit
typedef struct __attribute__ ((packed)) vec3 {
    cl_float x; // r
    cl_float y; // g
    cl_float z; // b
} vec3;

typedef vec3 point3;
typedef vec3 colour; // don't want to redefine the same stuff just for colour

void init_vec(vec3 *v, const cl_float x, const cl_float y, const cl_float z);
void init_scalar_vec(vec3 *v, const cl_float s);
void init_zero_vec(vec3 *v);
void copy_vec(vec3 *v, vec3 *w);

void add_vec(vec3 *v, const vec3 *w);
void sub_vec(vec3 *v, const vec3 *w);
void mult_vec(vec3 *v, const float s);
void div_vec(vec3 *v, const float s);
void normalize_vec(vec3 *v);

cl_float dot_vec(const vec3 *v, const vec3 *w);
cl_float magnitude_vec(const vec3 *v);
vec3 cross_vec(const vec3 *v, const vec3 *w);

const char *string_vec(const vec3 *v);

#endif
