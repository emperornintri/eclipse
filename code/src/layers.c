#include "layers.h"

void initializeConvolutionalLayer
(
  convolutionalLayer * layer, 
  int input_channel, 
  int filter_size, 
  int output_channel, 
  int padding, 
  int stride, 
  float ( * activation_function) (float), 
  float ( * activation_function_prime) (float)
)
{
  struct xorshift64_state generator;
  xorshift64Initialization (& generator);
  layer->input_channel = input_channel;
  layer->filter_size = filter_size;
  layer->output_channel = output_channel;
  layer->padding = padding;
  layer->stride = stride;
  layer->activation_function = activation_function;
  layer->activation_function_prime = activation_function_prime;
  int initial_value = filter_size * filter_size * input_channel;
  int total_size = initial_value * output_channel;
  layer->filters = (float * ) malloc (total_size * sizeof(float));
  if (total_size % 2)
  {
    normalFloatEfficient(& generator, layer->filters, (total_size - 1) / 2, 0, squareRoot(2.0 / ((double) initial_value), 1e-15));
    layer->filters[total_size - 1] = (float) (squareRoot(2.0 / ((double) initial_value), 1e-15) * normal(& generator));
  }
  else
  {
    normalFloatEfficient(& generator, layer->filters, total_size / 2, 0, squareRoot(2.0 / ((double) initial_value), 1e-15));
  }
  layer->biases = (float * ) calloc (output_channel, sizeof(float));
}

void initializePoolingLayer
(
  poolingLayer * layer, 
  int filter_size, 
  int padding, 
  int stride, 
  int is_max_pooling
)
{
  layer->filter_size = filter_size;
  layer->padding = padding;
  layer->stride = stride;
  layer->is_max_pooling = is_max_pooling;
}

void initializeDenseLayer
(
  denseLayer * layer, 
  int input_size, 
  int output_size, 
  float ( * activation_function) (float), 
  float ( * activation_function_prime) (float)
)
{
  struct xorshift64_state generator;
  xorshift64Initialization (& generator);
  layer->input_size = input_size;
  layer->output_size = output_size;
  layer->activation_function = activation_function;
  layer->activation_function_prime = activation_function_prime;
  int total_size = input_size * output_size;
  layer->weights = (float * ) malloc (total_size * sizeof(float));
  if (total_size % 2)
  {
    normalFloatEfficient(& generator, layer->weights, (total_size - 1) / 2, 0, squareRoot(2.0 / ((double) input_size), 1e-15));
    layer->weights[total_size - 1] = (float) (squareRoot(2.0 / ((double) input_size), 1e-15) * normal(& generator));
  }
  else
  {
    normalFloatEfficient(& generator, layer->weights, total_size / 2, 0, squareRoot(2.0 / ((double) input_size), 1e-15));
  }
  layer->biases = (float * ) calloc (output_size, sizeof(float));
}