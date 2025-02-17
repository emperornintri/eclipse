#include "mnist_dataset.h"

int swapBytes (int value)
{
  return
  (
    ((value >> 24) & 0x000000FF) |
    ((value >>  8) & 0x0000FF00) |
    ((value <<  8) & 0x00FF0000) |
    ((value << 24) & 0xFF000000)
  );
}

void readImages (const char * filename, dataset2D * dataset_images)
{
  long bytesRead;
  long file_descriptor;
  char magic_number_buffer[4];

  openFile (filename, & file_descriptor, 0, 0);
  readFile (4, magic_number_buffer, & file_descriptor, & bytesRead);

  char dimensions_buffer[12];
  readFile (12, dimensions_buffer, & file_descriptor, & bytesRead);

  dataset_images->sample_count = swapBytes (((int * ) dimensions_buffer)[0]);
  dataset_images->row_count = swapBytes (((int * ) dimensions_buffer)[1]);
  dataset_images->channel_count = 1;
  dataset_images->column_count = swapBytes (((int * ) dimensions_buffer)[2]);

  int data_size = dataset_images->sample_count * dataset_images->row_count * dataset_images->column_count;
  unsigned char * data_buffer = malloc (data_size);
  readFile (data_size, data_buffer, & file_descriptor, & bytesRead);
  closeFile (file_descriptor);

  dataset_images->samples = (float * ) malloc (sizeof(float) * data_size);
  for (int i = 0; i < data_size; i++)
  {
    dataset_images->samples[i] = (float) data_buffer[i];
  }
  free(data_buffer);
  return;
}

void readLabels (const char * filename, dataset1D * dataset_labels)
{
  long bytesRead;
  long file_descriptor;
  char magic_number_buffer[4];

  openFile (filename, & file_descriptor, 0, 0);
  readFile (4, magic_number_buffer, & file_descriptor, & bytesRead);

  char dimensions_buffer[4];
  readFile (4, dimensions_buffer, & file_descriptor, & bytesRead);
  dataset_labels->sample_count = swapBytes (((int * ) dimensions_buffer)[0]);
  dataset_labels->column_count = 10;
  unsigned char * data_buffer = malloc (dataset_labels->sample_count);

  readFile (dataset_labels->sample_count, data_buffer, & file_descriptor, & bytesRead);
  closeFile (file_descriptor);

  dataset_labels->samples = (float * ) calloc (dataset_labels->column_count * dataset_labels->sample_count, sizeof(float));
  for (int i = 0; i < dataset_labels->sample_count; i++)
  {
    dataset_labels->samples[i * dataset_labels->column_count + (int) data_buffer[i]] = 1;
  }
  free(data_buffer);
  return;
}

void batchLabels(int batch_size, int batch_index, dataset1D * dataset_labels, dataset1D * batched_labels)
{
  int size;
  if (dataset_labels->sample_count - batch_size * batch_index < batch_size)
  {
    size = dataset_labels->sample_count - batch_size * batch_index;
  }
  else
  {
    size = batch_size;
  }
  batched_labels->samples = dataset_labels->samples + batch_index * batch_size * dataset_labels->column_count;
  batched_labels->sample_count = size;
  batched_labels->column_count = dataset_labels->column_count;
}

void batchImages(int batch_size, int batch_index, dataset2D * dataset_images, dataset2D * batched_images)
{
  int size;
  if (dataset_images->sample_count - batch_size * batch_index < batch_size)
  {
    size = dataset_images->sample_count - batch_size * batch_index;
  }
  else
  {
    size = batch_size;
  }
  batched_images->samples = dataset_images->samples + batch_index * batch_size * dataset_images->row_count * dataset_images->column_count * dataset_images->channel_count;
  batched_images->sample_count = size;
  batched_images->row_count = dataset_images->row_count;
  batched_images->column_count = dataset_images->column_count;
  batched_images->channel_count = dataset_images->channel_count;
}

