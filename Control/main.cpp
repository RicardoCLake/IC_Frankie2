#include "header.h"

int main(int argc, char const *argv[])
{
    // Creating Gpio1
    Gpio* gpio1 = new Gpio(GPIO1_OFFSET);
    cout << "[M_gpio] Gpio1 created" << endl;

    // Pulsing in 44    gpio_1 - 12
    gpio1->setOE(1 << 12);
    while (1)
    {
        gpio1->setDataOut(1 << 12);
        this_thread::sleep_for(chrono::microseconds(atoi(argv[1])));
        gpio1->clearDataOut(1 << 12);
        this_thread::sleep_for(chrono::microseconds(atoi(argv[1])));
    }
    
    return 0;
}