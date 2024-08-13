#ifndef _object_h_
#define _object_h_

#include "sphere.h"
#include "material.h"

typedef enum object_type {
    SPHERE,
} object_type;

typedef struct __attribute__ ((packed)) object {
    object_type type;
    union {
        sphere s;
        // other object types will go here
    };
    // void *object_ptr;

    material m;
    // struct object *next_object;
} object;

void init_object_sphere(object *o, sphere *s, material *m);

#endif

