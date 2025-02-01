#include "print.h"

void print (const char * string) 
{
  size_type string_length = getStringLength(string);
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
    char string[2];
    string[0] = '0';
    string[1] = '\0';
    print(string);
    return;
  }
  else if (integer == 0x8000000000000000)
  {
    print("-9223372036854775808");
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
  print(string);
  return;
}

void printUnsignedInteger(unsigned long integer)
{
  long copy = integer;
  int number_of_digits = 0;
  if (integer == 0)
  {
    char string[2];
    string[0] = '0';
    string[1] = '\0';
    print(string);
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
  print(string);
  return;
}