#include "exit_program.h"

extern int testingPrint ();
extern int testingRandom ();
extern int testingMNISTData ();

void startProgram ()
{
  //int status_1 = testingPrint (); 
  //int status_2 = testingRandom ();
  int status_3 = testingMNISTData ();
  exitProgram (status_3);
}