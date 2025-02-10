#include "network.h"

void initializeLeNet5(LeNet5 * network)
{
  initializeConvolutionalLayer(& network->c1, 1, 5, 6, 2, 1, rectifiedLinearUnit, rectifiedLinearUnitPrime);
  initializePoolingLayer(& network->s2, 2, 0, 2, 0);
  initializeConvolutionalLayer(& network->c3, 6, 5, 16, 0, 1, rectifiedLinearUnit, rectifiedLinearUnitPrime);
  initializePoolingLayer(& network->s4, 2, 0, 2, 0);
  initializeDenseLayer(& network->c5, 400, 120, rectifiedLinearUnit, rectifiedLinearUnitPrime);
  initializeDenseLayer(& network->f6, 120, 84, rectifiedLinearUnit, rectifiedLinearUnitPrime);
  initializeDenseLayer(& network->output, 84, 10, identity, identityPrime);
}

void freeLeNet5(LeNet5 * network)
{
  free(network->c1.biases);
  free(network->c1.filters);
  free(network->c3.biases);
  free(network->c3.filters);
  free(network->c5.biases);
  free(network->c5.weights);
  free(network->f6.biases);
  free(network->f6.weights);
  free(network->output.biases);
  free(network->output.weights);
}

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
)
{
  convolve(& network->c1, input_2d_1, input_2d_2);
  pool(& network->s2, input_2d_2, input_2d_3);
  convolve(& network->c3, input_2d_3, input_2d_4);
  pool(& network->s4, input_2d_4, input_2d_5);
  flatten(input_2d_5, input_1d_1);
  connect(& network->c5, input_1d_1, input_1d_2);
  connect(& network->f6, input_1d_2, input_1d_3);
  connect(& network->output, input_1d_3, output);
}

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
)
{
  backpropagateOutputLayer(labels, output, output_gradients);
  backpropagateDenseLayer(learning_rate, & network->output, input_1d_3_gradients, output_gradients, input_1d_3, output);
  backpropagateDenseLayer(learning_rate, & network->f6, input_1d_2_gradients, input_1d_3_gradients, input_1d_2, input_1d_3);
  backpropagateDenseLayer(learning_rate, & network->c5, input_1d_1_gradients, input_1d_2_gradients, input_1d_1, input_1d_2);
  unflatten(input_1d_1_gradients, input_2d_5_gradients, input_2d_5->row_count, input_2d_5->column_count, input_2d_5->channel_count);
  backpropagatePoolingLayer(& network->s4, input_2d_4_gradients, input_2d_5_gradients, input_2d_4, input_2d_5);
  backpropagateConvolutionalLayer(learning_rate, & network->c3, input_2d_3_gradients, input_2d_4_gradients, input_2d_3, input_2d_4);
  backpropagatePoolingLayer(& network->s2, input_2d_2_gradients, input_2d_3_gradients, input_2d_2, input_2d_3);
  backpropagateConvolutionalLayer(learning_rate, & network->c1, input_2d_1_gradients, input_2d_2_gradients, input_2d_1, input_2d_2);
}  