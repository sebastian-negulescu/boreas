#ifndef _scene_h_
#define _scene_h_

#include <stddef.h>
#include <stdbool.h>
#include "object.h"

typedef struct scene {
    object *objects;
    size_t max_objects;
    cl_uint num_objects;
} scene;

void init_scene(scene *s, size_t n);
void destroy_scene(scene *s);

bool add_object(scene *s, object *o);

#endif

