#include "exit_program.h"

extern int testingPrint ();
extern int testingRandom ();
extern int testingNormal ();
extern int testingMNISTData ();

void startProgram ()
{
  //int status_1 = testingPrint (); 
  //int status_2 = testingRandom ();
  int status_3 = testingNormal ();
  int status_4 = testingMNISTData ();
  exitProgram (status_3 * status_4);
}