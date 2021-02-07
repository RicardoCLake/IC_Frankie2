#include "header.h"

int main(int argc, char const *argv[])
{
    if (!Gpio::gpioMap())
    {
        return;
    }

    Gpio* gpio1 = new Gpio(GPIO1_OFFSET);
    //Gpio* gpio2 = new Gpio(GPIO2_OFFSET);
    //Gpio* gpio3 = new Gpio(GPIO3_OFFSET);

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

    struct timeval vector0[10], vector1[10];
    struct timeval time0, time1;
    int i = 0;
    while (i < TIMES)
    {
        if (!gpio1->readDataIn(17)) {
            gettimeofday(&time0, NULL);
            vector0[i] = time0;
            while (gpio1->readDataIn(17)) usleep(1);
            gettimeofday(&time1, NULL);
            vector1[i] = time1;
            i++;
        }
    }

    cout << "Intervalos:" << endl;
    unsigned long sumInterval = 0;
    unsigned long interval = 0;
    for (int j = 0; j < TIMES - 1; j++)
    {
        interval = 1000000 * vector0[j + 1].tv_sec + vector0[j + 1].tv_usec - 1000000 * vector0[j].tv_sec + vector0[j].tv_usec;
        sumInterval += interval;
        cout << interval << endl;
    }
    cout << "A media dos intervalos foi " << sumInterval / (TIMES - 1) << endl;

    cout << "Duracoes:" << endl;
    unsigned long sumDuration = 0;
    unsigned long duration = 0;
    for (int j = 0; j < TIMES; j++)
    {
        duration = 1000000 * vector1[j].tv_sec + vector1[j].tv_usec - 1000000 * vector0[j].tv_sec + vector0[j].tv_usec;
        sumDuration += duration;
        cout << duration << endl;
    }
    cout << "A media das duracoes foi " << sumDuration / (TIMES) << endl;
    
    return 0;
}


