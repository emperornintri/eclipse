#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include "mathematics.h"
#include "memory.h"
#include "structures.h"

void backpropagateOutputLayer (dataset1D * true_labels, dataset1D * predicted_labels, dataset1D * gradients);
void backpropagateDenseLayer
(
  float learning_rate, 
  denseLayer * layer,
  dataset1D * input_gradients, 
  dataset1D * output_gradients, 
  dataset1D * activated_input, 
  dataset1D * unactivated_output
);
void backpropagateConvolutionalLayer
(
  float learning_rate, 
  convolutionalLayer * layer,
  dataset2D * input_gradients, 
  dataset2D * output_gradients, 
  dataset2D * activated_input, 
  dataset2D * unactivated_output
);
void backpropagatePoolingLayer
(
  poolingLayer * layer,
  dataset2D * input_gradients, 
  dataset2D * output_gradients, 
  dataset2D * activated_input, 
  dataset2D * unactivated_output
);
void unflatten (dataset1D * input, dataset2D * output, int row_count, int column_count, int channel_count);

#endif