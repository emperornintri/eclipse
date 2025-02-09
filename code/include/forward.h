#ifndef FORWARD_H
#define FORWARD_H

#include "memory.h"
#include "structures.h"

static inline void calculateIndices (int output_position, int stride, int padding, int filter_size, int input_size, int * input_start, int * filter_start, int * filter_end);
static inline float performConvolution
(
  int output_sample, int output_channel,
  int input_row_start, int input_column_start,
  int input_sample_size, int input_row_size, int input_column_size,
  int filter_row_start, int filter_row_end,
  int filter_column_start, int filter_column_end,
  const convolutionalLayer * layer, const dataset2D * input
);
static inline float performPooling 
(
  int output_sample, int output_channel,
  int input_row_start, int input_column_start,
  int input_sample_size, int input_row_size, int input_column_size,
  int filter_row_start, int filter_row_end,
  int filter_column_start, int filter_column_end,
  const poolingLayer * layer, const dataset2D * input
);
void convolve (convolutionalLayer * layer, dataset2D * input, dataset2D * output);
void pool (poolingLayer * layer, dataset2D * input, dataset2D * output);
void flatten (dataset2D * input, dataset1D * output);
void connect (denseLayer * layer, dataset1D * input, dataset1D * output);

#endif