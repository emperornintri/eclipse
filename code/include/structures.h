#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct convolutionalLayer
{
  int input_channel;
  int filter_size;
  int output_channel;
  int padding;
  int stride;
  float * biases;
  float * filters;
  float ( * activation_function) (float);
  float ( * activation_function_prime) (float);
}
convolutionalLayer;

typedef struct poolingLayer
{
  int filter_size;
  int padding;
  int stride;
  int is_max_pooling;
}
poolingLayer;

typedef struct denseLayer
{
  int input_size;
  int output_size;
  float * biases;
  float * weights;
  float ( * activation_function) (float);
  float ( * activation_function_prime) (float);
}
denseLayer;

typedef struct dataset1D
{
  float * samples;
  int sample_count;
  int column_count;
}
dataset1D;

typedef struct dataset2D
{
  float * samples;
  int sample_count;
  int row_count;
  int column_count;
  int channel_count;
}
dataset2D;

typedef struct LeNet5
{
  struct convolutionalLayer c1;
  struct poolingLayer s2;
  struct convolutionalLayer c3;
  struct poolingLayer s4;
  struct denseLayer c5;
  struct denseLayer f6;
  struct denseLayer output;
}
LeNet5;

#endif