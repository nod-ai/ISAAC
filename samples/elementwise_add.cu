__global__ void add(const float *a, const float *b, const float *c) {
  uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
  c[idx] = a[idx] + b[idx];
}
