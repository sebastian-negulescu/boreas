#ifndef _image_h_
#define _image_h_

#include <stddef.h>

typedef struct image {
    size_t width;
    size_t height;

    size_t samples;

    const char *filename;
} image;

#endif

