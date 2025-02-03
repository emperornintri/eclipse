#include "print.h"

void print (const char * string) 
{
  size_type string_length = getStringLength (string);
  asm volatile
  (
    "movq $1, %%rax\n"
    "movq $1, %%rdi\n"
    "movq %0, %%rsi\n"
    "movq %1, %%rdx\n"
    "syscall       \n"
    :
    : "r"(string), "r"(string_length)
    : "rax", "rdi", "rsi", "rdx"
  );
  return;
}

void printInteger (long integer)
{
  long copy = integer;
  int is_negative = 0;
  int number_of_digits = 0;
  if (integer == 0)
  {
    print ("0");
    return;
  }
  else if (integer == 0x8000000000000000)
  {
    print ("-9223372036854775808");
    return;
  }
  else if (integer < 0)
  {
    is_negative = 1;
    integer = -integer;
  }
  while (copy != 0)
  {
    copy /= 10;
    number_of_digits++;
  }
  char string[number_of_digits + 2];
  string[number_of_digits + 1] = '\0';
  int index = number_of_digits;
  while (integer != 0)
  {
    string[index--] = (integer % 10) + '0';
    integer /= 10;
  }
  if (is_negative)
  {
    string[0] = '-';
  }
  else
  {
    string[0] = '+';
  }
  print (string);
  return;
}

void printUnsignedInteger(unsigned long integer)
{
  unsigned long copy = integer;
  int number_of_digits = 0;
  if (integer == 0)
  {
    print ("0");
    return;
  }
  while (copy != 0)
  {
    copy /= 10;
    number_of_digits++;
  }
  char string[number_of_digits + 2];
  string[number_of_digits + 1] = '\0';
  int index = number_of_digits;
  while (integer != 0)
  {
    string[index--] = (integer % 10) + '0';
    integer /= 10;
  }
  string[0] = '+';
  print (string);
  return;
}

void printDouble(double x)
{
  int is_fractional = 0;
  int is_negative = 0;
  int number_of_digits = 0;
  union
  {
    double value;
    unsigned long bits;
  }
  x_union;
  x_union.value = x;
  if (x == 0)
  {
    print ("0");
    return;
  }
  else if ((x_union.bits & 0x7FF0000000000000) == 0x7FF0000000000000)
  {
    if ((x_union.bits & 0x000FFFFFFFFFFFFF) == 0)
    {
      if ((x_union.bits & 0x8000000000000000) == 0)
      {
        print ("+Infinity");
        return;
      }
      else
      {
        print ("-Infinity");
        return;
      }
    }
    else
    {
      print ("NaN");
      return;
    }
  }
  else if (x < 0)
  {
    x = -x;
    is_negative = 1;
  }
  if ((x_union.bits & 0x7FF0000000000000) >> 52 == 1087)
  {
    print("OOB");
    return;
  }
  unsigned long integer_part = truncateInteger (x);
  unsigned long copy = integer_part;
  unsigned long fractional_part = roundDouble(100000 * truncateFractional (x));
  while (copy != 0)
  {
    copy /= 10;
    number_of_digits++;
  }
  if (number_of_digits == 0)
  {
    is_fractional = 1;
    number_of_digits++;
  }
  char string[number_of_digits + 2 + 6];
  if (is_fractional)
  {
    string[1] = '0';
  }
  string[number_of_digits + 7] = '\0';
  int index = number_of_digits;
  while (integer_part != 0)
  {
    string[index--] = (integer_part % 10) + '0';
    integer_part /= 10;
  }
  string[number_of_digits + 1] = '.';
  index = number_of_digits + 6;
  while (index != number_of_digits + 1)
  {
    string[index--] = (fractional_part % 10) + '0';
    fractional_part /= 10;
  }
  if (is_negative)
  {
    string[0] = '-';
  }
  else
  {
    string[0] = '+';
  }
  print (string);
  return;
}