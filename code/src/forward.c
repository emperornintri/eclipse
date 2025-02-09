#include "forward.h"

static inline void calculateIndices (int output_position, int stride, int padding, int filter_size, int input_size, int * input_start, int * filter_start, int * filter_end)
{
    * input_start = output_position * stride - padding;
    * filter_start = ( * input_start < 0) ? - * input_start : 0;
    * filter_end = ( * input_start + filter_size > input_size) ? filter_size - ( * input_start + filter_size - input_size) : filter_size;
}

static inline float performConvolution
(
  int output_sample, int output_channel,
  int input_row_start, int input_column_start,
  int input_sample_size, int input_row_size, int input_column_size,
  int filter_row_start, int filter_row_end,
  int filter_column_start, int filter_column_end,
  const convolutionalLayer * layer, const dataset2D * input
) 
{
  float convolution_value = 0.0f;
  for (int filter_row = filter_row_start; filter_row < filter_row_end; filter_row++) 
  {
    for (int filter_column = filter_column_start; filter_column < filter_column_end; filter_column++) 
    {
      int filter_position = filter_row * layer->filter_size * layer->input_channel * layer->output_channel + filter_column * layer->input_channel * layer->output_channel + output_channel;
      int input_position = output_sample * input_sample_size 
                         + (input_row_start + filter_row) * input_row_size 
                         + (input_column_start + filter_column) * input_column_size;
      for (int input_channel = 0; input_channel < input->channel_count; input_channel++) {
        filter_position += input_channel * layer->output_channel;
        convolution_value += layer->filters[filter_position] * input->samples[input_position + input_channel];
      }
    }
  }
  return layer->activation_function(convolution_value + layer->biases[output_channel]);
}

static inline float performPooling
(
  int output_sample, int output_channel,
  int input_row_start, int input_column_start,
  int input_sample_size, int input_row_size, int input_column_size,
  int filter_row_start, int filter_row_end,
  int filter_column_start, int filter_column_end,
  const struct poolingLayer * layer, const struct dataset2D * input
) 
{
  if (layer->is_max_pooling)
  {
    float pooling_value = -1.0f / 0.0f;
    for (int filter_row = filter_row_start; filter_row < filter_row_end; filter_row++) 
    {
      for (int filter_column = filter_column_start; filter_column < filter_column_end; filter_column++) 
      {
        int input_position = output_sample * input_sample_size
                           + (input_row_start + filter_row) * input_row_size
                           + (input_column_start + filter_column) * input_column_size + output_channel;
        if (input->samples[input_position] >= pooling_value)
        {
          pooling_value = input->samples[input_position];
        }
      }
    }
    return pooling_value;
  }
  else
  {
    float pooling_value = 0.0f;
    for (int filter_row = filter_row_start; filter_row < filter_row_end; filter_row++) 
    {
      for (int filter_column = filter_column_start; filter_column < filter_column_end; filter_column++) 
      {
        int input_position = output_sample * input_sample_size
                           + (input_row_start + filter_row) * input_row_size
                           + (input_column_start + filter_column) * input_column_size + output_channel;
        pooling_value += input->samples[input_position];
      }
    }
    return pooling_value / (layer->filter_size * layer->filter_size);  
  }
}

