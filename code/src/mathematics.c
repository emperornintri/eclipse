#include "mathematics.h"

double absoluteValue (double x)
{
  return x > 0 ? x : -x;
}

double initialGuess (double x)
{
  union
  {
    double value;
    long bits;
  }
  guess;
  guess.bits = 0x0000000000000000;
  long x_bits = * ((long * ) & x);
  long biased_exponent = x_bits >> 52;
  double mantissa = x_bits & 0x000FFFFFFFFFFFFF;
  if (biased_exponent != 0)
  {
    if (biased_exponent << 63 != 0)
    {
      int rest = (biased_exponent - 1) >> 1;
      long new_biased_exponent = rest + 512;
      long temporary_result = ((long) mantissa) >> 1;
      double new_mantissa = * ((double * ) & temporary_result);
      guess.bits |= * ((long * ) & new_mantissa);
      guess.bits |= (new_biased_exponent << 52);
      return guess.value;
    }
    else
    {
      int rest = biased_exponent >> 1;
      long new_biased_exponent = rest + 511;
      long temporary_result = (((long ) mantissa) >> 1) | 0x0008000000000000;
      double new_mantissa = * ((double * ) & temporary_result);
      guess.bits |= * ((long * ) & new_mantissa);
      guess.bits |= (new_biased_exponent << 52);
      return guess.value;
    }
  }
  else
  {
    int l = countLeadingZeros ( * ((long * ) & mantissa)) - 11;
    if (l << 31 != 0)
    {
      int rest = (l - 1) >> 1;
      long new_biased_exponent = 512 - rest;
      long temporary_result = (((long ) mantissa) << (52 + l)) >> (52 + 2 * l - 1);
      double new_mantissa = * ((double * ) & temporary_result);
      guess.bits |= * ((long * ) & new_mantissa);
      guess.bits |= (new_biased_exponent << 52);
      return guess.value;
    }
    else
    {
      int rest = l >> 1;
      long new_biased_exponent = 511 - rest;
      long temporary_result = ((long) & mantissa) << l - 1;
      double new_mantissa = * ((double * ) & temporary_result);
      guess.bits |= * ((long * ) & new_mantissa);
      guess.bits |= (new_biased_exponent << 52);
      return guess.value;
    }
  }
    return guess.value;
}

double squareRoot (double x, double epsilon)
{
  int limit = 0;
  long x_bits = ( * (long * ) & x);
  if (x < 0)
  {
    return -1;
  }
  else if (x == 0)
  {
    return 0;
  }
  else if (x_bits & 0x7FF0000000000000 == 0x7FF0000000000000)
  {
    return x;
  }
  double x_guess = initialGuess(x);
  while (limit != 10)
  {
    double x_next = 0.5 * (x_guess + x / x_guess);
    if (absoluteValue ((x_next / x_guess) - 1) < epsilon) {
        return x_next;
    }
    x_guess = x_next;
    limit++;
  }
  return x_guess;
}