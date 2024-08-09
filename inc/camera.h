#ifndef _camera_h_
#define _camera_h_

#include <CL/cl.h>

#include "vec.h"
#include "ray.h"

typedef struct camera {
    ray look;
    vec3 up;

    cl_float fov;

    // unit vectors of camera's frame
    vec3 u; // x
    vec3 v; // y
    vec3 w; // z
} camera;

int init_camera(camera *c, vec3 *up, point3 *look_from, point3 *look_at, cl_float fov);

#endif

