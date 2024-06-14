#ifndef _render_h_
#define _render_h_

#include <stddef.h>
#include "camera.h"
#include "image.h"
#include "scene.h"
#include "sphere.h"

void render(camera *c, scene *s, image *img); 
colour get_sphere_colour(sphere *s, ray *r);
colour get_colour(scene *s, ray *r, unsigned int num_bounces);

#endif

