#include "training.h"

void training(float learning_rate, int batch_size, int epochs, LeNet5 * network, dataset2D * X_train, dataset1D * y_train)
{
  dataset1D labels;

  dataset2D input_2d_1;
  dataset2D input_2d_2;
  dataset2D input_2d_3;
  dataset2D input_2d_4;
  dataset2D input_2d_5;
  dataset1D input_1d_1;
  dataset1D input_1d_2;
  dataset1D input_1d_3;
  dataset1D output;

  dataset2D input_2d_1_gradients; 
  dataset2D input_2d_2_gradients; 
  dataset2D input_2d_3_gradients; 
  dataset2D input_2d_4_gradients; 
  dataset2D input_2d_5_gradients;
  dataset1D input_1d_1_gradients; 
  dataset1D input_1d_2_gradients; 
  dataset1D input_1d_3_gradients; 
  dataset1D output_gradients;

  int number_of_batches = X_train->sample_count / batch_size;
  if (X_train->sample_count % batch_size > 0)
  {
    number_of_batches++;
  }

  int count_correct;
  int count_correct_global;
  float loss;
  float loss_global;

  for (int epoch = 0; epoch < epochs; epoch++)
  {
    count_correct_global = 0;
    loss_global = 0;
    int stop = number_of_batches;
    for (int batch = 0; batch < stop; batch++)
    {
      count_correct = 0;
      loss = 0;

      batchImages (batch_size, batch, X_train, & input_2d_1);
      batchLabels (batch_size, batch, y_train, & labels);
      forwardLeNet5 (network, & input_2d_1, & input_2d_2, & input_2d_3, & input_2d_4, & input_2d_5, & input_1d_1, & input_1d_2, & input_1d_3, & output);
      backwardLeNet5
      (
        learning_rate,
        network, 
        & labels,
        & input_2d_1, 
        & input_2d_2, 
        & input_2d_3, 
        & input_2d_4, 
        & input_2d_5,
        & input_1d_1, 
        & input_1d_2, 
        & input_1d_3, 
        & output,
        & input_2d_1_gradients, 
        & input_2d_2_gradients, 
        & input_2d_3_gradients, 
        & input_2d_4_gradients, 
        & input_2d_5_gradients,
        & input_1d_1_gradients, 
        & input_1d_2_gradients, 
        & input_1d_3_gradients, 
        & output_gradients
      );

      count_correct = countCorrect(& output, & labels);
      count_correct_global += count_correct;
      loss = crossEntropyLoss(& output, & labels);
      loss_global += loss;

      if (batch%100 == 0)
      {
        print("Batch ");
        printUnsignedInteger(batch);
        print(" - Loss: ");
        printDouble(loss / (float) input_2d_1.sample_count);
        print(" Accuracy: ");
        printDouble(100 * (double) count_correct / (double) input_2d_1.sample_count);
        print("%.\n");
      }
      /*
      print("Predicted Labels:\n[");
      for (int i = 0; i < 1; i++)
      {
        print("[");
        for (int j = 0; j < output.column_count - 1; j++)
        {
          printDouble(output.samples[i * output.column_count + j]);
          print(", ");
        }
        printDouble(output.samples[i * output.column_count + output.column_count - 1]);
        print("]");
      }

      float probabilities[10];
      print("]\n");
      softmax (10, output.samples, probabilities);
      print("Softmax:\n[");
      for (int i = 0; i < 1; i++)
      {
        print("[");
        for (int j = 0; j < output.column_count - 1; j++)
        {
          printDouble(probabilities[j]);
          print(", ");
        }
        printDouble(probabilities[output.column_count - 1]);
        print("]");
      }
      print("]\n");*/
    }
    print("\nEpoch ");
    printUnsignedInteger(epoch);
    print(" - Loss: ");
    printDouble(loss_global / (float) (stop * batch_size));
    print(" Accuracy: ");
    printDouble(100 * (double) count_correct_global / (double) (stop * batch_size));
    print("%.\n");
  }
}