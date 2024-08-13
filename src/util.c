#include "util.h"

const cl_float ERROR = 1e-4;

cl_bool is_within(cl_float x, cl_float check, cl_float margin) {
    if (x <= check + margin && x >= check - margin) {
        return true;
    }

    return false;
}

