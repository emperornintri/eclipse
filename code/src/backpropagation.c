#include "backpropagation.h"

void backpropagateOutputLayer (dataset1D * true_labels, dataset1D * predicted_labels, dataset1D * gradients)
{
  int sample_count = true_labels->sample_count;
  int class_number = true_labels->column_count;

  gradients->sample_count = sample_count;
  gradients->column_count = class_number;
  gradients->samples = calloc(sample_count * class_number, sizeof(float));

  for (int sample = 0; sample < sample_count; sample++)
  {
    int sample_index_base = sample * class_number;
    float probabilities[class_number];
    
    softmax (class_number, predicted_labels->samples + sample_index_base, probabilities);
  
    for (int class_index = 0; class_index < class_number; class_index++)
    {
      gradients->samples[sample_index_base + class_index] = (probabilities[class_index] - true_labels->samples[sample_index_base + class_index]) / sample_count;
    }
  }
}

void backpropagateDenseLayer
(
  float learning_rate, 
  denseLayer * layer,
  dataset1D * input_gradients, 
  dataset1D * output_gradients, 
  dataset1D * activated_input, 
  dataset1D * unactivated_output
)
{
  int sample_count = activated_input->sample_count;

  int input_size = layer->input_size;
  int output_size = layer->output_size;

  input_gradients->sample_count = sample_count;
  input_gradients->column_count = input_size;
  input_gradients->samples = calloc(sample_count * input_size, sizeof(float));

  for (int sample_index = 0; sample_index < sample_count; sample_index++)
  {
    int input_sample_offset = sample_index * input_size;
    int output_sample_offset = sample_index * output_size;
    for (int output_index = 0; output_index < output_size; output_index++)
    {
      int  input_output_offset = output_index * input_size;
      float element = output_gradients->samples[output_sample_offset + output_index] * layer->activation_function_prime(unactivated_output->samples[output_sample_offset + output_index]);
      float scaled_element = learning_rate * element;
      layer->biases[output_index] -= scaled_element;
      for (int input_index = 0; input_index < input_size; input_index++)
      {
        input_gradients->samples[input_sample_offset + input_index] += element * layer->weights[input_output_offset + input_index];
        layer->weights[input_output_offset + input_index] -= scaled_element * activated_input->samples[input_sample_offset + input_index];
      }
    }
  }
}

