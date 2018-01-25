/**
 * Author: Khoi Trinh
 * Written for calculating backprop only
 * Included basic neural network struct
 */
#ifndef _BACK_PROP_H
#define _BACK_PROP_H

#define NODE_AMOUNT_0 773
#define NODE_AMOUNT_1 600
#define NODE_AMOUNT_2 400
#define NODE_AMOUNT_3 200
#define NODE_AMOUNT_4 100
#define NODE_AMOUNT_5 200
#define NODE_AMOUNT_6 100
#define NODE_AMOUNT_7 100
#define NODE_AMOUNT_8 100
#define NODE_AMOUNT_9 1
// the node number follows Abel's design

typedef struct
{
  // all arrays following notataion array[row_num][col_num]
  // arrays carrying the weights of the layers
  // this is the input layer
  float layer_weight_0[NODE_AMOUNT_1][NODE_AMOUNT_0];

  // hidden layers
  float layer_weight_1[NODE_AMOUNT_2][NODE_AMOUNT_1];
  float layer_weight_2[NODE_AMOUNT_3][NODE_AMOUNT_2];
  float layer_weight_3[NODE_AMOUNT_4][NODE_AMOUNT_3];
  float layer_weight_4[NODE_AMOUNT_5][NODE_AMOUNT_4];
  float layer_weight_5[NODE_AMOUNT_6][NODE_AMOUNT_5];
  float layer_weight_6[NODE_AMOUNT_7][NODE_AMOUNT_6];
  float layer_weight_7[NODE_AMOUNT_8][NODE_AMOUNT_7];
  float layer_weight_8[NODE_AMOUNT_9][NODE_AMOUNT_8];

  float intput[NODE_AMOUNT_0][1];

  // result of backpropation arranged by layer
  float backprop_res_0[NODE_AMOUNT_1][NODE_AMOUNT_0];
  float backprop_res_1[NODE_AMOUNT_2][NODE_AMOUNT_1];
  float backprop_res_2[NODE_AMOUNT_3][NODE_AMOUNT_2];
  float backprop_res_3[NODE_AMOUNT_4][NODE_AMOUNT_3];
  float backprop_res_4[NODE_AMOUNT_5][NODE_AMOUNT_4];
  float backprop_res_5[NODE_AMOUNT_6][NODE_AMOUNT_5];
  float backprop_res_6[NODE_AMOUNT_7][NODE_AMOUNT_6];
  float backprop_res_7[NODE_AMOUNT_8][NODE_AMOUNT_7];
  float backprop_res_8[NODE_AMOUNT_9][NODE_AMOUNT_8];

} neuralNetwork;

// calculate backprop, assume that activation function is tanh
__global__ void backprop_calCUDA(float* der_error_value,
                                 float* prev_layer_value,
                                 float* result);

// wrapper function for backprop to be called by CPU
void backprop_cal(float* der_error_value,
                  int    node_length,
                  float* prev_layer_value,
                  int    prev_layer_node_length,
                  float* result);

__global__ void der_error_cal_firstCUDA(float* desired_output,
                                        float* actual_output,
                                        float* result);

void der_error_cal_first(float* desired_output,
                         float* actual_output,
                         int    last_node_length,
                         float* result);

// take in the weight belonging to the previous node
// weight input should be a nx1 vector
// the derivative input should be a 1xn vector
__global__ void der_error_output_cal_hiddenCUDA(float* weight,
                                                float* next_layer_der,
                                                int    next_layer_node_lenth,
                                                float* result);

void der_error_output_cal_hidden(float* weight,
                                 int    cur_layer_node_lenth,
                                 float* next_layer_der,
                                 int    next_layer_node_lenth,
                                 float* result);
#endif