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

    // for testing purposes
    sphere sph_1;
    point3 sphere_origin = {0.f, 0.f, 3.f};
    sph_1.position = sphere_origin;
    sph_1.radius = 1.f;

    object o_1;
    o_1.object_ptr = (void *)&sph_1;
    o_1.type = SPHERE;
    add_object(&s, &o_1);

    sphere sph_2;
    sphere_origin.z = 10.f;
    sphere_origin.x = 5.f;
    sph_2.position = sphere_origin;
    sph_2.radius = 5.f;

    object o_2;
    o_2.object_ptr = (void *)&sph_2;
    o_2.type = SPHERE;
    add_object(&s, &o_2);

    /*
    sphere sph_3;
    sphere_origin.z = -1.f;
    sphere_origin.x = 0.f;
    sph_3.position = sphere_origin;
    sph_3.radius = .5f;

    object o_3;
    o_3.object_ptr = (void *)&sph_3;
    o_3.type = SPHERE;
    add_object(&s, &o_3);
    */

    render(&c, &s, &img);
    
    return 0;
}

