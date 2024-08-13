#ifndef _kernel_material_h_
#define _kernel_material_h_

#include "vec.h"

typedef enum material_type {
    EMISSIVE,
    DIFFUSE,
    MEDIUM,
} material_type;

typedef struct __attribute__ ((packed)) emissive {
    colour c;
} emissive;

typedef struct __attribute__ ((packed)) diffuse {
    colour albedo;
} diffuse;

typedef struct __attribute__ ((packed)) material {
    material_type type;
    union {
        emissive e;
        diffuse d;
    };
} material;

#endif
