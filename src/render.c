#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "render.h"
#include "material.h"

static const unsigned int MAX_BOUNCES = 3;

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

colour get_sphere_colour(sphere *s, ray *r) {
    intersection i = intersect(s, r);
    colour c = {0.f, 0.f, 0.f};
    if (i.hit) {
        c.x = 1.f;
        c.y = 1.f;
        c.z = 1.f;
    }
    return c;
}

colour get_colour(scene *s, ray *r, unsigned int num_bounces) {
    // colour c = {.1f, .3f, .7f};
    colour c = {0.f, 0.f, 0.f};

    if (num_bounces >= MAX_BOUNCES) {
        return c;
    }

    object *closest_object = NULL;
    float closest_distance = INFINITY;
    intersection closest_intersection;

    object *current_object = NULL;
    for (size_t i = 0; i < s->num_objects; ++i) {
        current_object = &s->objects[i];
        switch (current_object->type) {
            case SPHERE: {
                sphere *sphere_obj = &current_object->s;
                intersection i = intersect(sphere_obj, r);

                if (i.hit) {
                    vec3 distance_vec = r->d;
                    mult_vec(&distance_vec, i.t);
                    float distance_to_origin = magnitude_vec(&distance_vec);

                    if (distance_to_origin < closest_distance && i.t >= 0) {
                        closest_object = current_object;
                        closest_distance = distance_to_origin;
                        closest_intersection = i;
                    }
                }

                break;
            }
            default: break;
        }
    }

    if (closest_object == NULL) {
        // going to be the background colour
        return c;
    }

    // need to shade the closest object
    c = shade_material(&closest_object->m, &closest_intersection, r);
    if ((closest_object->m.type != EMISSIVE) && (num_bounces < MAX_BOUNCES)) {
        colour other_c = get_colour(s, r, num_bounces + 1);
        c.x *= other_c.x;
        c.y *= other_c.y;
        c.z *= other_c.z;
    }
    
    assert(c.x <= 1.f && c.y <= 1.f && c.z <= 1.f);
    return c;
}

