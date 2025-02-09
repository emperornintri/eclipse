#ifndef LAYERS_H
#define LAYERS_H

#include "memory.h"
#include "random.h"
#include "structures.h"

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
);
void initializePoolingLayer
(
  poolingLayer * layer, 
  int filter_size, 
  int padding, 
  int stride, 
  int is_max_pooling
);
void initializeDenseLayer
(
  denseLayer * layer, 
  int input_size, 
  int output_size, 
  float ( * activation_function) (float), 
  float ( * activation_function_prime) (float)
);

#endif