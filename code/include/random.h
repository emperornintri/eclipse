#ifndef RANDOM_H
#define RANDOM_H

struct splitmix64_state {
	unsigned long r;
};

struct xorshift64_state {
    unsigned long r;
};

void xorshift64Initialization(struct xorshift64_state * state);
unsigned long xorshift64(struct xorshift64_state * state);

#endif