#include "header.h"

// Pulsing in 44    gpio_1 - 12
#define PIN 1 << 12

void changeGPIO(int sig)
{
    if(isHigh)
    {
        gpio1->clearDataOut(PIN);
    } 
    else
    {
        gpio1->setDataOut(PIN);
    }
    isHigh = !isHigh;
}

int main(int argc, char const *argv[])
{
    // Duration register
    int cycleDuration = atoi(argv[1]);

    // Declarations for timer
    struct sigaction action;
    timer_t timerId;
    clock_t clockId;
    struct sigevent clockSignalEvent;
    struct itimerspec timerValue;
    int returnValue;

    // Registering new action for SIGUSR1 
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler =  changeGPIO;
    returnValue = sigaction(SIGUSR1, &action, NULL);
    assert(returnValue == 0);
    cout << "[Timer] Sigaction registered!" << endl;
    
    // Creating process interval timer 
    clockId = CLOCK_MONOTONIC;
    memset(&clockSignalEvent, 0, sizeof( struct sigevent));
    clockSignalEvent.sigev_notify = SIGEV_SIGNAL;
    clockSignalEvent.sigev_signo = SIGUSR1;
    clockSignalEvent.sigev_notify_attributes = NULL;
    returnValue = timer_create(clockId, &clockSignalEvent, &timerId);
    assert(returnValue == 0);
    cout << "[Timer] Timer created!" << endl;

    // Creating Gpio1
    if (!Gpio::gpioMap())
    {
        return -1;
    }
    gpio1 = new Gpio(GPIO1_OFFSET);
    cout << "[M_gpio] Gpio1 created" << endl;
    gpio1->setOE(PIN);
    
    // Setting timer interval values 
    memset(&timerValue, 0, sizeof(struct itimerspec));
    timerValue.it_interval.tv_sec = 0;
    timerValue.it_interval.tv_nsec = cycleDuration*1000;

    // Setting timer initial delay
    timerValue.it_value.tv_sec = 0;
    timerValue.it_value.tv_nsec = cycleDuration*1000; 

    // Starting timer 
    returnValue = timer_settime(timerId, 0, &timerValue, NULL);
    assert(returnValue == 0);
    cout << "[Control] Starting Cycles of " << argv[1] << " usecs..."<< endl;

    while(1)
    {
        sleep(100);
    }

    return 0;
}