void backpropagateConvolutionalLayer
(
  float learning_rate, 
  convolutionalLayer * layer,
  dataset2D * input_gradients,
  dataset2D * output_gradients, 
  dataset2D * activated_input,
  dataset2D * unactivated_output
)
{
  int padding = layer->padding;
  int stride = layer->stride;

  int sample_count = activated_input->sample_count;

  int input_height = activated_input->row_count;
  int input_width = activated_input->column_count;
  int input_channel_count = activated_input->channel_count;

  int output_height = unactivated_output->row_count;
  int output_width = unactivated_output->column_count;
  int output_channel_count = unactivated_output->channel_count;

  int filter_size = layer->filter_size;

  int input_sample_index = input_height * input_width * input_channel_count;
  int input_height_index = input_width * input_channel_count;

  int output_sample_index = output_height * output_width * output_channel_count;
  int output_height_index = output_width * output_channel_count;

  int filter_height_index = filter_size * input_channel_count * output_channel_count;
  int filter_width_index = input_channel_count * output_channel_count;

  input_gradients->sample_count = sample_count;
  input_gradients->row_count = input_height;
  input_gradients->column_count = input_width;
  input_gradients->channel_count = input_channel_count;
  input_gradients->samples = calloc(sample_count * input_sample_index, sizeof(float));
  for (int sample_index = 0; sample_index < sample_count; sample_index++)
  {
    int input_index_base = sample_index * input_sample_index;
    int output_index_base = sample_index * output_sample_index;
    for (int input_row = 0; input_row < input_height; input_row++)
    {
      int input_index_row = input_index_base + input_row * input_height_index;
      int output_height_minimum = maximum(0, ceiling((input_row + padding - filter_size + 1)/stride));
      int output_height_maximum = minimum(output_height, floor((input_row + padding)/stride) + 1);
      for (int input_column = 0; input_column < input_width; input_column++)
      {
        int input_index_column = input_index_row + input_column * input_channel_count;
        int output_width_minimum = maximum(0, ceiling((input_width + padding - filter_size + 1)/stride));
        int output_width_maximum = minimum(output_width, floor((input_width + padding)/stride) + 1);
        for (int input_channel = 0; input_channel < input_channel_count; input_channel++)
        {
          int filter_index_base = input_channel * output_channel_count;
          int input_index = input_index_column + input_channel;
          for (int output_row = output_height_minimum; output_row < output_height_maximum; output_row++)
          {
            int filter_index_row = filter_index_base + (input_row + padding - output_row * stride) * filter_height_index;
            int output_index_row = output_index_base + output_row * output_height_index;
            for (int output_column = output_width_minimum; output_column < output_width_maximum; output_column++)
            {
              int filter_index_column = filter_index_row + (input_column + padding - output_column * stride) * filter_width_index;
              int output_index_column = output_index_row + output_column * output_channel_count;
              for (int output_channel = 0; output_channel < output_channel_count; output_channel++)
              {
                int filter_index = filter_index_column + output_channel;
                int output_index = output_index_column + output_channel;
                input_gradients->samples[input_index] += output_gradients->samples[output_index] 
                                                       * layer->activation_function_prime(unactivated_output->samples[output_index])
                                                       * layer->filters[filter_index];
              }
            }
          }
        }
      }
    }
  }
  for (int sample_index = 0; sample_index < sample_count; sample_index++)
  {
    int input_index_base = sample_index * input_sample_index;
    int output_index_base = sample_index * output_sample_index;
    for (int output_row = 0; output_row < output_height; output_row++)
    {
      int output_index_row = output_index_base + output_row * output_height_index;
      int filter_height_minimum = maximum(0, padding - output_row * stride);
      int filter_height_maximum = minimum(filter_size, output_height - padding - output_row * stride);
      for (int output_column = 0; output_column< output_width; output_column++)
      {
        int output_index_column = output_index_row + output_column * output_channel_count;
        int filter_width_minimum = maximum(0, padding - output_column * stride);
        int filter_width_maximum = minimum(filter_size, output_width - padding - output_column * stride);
        for (int output_channel = 0; output_channel < output_channel_count; output_channel++)
        {
          int output_index = output_index_column + output_channel;
          int filter_index_base = output_channel;
          float element = output_gradients->samples[output_index] * layer->activation_function_prime(unactivated_output->samples[output_index]);
          float scaled_element = learning_rate * element;
          layer->biases[output_channel] -= scaled_element;
          for (int filter_row = filter_height_minimum; filter_row < filter_height_maximum; filter_row++)
          {
            int input_index_row = input_index_base + (output_row * stride + filter_row - padding) * input_height_index;
            int filter_index_row = filter_index_base + filter_row * filter_height_index;
            for (int filter_column = filter_width_minimum; filter_column < filter_width_maximum; filter_column++)
            {
              int input_index_column = input_index_row + (output_column * stride + filter_column - padding) * input_channel_count;
              int filter_index_column = filter_index_row + filter_column * filter_width_index;
              for (int input_channel = 0; input_channel < input_channel_count; input_channel++)
              {
                int filter_index = filter_index_column + input_channel * output_channel;
                int input_index = input_index_column + input_channel;
                layer->filters[filter_index] -= scaled_element * activated_input->samples[input_index];
              }
            }
          }
        }
      }
    }
  }
}

