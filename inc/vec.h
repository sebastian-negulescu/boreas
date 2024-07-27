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

// allocation functions
vec3 *create_vec();
void destroy_vec(vec3 *v);

void init_vec(vec3 *v, const float x, const float y, const float z);
void init_scalar_vec(vec3 *v, const float s);
void init_zero_vec(vec3 *v);

// perform operations on v
void add_vec(vec3 *v, const vec3 *w);
void sub_vec(vec3 *v, const vec3 *w);
void mult_vec(vec3 *v, const float s);
void div_vec(vec3 *v, const float s);
void normalize_vec(vec3 *v);

// return result of operations
float dot_vec(const vec3 *v, const vec3 *w);
float magnitude_vec(const vec3 *v);
// should this modify a separate vector, u?
vec3 *cross_vec(const vec3 *v, const vec3 *w);

const char *create_vec_str(const vec3 *v);
void destroy_vec_str(const char *vec_str);

#endif
