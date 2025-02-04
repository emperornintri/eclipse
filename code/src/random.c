#include "random.h"

unsigned long getTime()
{
    unsigned long cycles;
    asm volatile("rdtsc" : "=A"(cycles));
    return cycles;
}

unsigned long splitmix64(struct splitmix64_state * state)
{
	unsigned long result = (state->r += 0x9E3779B97f4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}

void xorshift64Initialization(struct xorshift64_state * state)
{
	struct splitmix64_state splitmix_state = {getTime()};
	unsigned long temporary = splitmix64(& splitmix_state);
	state->r = temporary;
}

unsigned long xorshift64(struct xorshift64_state * state)
{
	unsigned long x = state->r;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return state->r = x;
}