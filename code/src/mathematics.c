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

unsigned long truncateInteger (double x)
{
  union
  {
    double value;
    long bits;
  }
  x_union;
  x_union.value = x;
  unsigned int biased_exponent = (x_union.bits & 0x7FF0000000000000) >> 52;
  if (biased_exponent < 1023)
  {
    return 0;
  }
  else
  {
    int exponent = biased_exponent - 1023;
    if (exponent >= 52)
    {
      x_union.bits = (x_union.bits & 0x000FFFFFFFFFFFFF) + 0x0010000000000000 << exponent - 52;
      return x_union.bits;
    }
    else
    {
      x_union.bits = (x_union.bits & 0x000FFFFFFFFFFFFF) + 0x0010000000000000 >> 52 - exponent;
      return x_union.bits;
    }
  }
}

double truncateFractional (double x)
{
  union
  {
    double value;
    unsigned long bits;
  }
  x_union;
  x_union.value = x;
  long biased_exponent = (x_union.bits & 0x7FF0000000000000) >> 52;
  if (biased_exponent >= 1075)
  {
    return 0;
  }
  else
  {
    long exponent = biased_exponent - 1023;
    if (exponent <= - 1)
    {
      return x_union.value;
    }
    else
    {
      x_union.bits = ((x_union.bits & 0x000FFFFFFFFFFFFF) << (12 + exponent + 1)) >> 12;
      if (x_union.bits == 0)
      {
        return 0.0f;
      }
      x_union.bits = x_union.bits + (1022L << 52);
      if (x < 0)
      {
        return - x_union.value;
      }
      else
      {
        return x_union.value;
      }
    }
  }
}

long roundDouble (double x)
{
  union
  {
    double value;
    long bits;
  }
  x_union;
  x_union.value = x;
  unsigned int biased_exponent = (x_union.bits & 0x7FF0000000000000) >> 52;
  if (biased_exponent < 1023)
  {
    if (biased_exponent < 1022)
    {
      return 0;
    }
    else
    {
      if (x >= 0)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }
  else
  {
    int exponent = biased_exponent - 1023;
    if (exponent >= 52)
    {
      x_union.bits = (x_union.bits & 0x000FFFFFFFFFFFFF) + 0x0010000000000000 << exponent - 52;
      if (x < 0)
      {
        return - x_union.bits;
      }
      else
      {
        return x_union.bits;
      }
    }
    else
    {
      int carry = 0;
      if ((((x_union.bits & 0x000FFFFFFFFFFFFF) + 0x0010000000000000 >> (52 - exponent - 1)) << 63) != 0)
      {
        if (x < 0 && ((((x_union.bits & 0x000FFFFFFFFFFFFF) + 0x0010000000000000) << 63 - (52 - exponent - 1)) == 0x8000000000000000))
        {
          carry = 0;
        }
        else
        {
          carry = 1;
        }
      }
      x_union.bits = (x_union.bits & 0x000FFFFFFFFFFFFF) + 0x0010000000000000 >> 52 - exponent;
      if (x < 0)
      {
        return - x_union.bits - carry ;
      }
      else
      {
        return x_union.bits + carry;
      }
    }
  }
}