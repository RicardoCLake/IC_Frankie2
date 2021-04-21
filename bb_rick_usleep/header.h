#ifndef _HEADER_H
#define _HEADER_H

//********************************  INCLUDES  ********************************************
#include <iostream>
//#include <time.h>
#include <unistd.h>
#include <thread> 
#include <chrono>
#include <stdexcept>
#include "CommandsQueue.h"
#include "Gpio.h"

//*******************************  DEFINITIONS  *******************************************
#define GPIO1_BITS 0b00000000000011111111000011111111
#define GPIO2_BITS 0b00000000000000011111111111111110
struct Cycle
{
    int electrode1;
    int offset;
    int qtyElectrodes;
};
using namespace std;

//*******************************  PROTOTYPES  ********************************************
void mainReceiver (CommandsQueue* cq, int duration);
void mainGPIO (Cycle* tmpCycle, condition_variable* condVar2, bool* noMoreCycles);
//void mainPrinter ();

#endif //_HEADER_H