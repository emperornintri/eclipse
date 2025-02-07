#include "file.h"

void openFile (const char * filename, long * file_descriptor, long flags, long mode)
{
  asm volatile
  (
    "movq    $2, %%rax\n"
    "movq    %1, %%rdi\n"
    "movq    %2, %%rsi\n"
    "movq    %3, %%rdx\n"
    "syscall\n"
    "movq %%rax,    %0\n"
    : "=r"( * file_descriptor)
    : "m"(filename), "r"(flags), "r"(mode)
    : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "memory"
  );
  if (* file_descriptor < 0)
  {
    asm volatile
    (
      "mov $60, %%rax\n"
      "mov  $1, %%rdi\n"
      "syscall\n"
      :
      :
      : "rax", "rdi"
    );
  }
}

void readFile (long buffer_size, char * buffer, long * file_descriptor, long * bytesRead)
{
  asm volatile
  (
    "mov    $0, %%rax\n"
    "mov    %1, %%rdi\n"
    "mov    %2, %%rsi\n"
    "mov    %3, %%rdx\n"
    "syscall\n"
    "mov %%rax,    %0\n"
    : "=r"(* bytesRead)
    : "r"(* file_descriptor), "r"(buffer), "r"(buffer_size)
    : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "memory"
  );
}

void closeFile (long file_descriptor)
{
  asm volatile
  (
    "mov $3, %%rax\n"
    "mov %0, %%rdi\n"
    "syscall\n"
    :
    : "r"(file_descriptor)
    : "rax", "rdi", "rcx", "r11"
  );
}