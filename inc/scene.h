#ifndef _scene_h_
#define _scene_h_

#include "object.h"

typedef struct scene {
    object *objects;
} scene;

void init_scene(scene *s);
void add_object(scene *s, object *o);

#endif

