#include "util.h"

const float ERROR = 1e-4;

bool is_within(float x, float check, float margin) {
    if (x <= check + margin && x >= check - margin) {
        return true;
    }

    return false;
}

