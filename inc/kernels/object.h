#ifndef _kernel_object_h_
#define _kernel_object_h_

#include "sphere.h"
#include "material.h"

typedef enum object_type {
    SPHERE,
} object_type;

typedef struct object {
    object_type type;
    union {
        sphere s;
        // other object types will go here
    };
    // void *object_ptr;

    material m;
    // struct object *next_object;
} object;

#endif
