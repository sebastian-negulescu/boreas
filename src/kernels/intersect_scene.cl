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

    float3 origin = (float3)(rays[ray_ind].o.x, rays[ray_ind].o.y, rays[ray_ind].o.z);
    float3 direction = (float3)(rays[ray_ind].d.x, rays[ray_ind].d.y, rays[ray_ind].d.z);

    intersections[ray_ind].hit = false;
    for (size_t i = 0; i < num_objects; ++i) {
        float3 position = (float3)(
            scene[i].s.position.x,
            scene[i].s.position.y,
            scene[i].s.position.z
        );
        float3 diff = position;
        diff = diff - origin;

        float a = dot(direction, direction);
        float b = -2.f * dot(direction, diff);
        float c = -(scene[i].s.radius * scene[i].s.radius) + dot(diff, diff);

        if (is_within(a, 0.f, ERROR)) {
            printf("");
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

        intersections[ray_ind].t = t1;

        if (t2 < t1 && t2 >= 0.f) {
            intersections[ray_ind].t = t2;
        }

        if (intersections[ray_ind].t < ERROR) {
            continue;
        }

        intersections[ray_ind].hit = true;

        float3 p = direction;
        p = p * intersections[ray_ind].t;
        p += origin;

        float3 normal = p;
        normal = normal - position;
        normal = normalize(normal);

        intersections[ray_ind].normal.x = normal.x;
        intersections[ray_ind].normal.y = normal.y;
        intersections[ray_ind].normal.z = normal.z;

        intersections[ray_ind].point.x = p.x;
        intersections[ray_ind].point.y = p.y;
        intersections[ray_ind].point.z = p.z;
    }
}

