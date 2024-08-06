#ifndef _material_h_
#define _material_h_

#include "vec.h"
#include "sphere.h"

typedef enum material_type {
    EMISSIVE,
    DIFFUSE,
    MEDIUM,
} material_type;

// TODO: this is NOT physically accurate, needs falloff
typedef struct emissive {
    colour c;
} emissive;

typedef struct diffuse {
    colour albedo;
} diffuse;

typedef struct material {
    material_type type;
    union {
        emissive e;
        diffuse d;
    };
} material;

void init_material_emissive(material *m, emissive *e);
void init_material_diffuse(material *m, diffuse *d);

colour shade_material(material *m, intersection *i, ray *r);
colour shade_emissive(emissive *m);
colour shade_diffuse(diffuse *m, intersection *i, ray *r);

#endif

