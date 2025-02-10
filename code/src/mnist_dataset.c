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