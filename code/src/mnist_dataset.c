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
  dataset_labels->column_count = 1;
  unsigned char * data_buffer = malloc (dataset_labels->sample_count);

  readFile (dataset_labels->sample_count, data_buffer, & file_descriptor, & bytesRead);
  closeFile (file_descriptor);

  dataset_labels->samples = (float * ) malloc (sizeof(float) * dataset_labels->sample_count);
  for (int i = 0; i < dataset_labels->sample_count; i++)
  {
    dataset_labels->samples[i] = (float) data_buffer[i];
  }
  free(data_buffer);
  return;
}


void displayImage (dataset2D * images, int image)
{
  int current_pixel;
  int coordinate = image * images->row_count * images->column_count;
  for (int row = 0; row < images->row_count; row++)
  {
    for (int column = 0; column < images->column_count; column++)
    {
      current_pixel = images->samples[row * 28 + column + coordinate];
      if (current_pixel < 10)
      {
        print("  ");
        printUnsignedInteger(current_pixel);
        print(" ");
      }
      else if ((current_pixel >= 10) && (current_pixel < 100))
      {
        print(" ");
        printUnsignedInteger(current_pixel);
        print(" ");       
      }
      else
      {
        printUnsignedInteger(current_pixel);
        print(" ");   
      }
    }
    print("\n");
  }
}