#ifndef _kernel_material_h_
#define _kernel_material_h_

#include "vec.h"

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

#endif
