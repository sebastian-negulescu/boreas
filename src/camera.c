#include "camera.h"

void init_camera(camera *c, point3 *look_from, point3 *look_at, vec3 *up) {
    c->look_from = *look_from;
    c->look_at = *look_at;
    c->up = *up;

    // get the direction the camera is looking
    c->look_dir = *look_at;
    sub_vec(&(c->look_dir), look_from);
    // TODO: normalize the vector
}

