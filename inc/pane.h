#ifndef _render_h_
#define _render_h_

#include <stddef.h>
#include "camera.h"
#include "image.h"

typedef struct __attribute__ ((packed)) pane {
    point3 top_left;
    vec3 x_mod_base;
    vec3 y_mod_base;
} pane;

void init_pane(pane *p, camera *c, size_t width, size_t height);
ray *generate_camera_rays(camera *c, image *img, pane *p);

#endif

