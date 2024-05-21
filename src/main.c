#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include "vec.h"
#include "camera.h"

int main(int , char *[]) {
    // create a camera
    point3 look_from;
    point3 look_at;
    vec3 up;
    init_zero_vec(&look_from);
    init_vec(&look_at, 0.f, 0.f, 1.f);
    init_vec(&up, 0.f, 1.f, 0.f);

    camera c;
    init_camera(&c, &look_from, &look_at, &up);

    // define image properties
    size_t width = 2;
    size_t height = 2;
    float aspect_ratio = ((float)width) / ((float)height);

    float h_fov = 90.f * M_PI / 180.f; // horizontal fov in rad
    float v_fov = h_fov * aspect_ratio;

    // define pane to shoot rays through
    float pane_width_half = tan(h_fov / 2.f);
    float pane_width_segment_half = pane_width_half / (float)width;
    float pane_width_segment = pane_width_segment_half * 2.f;

    float pane_height_half = tan(v_fov / 2.f);
    float pane_height_segment_half = pane_height_half / (float)height;
    float pane_height_segment = pane_height_segment_half * 2.f;

    // set up top left of pane we are shooting through
    point3 top_left;

    vec3 pane_x = c.u;
    mult_vec(&pane_x, (pane_width_half - pane_width_segment_half));
    mult_vec(&pane_x, -1.f);

    vec3 pane_y = c.v;
    mult_vec(&pane_y, (pane_height_half - pane_height_segment_half)); 

    vec3 pane_z = c.w;
    mult_vec(&pane_z, -1.f);

    add_vec(&top_left, &pane_x);
    add_vec(&top_left, &pane_y);
    add_vec(&top_left, &pane_z);

    vec3 x_mod_base = c.u;
    mult_vec(&x_mod_base, pane_width_segment);

    vec3 y_mod_base = c.v;
    mult_vec(&y_mod_base, pane_height_segment);
    mult_vec(&y_mod_base, -1.f);

    // shoot rays
    point3 ray_origin = c.look.origin;
    vec3 ray_direction;

    for (size_t x = 0; x < width; ++x) {
        vec3 x_mod = x_mod_base;
        mult_vec(&x_mod, x); 
        
        for (size_t y = 0; y < height; ++y) {
            vec3 y_mod = y_mod_base;
            mult_vec(&y_mod, y);

            ray_direction = top_left;
            add_vec(&ray_direction, &x_mod);
            add_vec(&ray_direction, &y_mod);

            /*
            // for testing purposes
            const char *vec_str = string_vec(&ray_direction);
            printf("ray direction %s\n", vec_str);
            free((void *)vec_str);
            */

            normalize_vec(&ray_direction);
            
            ray r;
            init_ray(&r, &ray_origin, &ray_direction);
        }
    }

    return 0;
}

