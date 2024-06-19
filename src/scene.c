#include <stddef.h>
#include "scene.h"

void init_scene(scene *s) {
    s->objects = NULL;
}

void add_object(scene *s, object *o) {
    o->next_object = s->objects;
    s->objects = o;
}

