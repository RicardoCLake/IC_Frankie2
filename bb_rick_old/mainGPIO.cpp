#include "header.h"

void mainGPIO (Cycle* tmpCycle, condition_variable* condVar2, bool* noMoreCycles)
{
    if (!Gpio::gpioMap())
    {
        return;
    }

    Gpio* gpio1 = new Gpio(GPIO1_OFFSET);
    Gpio* gpio2 = new Gpio(GPIO2_OFFSET);
    Gpio* gpio3 = new Gpio(GPIO3_OFFSET);

    cout << "[M_gpio] Gpios created" << endl;

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
    
    gpio1->setOE(0b00110000000011111111000000000000);
    gpio2->setOE(0b00000011110000000000000000111110);
    gpio3->setOE(0b00001111000000000000000000000000);

    gpio2->setDataOut(1 << 1); // PIC enable

    mutex mut;
    unique_lock<mutex> locker2(mut);

    cout << "[M_gpio] Starting Cycles..." << endl;
    while (1)
    {
        // Waits signal to execute
        while (*noMoreCycles)
        {
            (*condVar2).wait(locker2);
        }
        //(*condVar2).wait(locker2); //#######################
        *noMoreCycles = true; //************
        
        //cout << "[cycle] +" << endl; //#########################
        
        // Cuts current (a1, a2, b1, b2)
        gpio1->clearDataOut(0b00110000000011000000000000000000);   

        //Turns on pic h          //????????????????????? is it really necessary?*
        gpio1->setDataOut(1 << 17);     //**********************************                 

        // Clears all data pins
        gpio1->clearDataOut(0b00000000000000011111000000000000);
        gpio2->clearDataOut(0b00000011110000000000000000111100);
        gpio3->clearDataOut(0b00001111000000000000000000000000);

        // Sets values of In1 and In2
        gpio2->setDataOut((*tmpCycle).electrode1 % 16 << 22);  //M In1
        gpio3->setDataOut(((*tmpCycle).electrode1 + (*tmpCycle).offset) % (*tmpCycle).qtyElectrodes % 16 << 14); //M In2

        // Digital operation
        gpio1->setDataOut((*tmpCycle).electrode1 << 12);   //pic D
        gpio2->setDataOut((*tmpCycle).offset << 2);        //pic P
        gpio1->clearDataOut(1 << 17);                   //turns off pic h   //????????????????????? *(the same)
        
        // Turns on electrode1
        if ((*tmpCycle).electrode1 / 16)                             
        {   
            gpio1->setDataOut(1 << 28);  //B1
        }
        else
        {
            gpio1->setDataOut(1 << 18);  //A1
        }

        // Turns on electrode2
        if ((((*tmpCycle).electrode1 + (*tmpCycle).offset) % (*tmpCycle).qtyElectrodes) / 16)       
        {
            gpio1->setDataOut(1 << 29);  //B2
        }
        else
        {
            gpio1->setDataOut(1 << 19);  //A2
        }
    }
}