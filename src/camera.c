#include "camera.h"

void init_camera(camera *c, vec3 *up, point3 *look_from, point3 *look_at) {
    c->up = *up;
    c->look_from = *look_from;
    c->look_at = *look_at;
}

