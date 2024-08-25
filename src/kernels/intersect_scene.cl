#include "kernels/intersection.h"
#include "kernels/object.h"
#include "kernels/vec.h"
#include "kernels/ray.h"
#include "kernels/util.h"


__kernel void intersect_scene(
    __global ray *rays, 
    unsigned int num_objects, 
    __global object *scene,
    __global intersection *intersections
) {
    const size_t ray_ind = get_global_id(0);

    float3 origin = get_vec(rays[ray_ind].o);
    float3 direction = get_vec(rays[ray_ind].d);

    intersections[ray_ind].hit = false;
    for (size_t i = 0; i < num_objects; ++i) {
        intersection curr_inter;
        curr_inter.hit = false;

        float3 position = get_vec(scene[i].s.position);     
        float radius = scene[i].s.radius;
        float3 diff = position;
        diff = diff - origin;

        float a = dot(direction, direction);
        float b = -2.f * dot(direction, diff);
        float c = -(radius * radius) + dot(diff, diff);

        if (is_within(a, 0.f, ERROR)) {
            continue;
        }

        float discriminant = b * b - 4.f * a * c;
        if (is_within(discriminant, 0.f, ERROR)) {
            discriminant = 0.f;
        }
        if (discriminant < 0.f) {
            continue; 
        }

        float discriminant_sqrt = sqrt(discriminant);

        float t1 = (-b + discriminant_sqrt) / (2 * a);
        float t2 = (-b - discriminant_sqrt) / (2 * a);

        curr_inter.t = t1;

        if (t2 < t1 && t2 >= 0.f) {
            curr_inter.t = t2;
        }

        if (curr_inter.t < ERROR) {
            continue;
        }

        // doesn't matter
        curr_inter.hit = true;

        float3 p = direction;
        p = p * curr_inter.t;
        p += origin;

        float3 normal = p;
        normal = normal - position;
        normal = normalize(normal);

        set_private_vec(&curr_inter.normal, normal);
        set_private_vec(&curr_inter.point, p);

        // update intersection if necessary
        if (!intersections[ray_ind].hit || curr_inter.t < intersections[ray_ind].t) {
            intersections[ray_ind].hit = true;
            intersections[ray_ind].t = curr_inter.t;
            copy_global_vec(&intersections[ray_ind].normal, &curr_inter.normal);
            copy_global_vec(&intersections[ray_ind].point, &curr_inter.point);
        }
    }
}

