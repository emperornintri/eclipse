#include "mnist_dataset.h"
#include "print.h"
#include "random.h"

int testingPrint ()
{
  printUnsignedInteger (truncateInteger(452.750));
  print ("\n");

  printUnsignedInteger (truncateFractional(45002.950));
  print ("\n");

  printUnsignedInteger (truncateInteger(1000 * truncateFractional(45002.950)));
  print ("\n");

  printUnsignedInteger (roundDouble(1000 * truncateFractional(45002.950)));
  print ("\n");

  printUnsignedInteger (roundDouble(0.4));
  print ("\n");

  printDouble (48.541);
  print ("\n"); 

  printDouble (-0.541);
  print ("\n"); 

  printDouble (-45002.950);
  print ("\n");

  printDouble ((100*truncateFractional(-45002.950)));
  print ("\n"); 

  printInteger(roundDouble(100000 * truncateFractional(-100*truncateFractional(45002.950))));
  print ("\n"); 
  
  printDouble (0.000005);
  print ("\n"); 

  printDouble (0.123456789);
  print ("\n"); 
  
  printDouble (- 0.123455);
  print ("\n"); 

  printDouble (roundDouble(- 0.5));
  print ("\n"); 
  
  union
  {
    double value;
    unsigned long bits;
  }
  x_union;
  x_union.bits = 0x43F0000000000000;
  printDouble(x_union.value);
  print("\n");

  x_union.bits = 0x7FF0000000000000;
  printDouble(x_union.value);
  print("\n");

  x_union.bits = 0x7FF0000000000001;
  printDouble(x_union.value);
  print("\n");

  x_union.bits = 0xFFF0000000000000;
  printDouble(x_union.value);
  print("\n");

  x_union.bits = 0xFFF0000000000001;
  printDouble(x_union.value);
  print("\n");

  print("This is a final test showcasing the print.\n");
  
  return 0;
}

int testingRandom()
{
  struct xorshift64_state state;
  xorshift64Initialization(& state);
  int bin = 100;
  int result[bin];
  for (int i = 0; i < bin; i++) {
        result[i] = 0;
  }
  int index;
  int n = 1000000;
  double value;
  for (int i = 0; i < n; i++)
  {
    value = ((float) xorshift64(& state)) / ((float) 0xFFFFFFFFFFFFFFFF);
    index = (int) (bin * value);
    result[index]++;
  }
  for (int i = 0; i < bin; i++)
  {
    printInteger(result[i]);
    print("\n");
  }
  for (int i = 0; i < 10; i++)
  {
    printDouble(normal(& state));
    print("\n");
  }
  return 0;
}

int testingNormal()
{
  struct xorshift64_state state;
  xorshift64Initialization(& state);
  int sample_number = 10000;
  int star_number = 100;
  int distribution[10];
  for (int i = 0; i < 10; i++)
  {
    distribution[i] = 0;
  }
  for (int i = 0; i < sample_number; i++)
  {
    double number = 5.0 + 2.0 * normal(& state);
    if ((number >= 0.0) && (number < 10.0))
    {
      distribution[(int) number]++;
    }
  }
  for (int i = 0; i < 10; i++)
  {
    printUnsignedInteger(i);
    print(" to");
    if (i < 9)
    {
      print(" ");
    }
    print(" ");
    printUnsignedInteger(i + 1);
    print(": ");
    for (int j = 0; j < distribution[i] * star_number / sample_number; j++)
    {
      print("*");
    }
    print("\n");
  }
}

int testingMNISTData()
{
  struct dataset2D X_train;
  struct dataset1D y_train;
  readImages ("/code/data/train-images-idx3-ubyte", & X_train);
  readLabels ("/code/data/train-labels-idx1-ubyte", & y_train);
  print("The image is:\n");
  displayImage (& X_train, 0);
  print("The label is:\n");
  printUnsignedInteger (y_train.samples[0]);
  print("The image is:\n");
  displayImage (& X_train, 59999);
  print("The label is:\n");
  printUnsignedInteger (y_train.samples[59999]);
  print("\n");
  free(X_train.samples);
  free(y_train.samples);
}