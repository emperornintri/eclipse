#include "exit_program.h"
#include "print.h"

extern int testingPrint ();
extern int testingRandom ();
extern int testingNormal ();
extern int testingNormalBis ();
extern int testingMNISTData ();
extern int testingConvolution ();
extern int testingForwardBackward ();
extern int testingTrain ();

void startProgram ()
{
  // testingPrint (); 
  // testingRandom ();
  // testingNormal ();
  // testingNormalBis ();
  // testingMNISTData ();
  // testingConvolution ();
  // testingForwardBackward ();
  // testingTrain ();

  int status = testingTrain ();
  exitProgram (0);
}