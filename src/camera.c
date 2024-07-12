// #include <stdio.h>
// #include <stdlib.h>
#include "camera.h"
#include "util.h"

int init_camera(camera *c, point3 *look_from, point3 *look_at, vec3 *up) {
    // check if up vector is 0.
    if (is_within(magnitude_vec(up), 0.f, ERROR)) {
        return -1;
    }

    // check if look from is the same as look at
    vec3 look_diff = *look_from;
    sub_vec(&look_diff, look_at);
    if (is_within(magnitude_vec(&look_diff), 0.f, ERROR)) {
        return -1;
    }

    c->up = *up;

    // get the direction the camera is looking
    vec3 look_dir = *look_at;
    sub_vec(&look_dir, look_from);
    normalize_vec(&look_dir);
    
    init_ray(&(c->look), look_from, &look_dir);

    c->w = look_dir;
    // rtiow has w negative, I guess they use z negative being forward
    normalize_vec(&c->w);

    c->u = cross_vec(up, &c->w);
    normalize_vec(&c->u);

    c->v = cross_vec(&c->w, &c->u);
    normalize_vec(&c->v);

    return 0;
}

