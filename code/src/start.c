#include "exit_program.h"

extern int main ();

void startProgram ()
{
  int status = main ();  
  exitProgram (status);
}