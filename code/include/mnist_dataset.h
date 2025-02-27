#ifndef MNIST_DATASET_H
#define MNIST_DATASET_H

#include "file.h"
#include "mathematics.h"
#include "memory.h"
#include "print.h"
#include "structures.h"

void readImages (const char * filename, dataset2D * dataset_images);
void readLabels (const char * filename, dataset1D * dataset_labels);
void batchImages(int batch_size, int batch_index, dataset2D * dataset_images, dataset2D * batched_images);
void batchLabels(int batch_size, int batch_index, dataset1D * dataset_labels, dataset1D * batched_labels);
void resizeImages(dataset2D * dataset_images, int new_height, int new_width);
void normalizeImages(dataset2D * dataset_images);
void denormalizeImages(dataset2D * dataset_images);

#endif
