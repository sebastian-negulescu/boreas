#ifndef _kernel_vec_h_
#define _kernel_vec_h_

typedef struct __attribute__ ((packed)) vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef vec3 point3;
typedef vec3 colour;

float3 get_vec(vec3 v) {
    return (float3)(v.x, v.y, v.z);
}

void set_private_vec(vec3 *v, float3 w) {
    v->x = w.x;
    v->y = w.y;
    v->z = w.z;
}

void set_global_vec(__global vec3 *v, float3 w) {
    v->x = w.x;
    v->y = w.y;
    v->z = w.z;
}

void copy_vec(vec3 *v, vec3 *w) {
    v->x = w->x;
    v->y = w->y;
    v->z = w->z;
}

void copy_global_vec(__global vec3 *v, vec3 *w) {
    v->x = w->x;
    v->y = w->y;
    v->z = w->z;
}

#endif

