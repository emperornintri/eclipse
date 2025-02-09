#ifndef EXIT_PROGRAM_H
#define EXIT_PROGRAM_H

#include "string.h"

extern const double logarithm_2_high;
extern const double logarithm_2_low;
extern const double logarithm_2_inverse;
extern const int T1[32];
extern const double exponential_polynomial_coefficients[5];
extern const double logarithm_polynomial_coefficients[7];

double absoluteValue (double x);
double initialGuess (double x);
double squareRoot (double x, double epsilon);
unsigned long truncateInteger(double x);
double truncateFractional (double x);
long roundDouble (double x);
double multiplyDoubleByPowerOf2 (double x, int n);
float exponential (float x);
float naturalLogarithm (float x);
float maximum (float x, float y);
float minimum (float x, float y);
float rectifiedLinearUnit (float x);
float rectifiedLinearUnitPrime (float x);
float identity (float x);
float identityPrime (float x);
void softmax (int size, float * logits, float * predictions);
#endif