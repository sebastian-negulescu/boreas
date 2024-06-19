#ifndef _object_h_
#define _object_h_

#include "material.h"

typedef enum object_type {
    SPHERE,
} object_type;

typedef struct object {
    object_type type;
    void *object_ptr;

    material *material;

    struct object *next_object;
} object;

void init_object(object *o);

#endif

