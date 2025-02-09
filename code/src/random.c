#include "random.h"

unsigned long getTime ()
{
    unsigned long cycles;
    asm volatile ("rdtsc" : "=A"(cycles));
    return cycles;
}

unsigned long splitmix64 (struct splitmix64_state * state)
{
	unsigned long result = (state->r += 0x9E3779B97f4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}

void xorshift64Initialization (struct xorshift64_state * state)
{
	struct splitmix64_state splitmix_state = {getTime()};
	unsigned long temporary = splitmix64(& splitmix_state);
	state->r = temporary;
}

unsigned long xorshift64 (struct xorshift64_state * state)
{
	unsigned long x = state->r;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return state->r = x;
}

double normal(struct xorshift64_state * state)
{
	double v_1 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
	double v_2 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
	double w = v_1 * v_1 + v_2 * v_2;
	while (w >= 1)
	{
		v_1 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
		v_2 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
		w = v_1 * v_1 + v_2 * v_2;
	}
	return v_1 * squareRoot(-2 * naturalLogarithm(w) / w, 1e-15);
}

void normalDoubleEfficient(struct xorshift64_state * state, double * result, unsigned int n)
{
	for (int index = 0; index < n; index++)
	{
		double v_1 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
		double v_2 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
		double w = v_1 * v_1 + v_2 * v_2;
		while (w >= 1)
		{
			v_1 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
			v_2 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
			w = v_1 * v_1 + v_2 * v_2;
		}
		result[index * 2 + 0] = v_1 * squareRoot(- 2 * naturalLogarithm(w) / w, 1e-15);
		result[index * 2 + 1] = v_2 * squareRoot(- 2 * naturalLogarithm(w) / w, 1e-15);
	}
}

void normalFloatEfficient(struct xorshift64_state * state, float * result, unsigned int n, double mean, double standard_deviation)
{
	for (int index = 0; index < n; index++)
	{
		double v_1 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
		double v_2 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
		double w = v_1 * v_1 + v_2 * v_2;
		while (w >= 1)
		{
			v_1 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
			v_2 = 2 * (((double) xorshift64(state)) / ((double) (0xFFFFFFFFFFFFFFFF))) - 1;
			w = v_1 * v_1 + v_2 * v_2;
		}
		result[index * 2 + 0] = (float) (mean + standard_deviation * (v_1 * squareRoot(- 2 * naturalLogarithm(w) / w, 1e-15)));
		result[index * 2 + 1] = (float) (mean + standard_deviation * (v_2 * squareRoot(- 2 * naturalLogarithm(w) / w, 1e-15)));
	}
}