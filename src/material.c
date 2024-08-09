#include <stdlib.h>
#include <math.h>

#include "util.h"
#include "material.h"
#include "vec.h"

void init_material_emissive(material *m, emissive *e) {
    m->type = EMISSIVE;
    m->e = *e;
}

void init_material_diffuse(material *m, diffuse *d) {
    m->type = DIFFUSE;
    m->d = *d;
}

colour shade_material(material *m, intersection *i, ray *r) {
    switch (m->type) {
        case EMISSIVE:
            return shade_emissive(&m->e);
            break;

        case DIFFUSE:
            return shade_diffuse(&m->d, i, r);
            break;

        default: break;
    }

    colour black = {0.f, 0.f, 0.f};
    return black;
}

colour shade_emissive(emissive *m) {
    return m->c; 
}

colour shade_diffuse(diffuse *m, intersection *i, ray *r) {
    float u_1 = (float) rand() / (float) RAND_MAX;
    float u_2 = (float) rand() / (float) RAND_MAX;

    float phi = acosf(2 * u_1 - 1);
    float theta = 2 * M_PI * u_2;

    vec3 random_unit_vec = {
        cosf(theta) * sinf(phi),
        sinf(theta) * sinf(phi),
        cosf(phi)
    };

    point3 bounce;
    init_zero_vec(&bounce);
    add_vec(&bounce, &i->point);
    add_vec(&bounce, &i->normal);
    add_vec(&bounce, &random_unit_vec);

    vec3 bounce_dir = bounce;
    sub_vec(&bounce_dir, &i->point);
    normalize_vec(&bounce_dir);

    r->origin = i->point;
    r->direction = bounce_dir;

    return m->albedo;
}

