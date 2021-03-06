#include "header.h"

void initGPIO()
{
    // Initial preparation for create gpio*
    if (!Gpio::gpioMap())
    {
        return;
    }

    // Creating gpio*
    gpio1 = new Gpio(GPIO1_OFFSET);
    gpio2 = new Gpio(GPIO2_OFFSET);
    gpio3 = new Gpio(GPIO3_OFFSET);
    cout << "[I_gpio] Gpios created" << endl;
    
    // Defining as output
    gpio1->setOE(0b00110000000011111111000000000000);
    gpio2->setOE(0b00000011110000000000000000111110);
    gpio3->setOE(0b00001111000000000000000000000000);

    gpio2->setDataOut(1 << 1); // PIC enable
}

void changeGPIO(int sig)
{
    if (tmpCycle.qtyCycles < 0)
        return;
    
    // Cuts current (a1, a2, b1, b2)
    gpio1->clearDataOut(0b00110000000011000000000000000000);   

    // Turns on pic h               
    //gpio1->setDataOut(1 << 17);                    

    // Clears all data pins
    gpio1->clearDataOut(0b00000000000000011111000000000000);
    gpio2->clearDataOut(0b00000011110000000000000000111100);
    gpio3->clearDataOut(0b00001111000000000000000000000000);

    // Sets values of In1 and In2
    gpio2->setDataOut(tmpCycle.electrode1 % 16 << 22);  //M In1
    gpio3->setDataOut((tmpCycle.electrode1 + tmpCycle.offset) % tmpCycle.qtyElectrodes % 16 << 14); //M In2

    // Digital operation
    gpio1->setDataOut(tmpCycle.electrode1 << 12);   //pic D
    gpio2->setDataOut(tmpCycle.offset << 2);        //pic P
    //gpio1->clearDataOut(1 << 17);                   //turns off pic h   
    
    // Turns on electrode1
    if (tmpCycle.electrode1 / 16)                             
    {   
        gpio1->setDataOut(1 << 28);  //B1
        gpio1->clearDataOut(1 << 18);
    }
    else
    {
        gpio1->setDataOut(1 << 18);  //A1
        gpio1->clearDataOut(1 << 28);
    }

    // Turns on electrode2
    if (((tmpCycle.electrode1 + tmpCycle.offset) % tmpCycle.qtyElectrodes) / 16)       
    {
        gpio1->setDataOut(1 << 29);  //B2
        gpio1->clearDataOut(1 << 19);
    }
    else
    {
        gpio1->setDataOut(1 << 19);  //A2
        gpio1->clearDataOut(1 << 29);
    }

    // Final iterations
    tmpCycle.electrode1 = (tmpCycle.electrode1 + 1) % tmpCycle.qtyElectrodes;
    tmpCycle.qtyCycles--;
        
}

