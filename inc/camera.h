#ifndef _camera_h_
#define _camera_h_

#include "vec.h"
#include "ray.h"

typedef struct camera {
    ray look;

    vec3 up;
} camera;

void init_camera(camera *c, vec3 *up, point3 *look_from, point3 *look_at);

#endif

