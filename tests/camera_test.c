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
    assert(!init_camera(&c, &look_from, &look_at, &up));

    // need to check u, v, w from c
    assert(is_within(magnitude_vec(&c.u), 1.f, ERROR));
    assert(is_within(magnitude_vec(&c.v), 1.f, ERROR));
    assert(is_within(magnitude_vec(&c.w), 1.f, ERROR));

    assert(is_within(c.u.x, 1.f, ERROR));
    assert(is_within(c.v.y, 1.f, ERROR));
    assert(is_within(c.w.z, 1.f, ERROR));

    printf("finished running axis aligned test\n");
}

void arbitrary_points_test() {
    printf("running arbitrary points test...\n");

    point3 look_from;
    point3 look_at;
    vec3 up;

    init_vec(&look_from, 17.f, -13.f, 9.f);
    init_vec(&look_at, -10.f, 4.f, 11.f);
    init_vec(&up, 3.f, 7.f, 1.f);

    camera c;
    assert(!init_camera(&c, &look_from, &look_at, &up));

    // need to check u, v, w from c
    assert(is_within(magnitude_vec(&c.u), 1.f, ERROR));
    assert(is_within(magnitude_vec(&c.v), 1.f, ERROR));
    assert(is_within(magnitude_vec(&c.w), 1.f, ERROR));

    // check u, v, w are orthogonal
    assert(is_within(dot_vec(&c.u, &c.v), 0.f, ERROR));
    assert(is_within(dot_vec(&c.u, &c.w), 0.f, ERROR));
    assert(is_within(dot_vec(&c.v, &c.w), 0.f, ERROR));

    printf("finished arbitraty points test\n");
}

void zero_up_vec() {
    printf("running zero up vec test...\n");

    point3 look_from;
    point3 look_at;
    vec3 up;

    init_vec(&look_from, 0.f, 0.f, 0.f);
    init_vec(&look_at, 0.f, 0.f, 1.f);
    init_vec(&up, 0.f, 0.f, 0.f);

    camera c;
    assert(init_camera(&c, &look_from, &look_at, &up));

    printf("finished zero up vec test\n");
}

void same_look_from_at() {
    printf("running same look from and at test...\n");

    point3 look_from;
    point3 look_at;
    vec3 up;

    init_vec(&look_from, 0.f, 0.f, 0.f);
    init_vec(&look_at, 0.f, 0.f, 0.f);
    init_vec(&up, 0.f, 1.f, 0.f);

    camera c;
    assert(init_camera(&c, &look_from, &look_at, &up));

    printf("finished same look from and at test\n");
}

int main() {
    printf("testing camera...\n");

    axis_aligned_test();
    arbitrary_points_test();
    zero_up_vec();
    same_look_from_at();

    printf("finished testing camera\n");
}

