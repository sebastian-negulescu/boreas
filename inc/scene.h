#ifndef _scene_h_
#define _scene_h_

#include <stddef.h>
#include "object.h"

typedef struct scene {
    object *objects;
    size_t max_objects;
    size_t free_index;
} scene;

void init_scene(scene *s, size_t n);
void destroy_scene(scene *s);

bool add_object(scene *s, object *o);

#endif

