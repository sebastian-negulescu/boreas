#include "kernels/vec.h"
#include "kernels/material.h"

__kernel void shade_emissive(
    __global colour *ray_colours,
    __global emissive *e
) {
    const size_t ray_ind = get_global_id(0);

    float3 ray_colour = get_vec(ray_colours[ray_ind]);
    float3 emissive_colour = get_vec(e->c);

    ray_colour *= emissive_colour;

    set_global_vec(&ray_colours[ray_ind], ray_colour);
}

