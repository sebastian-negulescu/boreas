#ifndef _sphere_h_
#define _sphere_h_

#include <stdbool.h>
#include <CL/cl.h>

#include "vec.h"
#include "ray.h"

typedef struct sphere {
    point3 position;
    cl_float radius;
} sphere;

void init_sphere(sphere *s, point3 *position, cl_float radius);

typedef struct intersection {
    bool hit;
    float t;
    vec3 normal;
    point3 point;
} intersection;

// TODO: return an intersection 
// or return bool and modify t 
// or return void and modify struct?
// i think there will only be one intersection function, so naming is ok
intersection intersect(sphere *s, ray *r);

#endif

