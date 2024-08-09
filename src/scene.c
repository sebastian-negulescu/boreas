#include <stddef.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "scene.h"

void init_scene(scene *s, size_t n) {
    s->objects = malloc(n * sizeof(object));
    s->max_objects = n;
    s->num_objects = 0;
}

void destroy_scene(scene *s) {
    free(s->objects);
}

bool add_object(scene *s, object *o) {
    if (s->num_objects >= s->max_objects) {
        return false;
    }

    // deep-copy object o into s->objects array
    // will this deep copy?
    s->objects[s->num_objects++] = *o;

    return true;
}

