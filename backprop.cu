/**
 * Author: Khoi Trinh
 * Implementation of backprop function in CUDA
 */

#include <assert.h>
#include "backprop.h"
#include "matrix.h"

// no need to include CUDA libraries since
// nvcc automatically takes care of dependencies

__global__ float* backprop_cal(float* desired_output, float* actual_output,
                               float* output_prev_layer) {
  assert(desired_output);
  assert(actual_output);
  assert(output_prev_layer);
}