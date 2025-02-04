#include "exit_program.h"

extern int testingPrint ();
extern int testingRandom ();

void startProgram ()
{
  int status_1 = testingPrint (); 
  int status_2 = testingRandom ();  
  exitProgram (status_1 * status_2);
}