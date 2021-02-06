#include "header.h"

int main(int argc, char const *argv[])
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
        M In2 120-123               gpio_3 - 24-27
        M A1  50                    gpio_1 - 18
        M A2  51                    gpio_1 - 19
        M B1  60                    gpio_1 - 28
        M B2  61                    gpio_1 - 29  
    */

   
    
    return 0;
}


