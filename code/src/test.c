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
}