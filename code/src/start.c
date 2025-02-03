#include "exit_program.h"

extern int testingPrint ();

void startProgram ()
{
  int status = testingPrint ();  
  exitProgram (status);
}