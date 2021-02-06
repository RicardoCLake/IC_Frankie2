#ifndef GPIO_H
#define GPIO_H

//#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>

#include "gpio_map.h"

class Gpio
{
    public:
        Gpio(volatile unsigned int offset);
        ~Gpio();
        //int with 32 bits
        void setOE (unsigned int ports); //each 1 is out
        void setDataOut (unsigned int ports);
        void clearDataOut (unsigned int ports);
        static bool gpioMap (); //init mapping to gpio_addr (necessary to start using first object)
        static volatile void* getGpioAddr();
    
    private:
        volatile unsigned int *gpioOeAddr;
        volatile unsigned int *gpioSetdataoutAddr;
        volatile unsigned int *gpioCleardataoutAddr;
        static volatile void *gpioAddr;
};


#endif //GPIO_H
