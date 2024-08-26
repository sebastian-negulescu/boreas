#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "pane.h"

void init_pane(pane *p, camera *c, size_t width, size_t height) {
    float aspect_ratio = ((float)width) / ((float)height);

    float h_fov = c->fov * M_PI / 180.f; // horizontal fov in rad
    float v_fov = h_fov / aspect_ratio;

    // define pane to shoot rays through
    float pane_width_half = tanf(h_fov / 2.f);
    float pane_width_segment_half = pane_width_half / (float)width;
    float pane_width_segment = pane_width_segment_half * 2.f;

    float pane_height_half = tanf(v_fov / 2.f);
    float pane_height_segment_half = pane_height_half / (float)height;
    float pane_height_segment = pane_height_segment_half * 2.f;

    // set up top left of pane we are shooting through
    point3 top_left;
    init_zero_vec(&top_left);

    vec3 pane_x = c->u;
    mult_vec(&pane_x, pane_width_half);
    mult_vec(&pane_x, -1.f);

    vec3 pane_y = c->v;
    mult_vec(&pane_y, pane_height_half); 

    vec3 pane_z = c->w;

    add_vec(&top_left, &pane_x);
    add_vec(&top_left, &pane_y);
    add_vec(&top_left, &pane_z);

    vec3 x_mod_base = c->u;
    mult_vec(&x_mod_base, pane_width_segment);

    vec3 y_mod_base = c->v;
    mult_vec(&y_mod_base, pane_height_segment);
    mult_vec(&y_mod_base, -1.f);

    p->top_left = top_left;
    p->x_mod_base = x_mod_base;
    p->y_mod_base = y_mod_base;
}

ray *generate_camera_rays(camera *c, image *img, pane *p) {
    size_t width = img->width;
    size_t height = img->height;

    ray *camera_rays = malloc(width * height * img->samples * sizeof(ray));
    if (camera_rays != NULL) {
        // shoot rays
        point3 ray_origin;
        copy_vec(&ray_origin, &c->look.o);
        vec3 ray_direction;

        for (size_t y = 0; y < height; ++y) {
            vec3 y_mod;
            copy_vec(&y_mod, &p->y_mod_base);
            mult_vec(&y_mod, y);

            for (size_t x = 0; x < width; ++x) {
                vec3 x_mod;
                copy_vec(&x_mod, &p->x_mod_base);
                mult_vec(&x_mod, x); 

                copy_vec(&ray_direction, &p->top_left);
                add_vec(&ray_direction, &x_mod);
                add_vec(&ray_direction, &y_mod);

                // RAY DIRECTION IS UNNORMALIZED FOR NOW
                ray r;
                init_ray(&r, &ray_origin, &ray_direction);

                for (size_t sample = 0; sample < img->samples; ++sample) {
                    ray sample_ray = r;

                    // jitter ray direction a bit per sample
                    vec3 x_jitter = p->x_mod_base; 
                    vec3 y_jitter = p->y_mod_base;
                    mult_vec(&x_jitter, .5f); // will be replaced with random
                    mult_vec(&y_jitter, .5f); // will be replaced with random
                    add_vec(&sample_ray.d, &x_jitter);
                    add_vec(&sample_ray.d, &y_jitter);

                    // HERE WE NORMALIZE THE DIRECTION VECTOR
                    normalize_vec(&sample_ray.d);

                    init_ray(
                        &camera_rays[
                            y * width * img->samples + 
                            x * img->samples + 
                            sample
                        ], 
                        &sample_ray.o, &sample_ray.d
                    );
                }
            }
        }
    }

    return camera_rays;
}

