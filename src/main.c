#include <stdio.h>
#include "vec.h"
#include "camera.h"
#include "image.h"
#include "scene.h"
#include "render.h"

int main(int, char *[]) {
    // create a camera
    point3 look_from = {0.f, 0.f, 0.f};
    point3 look_at = {0.f, 0.f, 1.f};
    vec3 up = {0.f, 1.f, 0.f};

    camera c;
    init_camera(&c, &look_from, &look_at, &up);

    // define image properties
    image img = {200, 200, "image.ppm"};

    scene s;
    init_scene(&s);

    // for testing purposes
    // defining scene, eventually will move to allow for heap allocation too
    emissive e_1;
    init_vec(&e_1.c, 1.f, 1.f, 1.f);

    diffuse d_2;
    init_vec(&d_2.albedo, .5f, .5f, .5f);

    material m_1;
    // init_material(&m_1, EMISSIVE, &e_1);
    init_material(&m_1, EMISSIVE, &e_1);

    sphere sph_1;
    point3 sphere_origin = {0.f, 0.f, 1.f};
    init_sphere(&sph_1, &sphere_origin, 0.5f);

    object o_1;
    init_object(&o_1, SPHERE, &sph_1, &m_1);
    add_object(&s, &o_1);
    
    material m_2;
    init_material(&m_2, DIFFUSE, &d_2);

    sphere_origin.z = 1.f;
    sphere_origin.y = -100.5f;
    sphere sph_2;
    init_sphere(&sph_2, &sphere_origin, 100.f);

    object o_2;
    init_object(&o_2, SPHERE, &sph_2, &m_2);
    add_object(&s, &o_2);

    render(&c, &s, &img);
    
    return 0;
}

