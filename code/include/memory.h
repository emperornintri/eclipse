#ifndef MEMORY_H
#define MEMORY_H

#define MIN_ALLOC_SIZE 4096

#include "print.h"

typedef struct Block
{
    unsigned int size;
    unsigned int is_free;
    struct Block *next;
}
Block;

Block * request_memory (unsigned long int size);
void initialize_memory (void);
void * malloc (unsigned long int size);
void free (void * ptr);
void * calloc (unsigned long int num, unsigned long int size);

#endif
