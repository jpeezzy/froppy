/**
 * Author: Khoi Trinh
 * Implementation of backprop function in CUDA
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "backprop.h"
#include "matrix.h"

#define THREAD_PER_BLOCK 64
#define REGISTERS_PER_THREAD 32
#define SHARED_MEMORY_PER_BLOCK 2048

// no need to include CUDA libraries since
// nvcc automatically takes care of dependencies

float der_error_cal_first(float desired_output, float actual_output)

{
  return actual_output - desired_output;
}

// using equation 6 in the CUDA backprop paper
// 1 block for each element in the result
__global__ void der_error_input_cal_hiddenCUDA(float* weight,
                                           int    prev_layer_node_lenth,
                                           float* cur_layer_der,
                                           int    cur_layer_node_lenth,
                                           float* prev_layer_value,
                                           float* result)
{
  int              weight_id    = blockIdx.x;
  int              cur_layer_id = threadIdx.x;
  float            sum          = 0;
  extern __shared__ float temp_sum[];
  // every element compute one multiplication then thread 0 add them up
  temp_sum[cur_layer_id] =
      weight[cur_layer_id * cur_layer_node_lenth + weight_id] *
      cur_layer_der[cur_layer_id];
  __syncthreads();
  if (threadIdx.x == 0)
    {
#pragma unroll
      for (int i = 0; i < cur_layer_node_lenth; ++i)
        {
          sum += temp_sum[i];
        }
      sum *= (1 - powf(tanhf(prev_layer_value[weight_id]), 2));
      result[weight_id] = sum;
    }
}
// expect the  to be an nx1 vectors
// and the weight to be a nxm, where n is the node
// length of the  layer and m is the node length
// of the current layer
// calculate the derivative of the pevious layer
void der_error_input_cal_hidden(float* weight,
                                int    prev_layer_node_lenth,
                                float* cur_layer_der,int    cur_layer_node_lenth,
                                float* prev_layer_value,
                                float* result)
{
  assert(weight); assert(cur_layer_der);
  assert(result);
  float* cuda_weight           = NULL;
  float* cuda_cur_layer_der    = NULL;
  float* cuda_prev_layer_value = NULL;
  float* cuda_result           = NULL;

  int size_w               = sizeof(weight);
  int size_cur_layer_der   = sizeof(cur_layer_der);
  int size_result          = sizeof(result);
  int size_cur_layer_value = sizeof(prev_layer_value);
  cudaMalloc((void**)&cuda_weight, size_w);
  cudaMalloc((void**)&cuda_cur_layer_der, size_cur_layer_der);
  cudaMalloc((void**)&cuda_result, size_result);
  cudaMalloc((void**)&cuda_prev_layer_value, size_cur_layer_value);

  cudaMemcpy(cuda_weight, weight, size_w, cudaMemcpyHostToDevice);
  cudaMemcpy(cuda_cur_layer_der,
             cur_layer_der,
             size_cur_layer_der,
             cudaMemcpyHostToDevice);
  cudaMemcpy(cuda_prev_layer_value,
             prev_layer_value,
             size_cur_layer_value,
             cudaMemcpyHostToDevice);

  der_error_input_cal_hiddenCUDA <<< prev_layer_node_lenth,
      cur_layer_node_lenth, cur_layer_node_lenth*sizeof(float) >>> (cuda_weight,
                               prev_layer_node_lenth,
                               cuda_cur_layer_der,cur_layer_node_lenth,
                               cuda_prev_layer_value,
                               cuda_result);
  cudaMemcpy(result, cuda_result, size_result, cudaMemcpyDeviceToHost);

  cudaFree(cuda_cur_layer_der);
  cudaFree(cuda_result);
  cudaFree(cuda_prev_layer_value);
}

__global__ void backprop_calCUDA(float* der_error_value,
                                 float* prev_layer_value,int prev_layer_node_length,
                                 float* result)
{
  int weight_id_x    = blockIdx.x;
  int weight_id_y    = blockIdx.y;
  int cur_layer_id_x = threadIdx.x;
  int cur_layer_id_y = threadIdx.y;
  result[(blockDim.x * weight_id_x + cur_layer_id_x)*prev_layer_node_length+ blockDim.y * weight_id_y + cur_layer_id_y] =
            der_error_value[blockDim.y * weight_id_y + cur_layer_id_y] *
            prev_layer_value[blockDim.x * weight_id_x + cur_layer_id_x];
}

void backprop_cal(float* der_error_value,
                  int    node_length,
                  float* prev_layer_value,
                  int    prev_layer_node_length,
                  float* result)
{
  assert(der_error_value);
  assert(prev_layer_value);
  assert(result);
  float*    cuda_der_error_value  = NULL;
  float*    cuda_prev_layer_value = NULL;
  float*    cuda_result           = NULL;
  const int block_size            = 32;

  dim3 block_index(node_length / block_size, prev_layer_node_length / block_size);

  // memory allocations
  int size_der_error_value  = sizeof(der_error_value);
  int size_prev_layer_value = sizeof(prev_layer_value);
  int size_result           = sizeof(result);
  cudaMalloc((void**)&cuda_der_error_value, size_der_error_value);
  cudaMalloc((void**)&cuda_prev_layer_value, size_prev_layer_value);
  cudaMalloc((void**)&cuda_result, size_result);

  backprop_calCUDA <<< block_index, block_size >>> (cuda_der_error_value,cuda_prev_layer_value,prev_layer_node_length,cuda_result);

  // cleanup
  cudaMemcpy(result, cuda_result, size_result, cudaMemcpyDeviceToHost);
  cudaFree(cuda_der_error_value);
  cudaFree(cuda_prev_layer_value);
  cudaFree(cuda_result);
}