#include "mathematics.h"

const double logarithm_2_high = 6.93147180369123816490e-01;    // 0x3FE62E42FEE00000
const double logarithm_2_low = 1.90821492927058770002e-10;     // 0x3DEA39EF35793C76
const double logarithm_2_inverse = 1.44269504088896338700e+00; // 0x3FF71547652B82FE
const int T1[32] = 
{
	    0,	1024,	 3062,	5746,	 9193, 13348,	18162, 23592,
	29598, 36145,	43202, 50740,	58733, 67158,	75992, 85215,
	83599, 71378,	60428, 50647,	41945, 34246,	27478, 21581,
	16499, 12183,	 8588,	5674,	 3403,	1742,	  661,	 130
};
const double exponential_polynomial_coefficients[5] = 
{
  + 1.66666666666666657415e-01,                                // 0x3FC5555555555555         
  - 2.77777777777777875115e-03,                                // 0xBF66C16C16C16C19
  + 6.61375661375661555788e-05,                                // 0x3F11566ABC011568
  - 1.65343915343915139072e-06,                                // 0xBEBBBD779334EF01
  + 4.17535139757357239171e-08                                 // 0x3E666A8F2BF70E76
};
const double logarithm_polynomial_coefficients[7] = 
{ 
  + 6.666666666666735130e-01,
  + 3.999999999940941908e-01,
  + 2.857142874366239149e-01,
  + 2.222219843214978396e-01,
  + 1.818357216161805012e-01,
  + 1.531383769920937332e-01,
  + 1.479819860511658591e-01,
};

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

double multiplyDoubleByPowerOf2 (double x, int n) 
{
  union 
  {
    double x;
    unsigned long long bits;
  } 
  result;
  result.x = x; 
  unsigned int exponent = (result.bits >> 52) & 0x7FF;
  exponent += n;
  if ((int) exponent < 1)
  {
    result.bits = ((result.bits & 0x000FFFFFFFFFFFFF) + 0x0010000000000000) >> (1 - (int) exponent);
    if (x < 0)
    {
      result.bits = result.bits | 0x8000000000000000;
    }
  }
  else
  {
    result.bits = (result.bits & 0x800FFFFFFFFFFFFF) | ((unsigned long long) exponent << 52);
  }
  x = result.x;
  return x;
}

float exponential (float x) 
{
    if (x >= (256 - 1 - 127) * (logarithm_2_high))
    {
      return 1.0 / 0.0;
    }
    int n;
    double exponential_component_r, exponential_r, high, low, r, r2;
    double x_double = (double) x;
    n = (int) (x_double * logarithm_2_inverse + (x_double < 0 ? - 0.5 : 0.5));
    high = x_double - n * logarithm_2_high;
    low = n * logarithm_2_low;
    r = high - low;
    r2 = r * r;
    exponential_component_r = r - r2 * (exponential_polynomial_coefficients[0] + 
                                  r2 * (exponential_polynomial_coefficients[1] + 
                                  r2 * (exponential_polynomial_coefficients[2] + 
                                  r2 * (exponential_polynomial_coefficients[3] + 
                                  r2 * (exponential_polynomial_coefficients[4])))));
    exponential_r = 1 - (low - (r * exponential_component_r) / (2.0 - exponential_component_r) - high);
    return (float) multiplyDoubleByPowerOf2 (exponential_r, n);
}

float naturalLogarithm (float x)
{
  if (x <= 0)
  {
    return - 1.0 / 0.0;
  }
  int i, k, x_high;
  double k_double, f, half_f_square, s, R, t1, t2, w, z;
  double x_double = (double) x;  
  k = 0;
  x_high = * (1 + (int * ) & x_double);
  k += (x_high >> 20) - 1023;
  x_high &= 0x000fffff;
  i = (x_high + 0x95f64) & 0x100000;
	* (1 + (int * ) & x_double) = x_high | (i ^ 0x3ff00000);
	k += (i >> 20);
	f = x_double - 1.0;
 	s = f / (2.0 + f); 
	k_double = (double) k;
	z = s * s;
	w = z * z;
	t1 = w * (logarithm_polynomial_coefficients[1] + 
       w * (logarithm_polynomial_coefficients[3] + 
       w * logarithm_polynomial_coefficients[5])); 
	t2 = z * (logarithm_polynomial_coefficients[0] + 
       w * (logarithm_polynomial_coefficients[2] + 
       w * (logarithm_polynomial_coefficients[4] + 
       w * logarithm_polynomial_coefficients[6]))); 
	R = t2 + t1;
  half_f_square = 0.5 * f * f;
  return k_double * logarithm_2_high - ((half_f_square - (s * (half_f_square + R) + k_double * logarithm_2_low)) - f);
}

float rectifiedLinearUnit (float x)
{
  return (x > 0) ? x : 0;
}

float identity (float x)
{
  return x;
}

float maximum (float x, float y)
{
  return (x > y) ? x : y;
}

float minimum (float x, float y)
{
  return (x < y) ? x : y;
}

void softmax (int size, float * logits, float * probabilities)
{

  float max_logit, sum;
  sum = 1e-8f;
  max_logit = logits[0];
  for (int logits_index = 1; logits_index < size; logits_index++) 
  {
    if (logits[logits_index] > max_logit)
    {
      max_logit = logits[logits_index];
    }
  }
  for (int logits_index = 0; logits_index < size; logits_index++)
  {
    probabilities[logits_index] = exponential(logits[logits_index] - max_logit);
    sum += probabilities[logits_index];
  }
  for (int logits_index = 0; logits_index < size; logits_index++)
  {
    probabilities[logits_index] /= sum;
  }
}