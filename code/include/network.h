#ifndef NETWORK_H
#define NETWORK_H

#include "backpropagation.h"
#include "forward.h"
#include "structures.h"
#include "layers.h"

void initializeLeNet5(LeNet5 * network, int resized);
void freeLeNet5(LeNet5 * network);
void forwardLeNet5
(
  LeNet5 * network, 
  dataset2D * input_2d_1, 
  dataset2D * input_2d_2, 
  dataset2D * input_2d_3, 
  dataset2D * input_2d_4, 
  dataset2D * input_2d_5,
  dataset1D * input_1d_1, 
  dataset1D * input_1d_2, 
  dataset1D * input_1d_3, 
  dataset1D * output
);
void backwardLeNet5
(
  float learning_rate,
  LeNet5 * network, 
  dataset1D * labels,
  dataset2D * input_2d_1, 
  dataset2D * input_2d_2, 
  dataset2D * input_2d_3, 
  dataset2D * input_2d_4, 
  dataset2D * input_2d_5,
  dataset1D * input_1d_1, 
  dataset1D * input_1d_2, 
  dataset1D * input_1d_3, 
  dataset1D * output,
  dataset2D * input_2d_1_gradients, 
  dataset2D * input_2d_2_gradients, 
  dataset2D * input_2d_3_gradients, 
  dataset2D * input_2d_4_gradients, 
  dataset2D * input_2d_5_gradients,
  dataset1D * input_1d_1_gradients, 
  dataset1D * input_1d_2_gradients, 
  dataset1D * input_1d_3_gradients, 
  dataset1D * output_gradients
);

#endif