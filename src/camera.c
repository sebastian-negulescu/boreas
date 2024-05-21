// #include <stdio.h>
// #include <stdlib.h>
#include "camera.h"

void init_camera(camera *c, point3 *look_from, point3 *look_at, vec3 *up) {
    c->up = *up;

    // get the direction the camera is looking
    vec3 look_dir = *look_at;
    sub_vec(&look_dir, look_from);
    normalize_vec(&look_dir);
    
    init_ray(&(c->look), look_from, &look_dir);

    c->w = look_dir;
    // flip w vector to point backwards (idk why, it shouldn't matter)
    mult_vec(&c->w, -1.f);
    normalize_vec(&c->w);

    // rtiow has w and up flipped, resulting in negative u
    c->u = cross_vec(&c->w, up);
    normalize_vec(&c->u);

    // rtiow has u and w flipped, resulting in negative v
    c->v = cross_vec(&c->u, &c->w);
    normalize_vec(&c->v);

    /*
    const char *w_vec_str = string_vec(&c->w);
    printf("w: %s\n", w_vec_str);
    free((void *)w_vec_str);

    const char *v_vec_str = string_vec(&c->v);
    printf("v: %s\n", v_vec_str);
    free((void *)v_vec_str);

    const char *u_vec_str = string_vec(&c->u);
    printf("u: %s\n", u_vec_str);
    free((void *)u_vec_str);
    */
}

