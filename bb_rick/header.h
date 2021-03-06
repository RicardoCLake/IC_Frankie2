#ifndef _HEADER_H
#define _HEADER_H

//********************************  INCLUDES  ********************************************
#include <iostream>
//#include <time.h>
#include <unistd.h>
#include <thread> 
#include <chrono>
#include <stdexcept>
#include <assert.h>
#include <memory.h>
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
    long qtyCycles;
};
using namespace std;

//*******************************  PROTOTYPES  ********************************************
void mainReceiver (CommandsQueue* cq);
//void mainPrinter ();

#endif //_HEADER_H