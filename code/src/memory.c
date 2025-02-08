#include "memory.h"

static Block * free_list = 0;

Block * request_memory (unsigned long int size)
{
  unsigned long int total_size = sizeof (Block) + size;
  if (total_size < MIN_ALLOC_SIZE)
  {
    total_size = MIN_ALLOC_SIZE;
  }
  void * ptr;
  asm volatile
  (
    "movq      $9,     %%rax;"
    "movq      $0,     %%rdi;"
    "movq %[size],     %%rsi;"
    "movq      $3,     %%rdx;"
    "movq     $34,     %%r10;"
    "movq     $-1,      %%r8;"
    "movq      $0,      %%r9;"
    "syscall;"
    "movq   %%rax, %[result];"
    : [result] "=r" (ptr)
    : [size]    "r" (total_size)
    : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"
  );
  if (ptr == (void * ) -1)
  {
    print("Oh noes, the memory goes brrrr...");
    return 0;
  }
  Block * new_block = (Block * ) ptr;
  new_block->size = total_size - sizeof (Block);
  new_block->is_free = 1;
  new_block->next = 0;
  return new_block;
}

void initialize_memory ()
{
  if (!free_list)
  {
    free_list = request_memory (MIN_ALLOC_SIZE);
  }
}

void * malloc (unsigned long int size)
{
  if (size == 0)
  {
    return 0;
  }
  initialize_memory ();
  Block * current = free_list;
  Block * previous = 0;
  while (current) {
    if (current->is_free && current->size >= size)
    {
      if (current->size > size + sizeof (Block))
      {
        Block * new_block = (Block * ) ((char * ) current + sizeof (Block) + size);
        new_block->size = current->size - size - sizeof (Block);
        new_block->is_free = 1;
        new_block->next = current->next;
        current->next = new_block;
        current->size = size;
      }
      current->is_free = 0;
      return (char * ) current + sizeof (Block);
    }
  previous = current;
  current = current->next;
  }
  Block * new_block = request_memory (size);
  if (!new_block)
  {
    print ("Oh noes, the memory goes brrrr...");
    return 0;
  }
  if (previous)
  {
    previous->next = new_block;
  }
  new_block->is_free = 0;
  return (char * ) new_block + sizeof (Block);
}

void free (void * ptr)
{
  if (!ptr)
  {
    return;
  }
  Block * block = (Block * ) ((char * ) ptr - sizeof (Block));
  block->is_free = 1;
  Block * current = free_list;
  while (current) 
  {
    if (current->is_free && current->next && current->next->is_free && (char * ) current + sizeof (Block) + current->size == (char * ) current->next)
    {
      current->size += current->next->size + sizeof (Block);
      current->next = current->next->next;
    }
    else
    {
      current = current->next;
    }
  }
}

void * calloc (unsigned long int num, unsigned long int size)
{
  unsigned long int total_size = num * size;
  char * ptr = (char * ) malloc (total_size);
  if (!ptr)
  {
    print("Oh noes, the memory goes brrrr...");
    return 0;
  }
  unsigned int i;
  for (i = 0; i + 8 <= total_size; i += 8)
  {
    * ((unsigned long int * ) (ptr + i)) = 0;  
  }
  for (; i < total_size; ++i) {
    ptr[i] = 0;  
  }
  return ptr;
}