void backpropagatePoolingLayer
(
  poolingLayer * layer,
  dataset2D * input_gradients,
  dataset2D * output_gradients, 
  dataset2D * activated_input,
  dataset2D * unactivated_output
)
{
  int padding = layer->padding;
  int padding_double = padding * 2;
  int stride = layer->stride;

  int sample_count = activated_input->sample_count;

  int input_height = activated_input->row_count;
  int input_width = activated_input->column_count;
  int input_channel_count = activated_input->channel_count;

  int output_height = unactivated_output->row_count;
  int output_width = unactivated_output->column_count;

  int filter_size = layer->filter_size;

  int input_sample_index = input_height * input_width * input_channel_count;
  int input_height_index = input_width * input_channel_count;

  int output_sample_index = output_height * output_width * input_channel_count;
  int output_height_index = output_width * input_channel_count;

  int padded_sample_index = (input_height + padding_double) * (input_width + padding_double) * input_channel_count;
  int padded_height_index = (input_width + padding_double) * input_channel_count;

  input_gradients->sample_count = sample_count;
  input_gradients->row_count = input_height;
  input_gradients->column_count = input_width;
  input_gradients->channel_count = input_channel_count;
  input_gradients->samples = calloc(sample_count * input_sample_index, sizeof(float));

  if (layer->is_max_pooling)
  {
    for (int sample_index = 0; sample_index < sample_count; sample_index++)
    {
      int input_index_base = sample_index * input_sample_index;
      int output_index_base = sample_index * output_sample_index;
      int padded_index_base = sample_index * padded_sample_index;
      for (int input_row = 0; input_row < input_height; input_row++)
      {
        int input_index_row = input_index_base + input_row * input_height_index;
        int padded_index_row = padded_index_base + (input_row + padding) * padded_height_index;
        int output_height_minimum = maximum(0, ceiling((input_row + padding - filter_size + 1)/stride));
        int output_height_maximum = minimum(output_height, floor((input_row + padding)/stride) + 1);
        for (int input_column = 0; input_column< input_width; input_column++)
        {
          int input_index_column = input_index_row + input_column * input_channel_count;
          int padded_index_column = padded_index_row + (input_column + padding) * input_channel_count;
          int output_width_minimum = maximum(0, ceiling((input_width + padding - filter_size + 1)/stride));
          int output_width_maximum = minimum(output_width, floor((input_width + padding)/stride) + 1);
          for (int output_row = output_height_minimum; output_row < output_height_maximum; output_row++)
          {
            int output_index_row = output_index_base + output_row * output_height_index;
            for (int output_column = output_width_minimum; output_column < output_width_maximum; output_column++)
            {
              int output_index_column = output_index_row + output_column * input_channel_count;
              for (int input_channel = 0; input_channel < input_channel_count; input_channel++)
              {
                int input_index = input_index_column + input_channel;
                int output_index = output_index_column + input_channel;
                int padded_index = padded_index_column + input_channel;
                if (unactivated_output->samples[output_index] == activated_input->samples[padded_index])
                {
                input_gradients->samples[input_index] += output_gradients->samples[output_index];
                }
              }
            }
          }
        }
      }
    }
  }
  else
  {
    int filter_size_squared = filter_size * filter_size;
    for (int sample_index = 0; sample_index < sample_count; sample_index++)
    {
      int input_index_base = sample_index * input_sample_index;
      int output_index_base = sample_index * output_sample_index;
      for (int input_row = 0; input_row < input_height; input_row++)
      {
        int input_index_row = input_index_base + input_row * input_height_index;
        int output_height_minimum = maximum(0, ceiling((input_row + padding - filter_size + 1)/stride));
        int output_height_maximum = minimum(output_height, floor((input_row + padding)/stride) + 1);
        for (int input_column = 0; input_column< input_width; input_column++)
        {
          int input_index_column = input_index_row + input_column * input_channel_count;
          int output_width_minimum = maximum(0, ceiling((input_width + padding - filter_size + 1)/stride));
          int output_width_maximum = minimum(output_width, floor((input_width + padding)/stride) + 1);
          for (int output_row = output_height_minimum; output_row < output_height_maximum; output_row++)
          {
            int output_index_row = output_index_base + output_row * output_height_index;
            for (int output_column = output_width_minimum; output_column < output_width_maximum; output_column++)
            {
              int output_index_column = output_index_row + output_column * input_channel_count;
              for (int input_channel = 0; input_channel < input_channel_count; input_channel++)
              {
                int input_index = input_index_column + input_channel;
                int output_index = output_index_column + input_channel;
                input_gradients->samples[input_index] += output_gradients->samples[output_index]/filter_size_squared;
              }
            }
          }
        }
      }
    }
  }
}

void unflatten (dataset1D * input, dataset2D * output, int row_count, int column_count, int channel_count)
{
  output->sample_count = input->sample_count;
  output->row_count = row_count;
  output->column_count = column_count;
  output->channel_count = channel_count;
  output->samples = input->samples;
}