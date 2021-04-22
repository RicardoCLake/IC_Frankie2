#ifndef _HEADER_H
#define _HEADER_H

//********************************  INCLUDES  ********************************************
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <thread> 
#include <chrono>
#include <stdexcept>
#include <assert.h>
#include <memory.h>
#include <signal.h>

#include "Gpio.h"

//*******************************  DEFINITIONS  *******************************************
using namespace std;

//***************************  GLOBAL DECLARATIONS :( ************************************
Gpio* gpio1;
bool isHigh;
    



#endif //_HEADER_H