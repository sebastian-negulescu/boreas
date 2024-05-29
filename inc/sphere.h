#ifndef _sphere_h_
#define _sphere_h_

#include "vec.h"
#include "ray.h"

typedef struct sphere {
    point3 position;
    float radius;
} sphere;

typedef struct intersection {
    bool hit;
    float t;
} intersection;

// TODO: return an intersection 
// or return bool and modify t 
// or return void and modify struct?
// i think there will only be one intersection function, so naming is ok
intersection intersect(sphere *s, ray *r);

#endif

