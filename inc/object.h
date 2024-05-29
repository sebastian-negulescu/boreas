#ifndef _object_h_
#define _object_h_

typedef enum object_type {
    SPHERE,
} object_type;

typedef struct object {
    object_type type;
    void *object_ptr;

    struct object *next_object;
} object;

#endif

