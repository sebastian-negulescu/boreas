// #include <stdio.h>
#include <stddef.h>
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
    size_t width = 600;
    size_t height = 400;
    float aspect_ratio = ((float) width)/((float) height);

    float h_fov = 45.f; // horizontal fov
    float v_fov = h_fov * aspect_ratio;

    return 0;
}

