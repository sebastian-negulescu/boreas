#include <stddef.h>
#include "object.h"

void init_object(object *o) {
    o->object_ptr = NULL;
    o->material = NULL;
    o->next_object = NULL;
}

