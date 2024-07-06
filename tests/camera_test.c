#include <stdio.h>
#include <assert.h>

#include "camera.h"
#include "vec.h"
#include "util.h"

void axis_aligned_test() {
    printf("running axis aligned test...\n");

    point3 look_from;
    point3 look_at;
    vec3 up;

    init_vec(&look_from, 0.f, 0.f, 0.f);
    init_vec(&look_at, 0.f, 0.f, 1.f);
    init_vec(&up, 0.f, 1.f, 0.f);

    camera c;
    init_camera(&c, &look_from, &look_at, &up);

    // need to check u, v, w from c
    assert(is_within(magnitude_vec(&c.u), 1.f, ERROR));
    assert(is_within(magnitude_vec(&c.v), 1.f, ERROR));
    assert(is_within(magnitude_vec(&c.w), 1.f, ERROR));

    assert(is_within(c.u.x, 1.f, ERROR));
    assert(is_within(c.v.y, 1.f, ERROR));
    assert(is_within(c.w.z, -1.f, ERROR));

    printf("finished running axis aligned test\n");
}

int main() {
    printf("testing camera...\n");

    axis_aligned_test();

    printf("finished testing camera\n");
}

