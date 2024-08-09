#ifndef _render_h_
#define _render_h_

#include <stddef.h>
#include "camera.h"
#include "image.h"
#include "scene.h"
#include "sphere.h"

typedef struct pane {
    point3 top_left;
    vec3 x_mod_base;
    vec3 y_mod_base;
} pane;

void init_pane(pane *p, camera *c, size_t width, size_t height);

void render(camera *c, scene *s, image *img); 

colour get_sphere_colour(sphere *s, ray *r);
colour get_colour(scene *s, ray *r, unsigned int num_bounces);

#endif

