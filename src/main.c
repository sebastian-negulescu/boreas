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
    image img = {20, 20, "image.ppm"};

    scene s;

    render(&c, &s, &img);
    
    return 0;
}

