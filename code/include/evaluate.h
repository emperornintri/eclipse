#ifndef EVALUATE_H
#define EVALUATE_H

#include "mathematics.h"
#include "structures.h"

int countCorrect (dataset1D * predictions, dataset1D * true_labels);
float crossEntropyLoss (dataset1D * predictions, dataset1D * true_labels);

#endif