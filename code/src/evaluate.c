#include "evaluate.h"

int countCorrect (dataset1D * predictions, dataset1D * true_labels)
{
  int count_correct = 0;
  for (int sample = 0; sample < predictions->sample_count; sample++)
  {
    int y_predicted = argmax (predictions->samples + sample * predictions->column_count, predictions->column_count);
    if (true_labels->samples[sample * true_labels->column_count + y_predicted] == 1)
    {
      count_correct++;
    }
  }
  return count_correct;
}

float crossEntropyLoss (dataset1D * predictions, dataset1D * true_labels)
{
  float loss = 0;
  for (int sample = 0; sample < true_labels->sample_count; sample++)
  {
    float probabilities[predictions->column_count];
    softmax (predictions->column_count, predictions->samples + sample * predictions->column_count, probabilities);
    for (int label = 0; label < true_labels->column_count; label++)
    {
      if (true_labels->samples[sample * true_labels->column_count + label] == 1)
      {
        loss += - naturalLogarithm(probabilities[label]);
        break;
      }
    }
  }
  return loss;
}