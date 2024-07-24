#include <math.h>
#include <stdio.h>
#include <assert.h>
#include "util.h"
#include "render.h"
#include "vec.h"

void modification_test() {
    printf("running modification test...\n");

    point3 look_from = {0.f, 0.f, 0.f};
    point3 look_at = {0.f, 0.f, 1.f};
    vec3 up = {0.f, 1.f, 0.f};

    camera c;
    init_camera(&c, &look_from, &look_at, &up);

    struct pane_info test_pane;
    init_pane_info(&test_pane, &c, 600, 400, 90.f);

    // 90 h_fov
    // 60 v_fov

    vec3 x_mod_normalized = test_pane.x_mod_base;
    vec3 y_mod_normalized = test_pane.y_mod_base;
    normalize_vec(&x_mod_normalized);
    normalize_vec(&y_mod_normalized);

    vec3 x_mod_correct = {1.f, 0.f, 0.f};
    vec3 y_mod_correct = {0.f, -1.f, 0.f};

    float x_mod_magnitude = magnitude_vec(&test_pane.x_mod_base);
    float y_mod_magnitude = magnitude_vec(&test_pane.y_mod_base);

    assert(is_within(dot_vec(&x_mod_normalized, &x_mod_correct), 1.f, ERROR));
    assert(is_within(dot_vec(&y_mod_normalized, &y_mod_correct), 1.f, ERROR));

    assert(is_within(x_mod_magnitude, 1.f / (600.f / 2.f), ERROR));
    assert(is_within(y_mod_magnitude, (1.f / sqrtf(3.f)) / (400.f / 2.f), ERROR)); 

    printf("finished running modification test\n");
}

void fov_test() {
    printf("running fov test...\n");

    point3 look_from = {0.f, 0.f, 0.f};
    point3 look_at = {0.f, 0.f, 1.f};
    vec3 up = {0.f, 1.f, 0.f};

    camera c;
    init_camera(&c, &look_from, &look_at, &up);

    struct pane_info test_pane;
    init_pane_info(&test_pane, &c, 1, 1, 90.f);

    vec3 correct_top_left = {-1.f, 1.f, 1.f};
    normalize_vec(&correct_top_left);

    vec3 normalized_top_left = test_pane.top_left;
    normalize_vec(&normalized_top_left);
    
    float dot_product = dot_vec(&normalized_top_left, &correct_top_left);
    assert(is_within(dot_product, 1.f, ERROR));
    
    printf("finished running fov test\n");
}

int main() {
    printf("testing ray casting...\n");

    modification_test();
    fov_test();

    printf("finished testing ray casting\n");
    return 0;
}

