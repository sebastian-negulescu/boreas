#include <stddef.h>
#include <stdlib.h>
#include "scene.h"

void init_scene(scene *s, size_t n) {
    s->objects = malloc(n * sizeof(object));
    s->max_objects = n;
    s->free_index = 0;
}

void destroy_scene(scene *s) {
    free(s->objects);
}

bool add_object(scene *s, object *o) {
    if (s->free_index >= s->max_objects) {
        return false;
    }

    // deep-copy object o into s->objects array
    // will this deep copy?
    s->objects[s->free_index++] = *o;

    return true;
}

