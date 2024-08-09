#ifndef _kernel_pane_h_
#define _kernel_pane_h_

#include "vec.h"

typedef struct pane {
    point3 top_left;
    vec3 x_mod_base;
    vec3 y_mod_base;
} pane;

#endif
