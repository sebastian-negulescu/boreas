#include "intersection.h"
#include "object.h"
#include "vec.h"
#include "ray.h"

__kernel void intersect_scene(
    __global ray *rays, 
    unsigned int num_objects, 
    __global object *scene,
    __global intersection *intersections
) {
    const size_t ray_ind = get_global_id(0);

    // localize ray for faster operation?? idk, I just don't want to use ray_ind
    ray r;
    r.o.x = rays[ray_ind].o.x;
    r.o.y = rays[ray_ind].o.y;
    r.o.z = rays[ray_ind].o.z;
    r.d.x = rays[ray_ind].d.x;
    r.d.y = rays[ray_ind].d.y;
    r.d.z = rays[ray_ind].d.z;

    for (size_t i = 0; i < num_objects; ++i) {
        // ALL objects are spheres so we can access with .s
        intersections[i].hit = true;
    }

}

