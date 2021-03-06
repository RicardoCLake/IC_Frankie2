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
/*
        Pic D 44-48     Electrode1  gpio_1 - 12-16
        Pic P 66-69     Offset      gpio_2 - 02-05
        Pic h 49                    gpio_1 - 17
        Pic E 65                    gpio_2 - 1
        M In1 86-89                 gpio_2 - 22-25
        M In2 110-113               gpio_3 - 14-17
        M A1  50                    gpio_1 - 18
        M A2  51                    gpio_1 - 19
        M B1  60                    gpio_1 - 28
        M B2  61                    gpio_1 - 29  
*/

//*******************************  PROTOTYPES  ********************************************
void mainReceiver (CommandsQueue* cq);
//void mainPrinter ();

#endif //_HEADER_H