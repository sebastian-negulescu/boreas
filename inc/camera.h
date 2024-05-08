#ifndef _camera_h_
#define _camera_h_

#include "vec.h"

typedef struct camera {
    vec3 up;
    point3 look_from;
    point3 look_at;
} camera;

void init_camera(camera *c, vec3 *up, point3 *look_from, point3 *look_at);

#endif