void convolve (convolutionalLayer * layer, dataset2D * input, dataset2D * output)
{
  int total_size, sample_size, row_size, column_size;
  int input_sample_size, input_row_size, input_column_size;

  output->sample_count = input->sample_count;
  output->row_count = (int) ((input->row_count + 2 * layer->padding - layer->filter_size) / layer->stride) + 1;
  output->column_count = (int) ((input->column_count + 2 * layer->padding - layer->filter_size) / layer->stride) + 1;
  output->channel_count = layer->output_channel;

  column_size = output->channel_count;
  row_size = column_size * output->column_count;
  sample_size = row_size * output->row_count;
  total_size = sample_size * output->sample_count;

  input_column_size = input->channel_count;
  input_row_size = input_column_size * input->column_count;
  input_sample_size = input_row_size * input->row_count;
  output->samples = malloc (total_size * sizeof(float));
  int output_sample, output_row, output_col, output_channel;
  int filter_row_start, filter_row_end, filter_column_start, filter_column_end, input_row_start, input_column_start;

  int index = 0;
  float value;
  for (int output_sample = 0; output_sample < output->sample_count; output_sample++)
  {
    for (int output_row = 0; output_row < output->row_count; output_row++)
    {
      calculateIndices(output_row, layer->stride, layer->padding, layer->filter_size, input->row_count, & input_row_start, & filter_row_start, & filter_row_end);
      for (int output_column = 0; output_column < output->column_count; output_column++)
      {
        calculateIndices(output_column, layer->stride, layer->padding, layer->filter_size, input->column_count, & input_column_start, & filter_column_start, & filter_column_end);
        for (int output_channel = 0; output_channel < output->channel_count; output_channel++)
        {
          output->samples[index++] = performConvolution
          (
            output_sample, output_channel,
            input_row_start, input_column_start,
            input_sample_size, input_row_size, input_column_size,
            filter_row_start, filter_row_end,
            filter_column_start, filter_column_end,
            layer, input
          );
        }
      }
    }
  }
}

void pool (poolingLayer * layer, dataset2D * input, dataset2D * output)
{
  int total_size, sample_size, row_size, column_size;
  int input_sample_size, input_row_size, input_column_size;

  output->sample_count = input->sample_count;
  output->row_count = (int) ((input->row_count + 2 * layer->padding - layer->filter_size) / layer->stride) + 1;
  output->column_count = (int) ((input->column_count + 2 * layer->padding - layer->filter_size) / layer->stride) + 1;
  output->channel_count = input->channel_count;

  column_size = output->channel_count;
  row_size = column_size * output->column_count;
  sample_size = row_size * output->row_count;
  total_size = sample_size * output->sample_count;

  input_column_size = input->channel_count;
  input_row_size = input_column_size * input->column_count;
  input_sample_size = input_row_size * input->row_count;

  output->samples = malloc (total_size * sizeof(float));

  int output_sample, output_row, output_col, output_channel;
  int filter_row_start, filter_row_end, filter_column_start, filter_column_end, input_row_start, input_column_start;
  int filter_position, input_position;

  int index = 0;
  float value;

  for (int output_sample = 0; output_sample < output->sample_count; output_sample++)
  {
    for (int output_row = 0; output_row < output->row_count; output_row++)
    {
      calculateIndices(output_row, layer->stride, layer->padding, layer->filter_size, input->row_count, & input_row_start, & filter_row_start, & filter_row_end);
      for (int output_column = 0; output_column < output->column_count; output_column++)
      {
        calculateIndices(output_column, layer->stride, layer->padding, layer->filter_size, input->column_count, & input_column_start, & filter_column_start, & filter_column_end);
        for (int output_channel = 0; output_channel < output->channel_count; output_channel++)
        {
          output->samples[index++] = performPooling
          (
            output_sample, output_channel,
            input_row_start, input_column_start,
            input_sample_size, input_row_size, input_column_size,
            filter_row_start, filter_row_end,
            filter_column_start, filter_column_end,
            layer, input
          );
        }
      }
    }
  }
}

void flatten (dataset2D * input, dataset1D * output) {
    output->sample_count = input->sample_count;
    output->column_count = input->row_count * input->column_count * input->channel_count;
    output->samples = input->samples;
}

void connect (denseLayer * layer, dataset1D * input, dataset1D * output)
{
  int input_sample_index, output_sample_index, output_weight_index;

  float value;

  output->sample_count = input->sample_count;
  output->column_count = layer->output_size;

  output->samples = malloc (output->sample_count * output->column_count * sizeof(float));

  for (int sample = 0; sample < output->sample_count; sample++)
  {
    input_sample_index = sample * input->column_count;
    output_sample_index = sample * layer->output_size;
    for (int output_index = 0; output_index < layer->output_size; output_index++)
    {
      value = layer->biases[output_index];
      output_weight_index = output_index * layer->input_size;
      for (int input_index = 0; input_index < layer->input_size; input_index++)
      {
        value += layer->weights[output_weight_index + input_index] * input->samples[input_sample_index + input_index];
      }
      output->samples[output_sample_index + output_index] = layer->activation_function(value); 
    }
  }
}