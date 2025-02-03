#include "mathematics.h"
#include "print.h"

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

  printDouble (0.541);
  print ("\n"); 

  printDouble (45002.950);
  print ("\n"); 

  printDouble (0.000005);
  print ("\n"); 

  printDouble (0.123456789);
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