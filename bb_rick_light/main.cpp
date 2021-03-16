#define MAIN_CPP
#include "header.h"

int main(int argc, char const *argv[])
{
    // Declarations for CommandsQueue
    CommandsQueue *cq = new CommandsQueue();

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
    
    // Creating process interval timer 
    clockId = CLOCK_MONOTONIC;
    memset(&clockSignalEvent, 0, sizeof( struct sigevent));
    clockSignalEvent.sigev_notify = SIGEV_SIGNAL;
    clockSignalEvent.sigev_signo = SIGUSR1;
    clockSignalEvent.sigev_notify_attributes = NULL;
    returnValue = timer_create(clockId, &clockSignalEvent, &timerId);
    assert(returnValue == 0); 

    initReceiver();
    initGPIO();

    cout << "[main] Main OK" << endl;
    while (1)
    {
        cout << "[timer] Next command..." << endl;  //############################
        receive(cq, atoi (argv[1]));
        this_thread::sleep_for(chrono::microseconds(cq->processNext())); //**********************

        // Initialising
        Command comm = cq->getFront();
        tmpCycle.electrode1 = comm.electrode1;
        tmpCycle.offset = comm.offset;
        tmpCycle.qtyElectrodes = comm.qtyElectrodes;
        tmpCycle.qtyCycles = comm.qtyCycles;

        // Setting timer interval values 
        memset(&timerValue, 0, sizeof(struct itimerspec));
        timerValue.it_interval.tv_sec = 0;
        timerValue.it_interval.tv_nsec = comm.durationCycle*1000;

        // Setting timer initial delay
        timerValue.it_value.tv_sec = 0;
        timerValue.it_value.tv_nsec = comm.durationCycle*1000; //*****************************

        // Creating timer 
        returnValue = timer_settime(timerId, 0, &timerValue, NULL);
        assert(returnValue == 0);
        cout << "[timer] Lets go (" << tmpCycle.electrode1 << " + " << tmpCycle.offset << ") * " << tmpCycle.qtyElectrodes << endl;  //############################

        while (tmpCycle.qtyCycles > 0)
        {
            this_thread::sleep_for(chrono::seconds(1));
        }

        // Reset timer 
        memset(&timerValue, 0, sizeof(struct itimerspec));
        returnValue = timer_settime(timerId, 0, &timerValue, NULL);
        assert(returnValue == 0);   
    }

    return 0;
}
