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

typedef struct {
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
__global__ float* backprop_cal(float* desired_output, float* actual_output,
                               float* output_prev_layer);
#endif