#ifndef _kernel_util_h_
#define _kernel_util_h_

__constant float ERROR = 1e-4;

inline bool is_within(float x, float check, float margin) {
    if (x <= check + margin && x >= check - margin) {
        return true;
    }

    return false;
}

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#endif

