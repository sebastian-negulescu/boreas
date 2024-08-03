__kernel void mat_mul(
    const int N, __global float *A, __global float *B, __global float *C) {
    int i, j, k;
    float temp;

    // these will get unique values for i, j in the cluster
    i = get_global_id(0);
    j = get_global_id(1);

    temp = 0.f;
    // N is the size of the common dimension between A and B
    for (k = 0; k < N; ++k) {
         temp += A[i * N + k] * B[k * N + j];
    }
    C[i * N + j] = temp;
}

