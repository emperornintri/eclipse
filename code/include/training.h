#ifndef TRAINING_H
#define TRAINING_H

#include "evaluate.h"
#include "mathematics.h"
#include "mnist_dataset.h"
#include "network.h"
#include "structures.h"

void training(float learning_rate, int batch_size, int epochs, LeNet5 * network, dataset2D * X_train, dataset1D * y_train);

#endif