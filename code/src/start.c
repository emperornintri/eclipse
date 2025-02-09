#include "exit_program.h"

extern int testingPrint ();
extern int testingRandom ();
extern int testingNormal ();
extern int testingMNISTData ();
extern int testingConvolution();
extern int testingForward();

void startProgram ()
{
  //int status_1 = testingPrint (); 
  //int status_2 = testingRandom ();
  //int status_3 = testingNormal ();
  //int status_4 = testingMNISTData ();
  //int status_5 = testingConvolution();
  int status_6 = testingForward();
  exitProgram (status_6);
}