void resizeImages(dataset2D * dataset_images, int new_height, int new_width)
{
  int old_height = dataset_images->row_count;
  int old_width = dataset_images->column_count;

  float scale_x = (float) old_height / (float) new_height;
  float scale_y = (float) old_width / (float) new_width;

  int x_1, 
      x_2, 
      y_1, 
      y_2, 
      temporary_index_i,
      temporary_index_ij,
      temporary_index_ijn,
      temporary_index_ijnc,
      temporary_index_1_1_x,
      temporary_index_1_2_x,
      temporary_index_2_1_x,
      temporary_index_2_2_x,
      temporary_index_1_1_xy,
      temporary_index_1_2_xy,
      temporary_index_2_1_xy,
      temporary_index_2_2_xy,
      temporary_index_1_1_xyn,
      temporary_index_1_2_xyn,
      temporary_index_2_1_xyn,
      temporary_index_2_2_xyn,
      temporary_index_1_1_xync,
      temporary_index_1_2_xync,
      temporary_index_2_1_xync, 
      temporary_index_2_2_xync;

  float source_x, source_y;

  float * new_samples = malloc (dataset_images->sample_count * new_height * new_width * dataset_images->channel_count * sizeof(float));

  for (int i = 0; i < new_height; i++)
  {
    temporary_index_i = i * new_width * dataset_images->channel_count;

    source_x = (i + 0.5) * scale_x - 0.5;

    x_1 = (int) source_x;
    
    if (source_x == x_1)
    {
      x_2 = x_1;
    }
    else
    {
      x_2 = x_1 + 1;
    }

    x_1 = (x_1 > old_height - 1) ? (old_height - 1) : x_1;
    x_2 = (x_2 > old_height - 1) ? (old_height - 1) : x_2;

    temporary_index_1_1_x = x_1 * old_width * dataset_images->channel_count;
    temporary_index_1_2_x = x_1 * old_width * dataset_images->channel_count;
    temporary_index_2_1_x = x_2 * old_width * dataset_images->channel_count;
    temporary_index_2_2_x = x_2 * old_width * dataset_images->channel_count;

    for (int j = 0; j < new_width; j++)
    {
      temporary_index_ij = temporary_index_i + j * dataset_images->channel_count;
      
      source_y = (j + 0.5) * scale_y - 0.5;

      y_1 = (int) source_y;

      if (source_y == y_1)
      {
        y_2 = y_1;
      }
      else
      {
        y_2 = y_1 + 1;
      }

      y_1 = (y_1 > old_width - 1) ? (old_width - 1) : y_1;
      y_2 = (y_2 > old_width - 1) ? (old_width - 1) : y_2;

      temporary_index_1_1_xy = temporary_index_1_1_x + y_1 * dataset_images->channel_count;
      temporary_index_1_2_xy = temporary_index_1_2_x + y_2 * dataset_images->channel_count;
      temporary_index_2_1_xy = temporary_index_2_1_x + y_1 * dataset_images->channel_count;
      temporary_index_2_2_xy = temporary_index_2_2_x + y_2 * dataset_images->channel_count;

      if (x_1 == x_2)
      {
        if (y_1 == y_2)
        {
          for (int n = 0; n < dataset_images->sample_count; n++)
          {
            temporary_index_ijn = temporary_index_ij + n * new_height * new_width * dataset_images->channel_count;
            temporary_index_1_1_xyn = temporary_index_1_1_xy + n * old_height * old_width * dataset_images->channel_count;

            for (int c = 0; c < dataset_images->channel_count; c++)
            {
              temporary_index_ijnc = temporary_index_ijn + c;
              temporary_index_1_1_xync = temporary_index_1_1_xyn + c;

              new_samples[temporary_index_ijnc] = dataset_images->samples[temporary_index_1_1_xync];
            }
          }
        }
        else
        {
          for (int n = 0; n < dataset_images->sample_count; n++)
          {
            temporary_index_ijn = temporary_index_ij + n * new_height * new_width * dataset_images->channel_count;
            temporary_index_1_1_xyn = temporary_index_1_1_xy + n * old_height * old_width * dataset_images->channel_count;
            temporary_index_1_2_xyn = temporary_index_1_2_xy + n * old_height * old_width * dataset_images->channel_count;

            for (int c = 0; c < dataset_images->channel_count; c++)
            {
              temporary_index_ijnc = temporary_index_ijn + c;
              temporary_index_1_1_xync = temporary_index_1_1_xyn + c;
              temporary_index_1_2_xync = temporary_index_1_2_xyn + c;

              new_samples[temporary_index_ijnc] = (y_2 - source_y) * dataset_images->samples[temporary_index_1_1_xync]
                                                + (source_y - y_1) * dataset_images->samples[temporary_index_1_2_xync];
            }
          }
        }
      }
      else
      {
        if (y_1 == y_2)
        {
          for (int n = 0; n < dataset_images->sample_count; n++)
          {
            temporary_index_ijn = temporary_index_ij + n * new_height * new_width * dataset_images->channel_count;
            temporary_index_1_1_xyn = temporary_index_1_1_xy + n * old_height * old_width * dataset_images->channel_count;
            temporary_index_2_1_xyn = temporary_index_2_1_xy + n * old_height * old_width * dataset_images->channel_count;

            for (int c = 0; c < dataset_images->channel_count; c++)
            {
              temporary_index_ijnc = temporary_index_ijn + c;
              temporary_index_1_1_xync = temporary_index_1_1_xyn + c;
              temporary_index_2_1_xync = temporary_index_2_1_xyn + c;

              new_samples[temporary_index_ijnc] = (x_2 - source_x) * dataset_images->samples[temporary_index_1_1_xync]
                                                + (source_x - x_1) * dataset_images->samples[temporary_index_2_1_xync];
            }
          }
        }
        else
        {
          for (int n = 0; n < dataset_images->sample_count; n++)
          {
            temporary_index_ijn = temporary_index_ij + n * new_height * new_width * dataset_images->channel_count;
            temporary_index_1_1_xyn = temporary_index_1_1_xy + n * old_height * old_width * dataset_images->channel_count;
            temporary_index_1_2_xyn = temporary_index_1_2_xy + n * old_height * old_width * dataset_images->channel_count;
            temporary_index_2_1_xyn = temporary_index_2_1_xy + n * old_height * old_width * dataset_images->channel_count;
            temporary_index_2_2_xyn = temporary_index_2_2_xy + n * old_height * old_width * dataset_images->channel_count;

            for (int c = 0; c < dataset_images->channel_count; c++)
            {
              temporary_index_ijnc = temporary_index_ijn + c;
              temporary_index_1_1_xync = temporary_index_1_1_xyn + c;
              temporary_index_1_2_xync = temporary_index_1_2_xyn + c;
              temporary_index_2_1_xync = temporary_index_2_1_xyn + c;
              temporary_index_2_2_xync = temporary_index_2_2_xyn + c;

              new_samples[temporary_index_ijnc] = (y_2 - source_y) 
                                                * ((x_2 - source_x) * dataset_images->samples[temporary_index_1_1_xync] 
                                                + ((source_x - x_1) * dataset_images->samples[temporary_index_2_1_xync]))
                                                + (source_y - y_1)
                                                * ((x_2 - source_x) * dataset_images->samples[temporary_index_1_2_xync] 
                                                + ((source_x - x_1) * dataset_images->samples[temporary_index_2_2_xync]));
            }
          }
        }
      }
    }
  }
  free(dataset_images->samples);
  dataset_images->samples = new_samples;
  dataset_images->row_count = new_height;
  dataset_images->column_count = new_width;
}

void normalizeImages(dataset2D * dataset_images)
{
  for (int i = 0; i < dataset_images->sample_count * dataset_images->row_count * dataset_images->column_count * dataset_images->channel_count; i++)
  {
    dataset_images->samples[i] /= 255.0f;
  }
}

void denormalizeImages(dataset2D * dataset_images)
{
  for (int i = 0; i < dataset_images->sample_count * dataset_images->row_count * dataset_images->column_count * dataset_images->channel_count; i++)
  {
    dataset_images->samples[i] *= 255.0f;
  }
}