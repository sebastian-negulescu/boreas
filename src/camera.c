#include "camera.h"

void init_camera(camera *c, point3 *look_from, point3 *look_at, vec3 *up) {
    c->up = *up;

    // get the direction the camera is looking
    vec3 look_dir = *look_at;
    sub_vec(&look_dir, look_from);
    normalize_vec(&look_dir);

    init_ray(&(c->look), look_from, &look_dir);
}

