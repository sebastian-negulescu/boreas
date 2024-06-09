#include "material.h"

colour shade_material(material *m, intersection *i, ray *r) {
    switch (m->type) {
        case EMISSIVE:
            return shade_emissive((emissive *)m->material_ptr);
            break;

        case DIFFUSE:
            return shade_diffuse((diffuse *)m->material_ptr, i, r);
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
    return m->albedo;
}

