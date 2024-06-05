#include "scene.h"

void add_object(scene *s, object *o) {
    o->next_object = s->objects;
    s->objects = o;
}

