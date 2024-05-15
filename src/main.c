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
    size_t width = 3;
    size_t height = 2;
    float aspect_ratio = ((float) width)/((float) height);

    float h_fov = 45.f * M_PI / 180.f; // horizontal fov in rad
    float v_fov = h_fov * aspect_ratio;

    // shoot rays
    point3 ray_origin = c.look.origin;
    vec3 ray_direction;

    // printf("ray: %s\n", string_vec(&ray_origin));

    float z_coord = 1.f;
    for (size_t x = 0; x < width; ++x) {
        float x_coord = (2.f * ((float) x) / ((float) (width - 1)) - 1.f) 
            * (tanf(h_fov) / 2.f);
        for (size_t y = 0; y < height; ++y) {
            // start from bottom left, end at top right
            float y_coord = (2.f * ((float) y) / ((float) (height - 1)) - 1.f)
                * (tanf(v_fov) / 2.f);
            
            init_vec(&ray_direction, x_coord, y_coord, z_coord);

            /*
            for testing purposes
            const char *vec_str = string_vec(&ray_direction);
            printf("ray direction %s\n", vec_str);
            free((void *)vec_str);
            */

            normalize_vec(&ray_direction);
            // TODO: transform to camera space
        }
    }

    return 0;
}

