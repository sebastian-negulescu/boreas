#include <stddef.h>
#include "object.h"

void init_object(object *o, object_type t, void *o_ptr, material *m) {
    o->type = t;
    o->object_ptr = o_ptr;
    o->material = m;
    o->next_object = NULL;
}

