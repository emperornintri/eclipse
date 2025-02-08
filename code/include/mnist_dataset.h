#ifndef MNIST_DATASET_H
#define MNIST_DATASET_H

#include "file.h"
#include "memory.h"
#include "print.h"
#include "structures.h"

void readImages (const char * filename, dataset2D * dataset_images);
void readLabels (const char * filename, dataset1D * dataset_labels);
void displayImage (dataset2D * images, int image);

#endif
