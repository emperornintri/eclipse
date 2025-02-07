#ifndef MNIST_DATASET_H
#define MNIST_DATASET_H

#include "file.h"
#include "memory.h"
#include "print.h"

struct dataset
{
  float * samples;
  int row_count;
  int column_count;
  int channel_count;
  int sample_count;
};

void readImages (const char * filename, struct dataset * dataset_images);
void displayImage (struct dataset * images, int image);

#endif
