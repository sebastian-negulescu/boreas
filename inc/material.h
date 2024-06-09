#ifndef _material_h_
#define _material_h_

#include "vec.h"
#include "sphere.h"

typedef enum material_type {
    EMISSIVE,
    DIFFUSE,
} material_type;

typedef struct material {
    material_type type;
    void *material_ptr; 
} material;

colour shade_material(material *m, intersection *i, ray *r);

// TODO: this is NOT physically accurate, needs falloff
typedef struct emissive {
    colour c;
} emissive;

colour shade_emissive(emissive *m);

typedef struct diffuse {
    colour albedo;
} diffuse;

colour shade_diffuse(diffuse *m, intersection *i, ray *r);

#endif

