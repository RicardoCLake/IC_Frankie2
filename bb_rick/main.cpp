#include "header.h"
//#include "mainReceiver.cpp"
//#include "mainGPIO.cpp"

Cycle tmpCycle;
void initGPIO();
void changeGPIO(int sig);

int main(int argc, char const *argv[])
{
    // Declarations for CommandsQueue
    condition_variable condVar1;
    CommandsQueue *cq = new CommandsQueue(&condVar1);

    // Declarations for timer
    struct sigaction action;
    timer_t timerId;
    struct sigevent clockSignalEvent;
    struct itimerspec timer_value;
    int returnValue;

    // Registering new action for SIGUSR1 
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler =  changeGPIO;
    returnValue = sigaction(SIGUSR1, &action, NULL);
    assert(returnValue == 0);
    
    // Creating process interval timer 
    memset(&clockSignalEvent, 0, sizeof( struct sigevent));
    clockSignalEvent.sigev_notify = SIGEV_SIGNAL;
    clockSignalEvent.sigev_signo = SIGUSR1;
    clockSignalEvent.sigev_notify_attributes = NULL;
    returnValue = timer_create(CLOCK_MONOTONIC, &clockSignalEvent, &timerId);
    assert(returnValue == 0);
    
    
    // Starting receiver thread
    cout << "[M_main] Starting Receiver Thread..." << endl;
    thread first (mainReceiver, cq); 

    initGPIO();

    // Creating locker used to wait signal of condition_variable
    mutex mu;
    unique_lock<mutex> locker1(mu);

    cout << "[main] Main OK" << endl;
    cout << "[M_gpio] Starting Cycles..." << endl;
    while (1)
    {
        //waiting signal of condition_variable
        while (cq->getNumberOfCommands() == 0)
        {
            condVar1.wait(locker1);
        }

        cout << "[timer] Next command..." << endl;  //############################
        //initialising
        Command comm = cq->getNext();
        tmpCycle.electrode1 = comm.electrode1;
        tmpCycle.offset = comm.offset;
        tmpCycle.qtyElectrodes = comm.qtyElectrodes;
        
        //doing cycles at the defined time
        this_thread::sleep_for(chrono::microseconds(cq->processNext()));
        cout << "[timer] Lets go (" << tmpCycle.electrode1 << " + " << tmpCycle.offset << ") * " << tmpCycle.qtyElectrodes << endl;  //############################
        
        /* setitng timer interval values */
        memset(&timer_value, 0, sizeof(struct itimerspec));
        timer_value.it_interval.tv_sec = 0;
        timer_value.it_interval.tv_nsec = comm.durationCycle*1000;

        /* setting timer initial expiration values*/
        timer_value.it_value.tv_sec = 0;
        timer_value.it_value.tv_nsec = comm.durationCycle*1000;

        tmpCycle.qtyCycles = comm.qtyCycles;

        /* Create timer */
        returnValue = timer_settime(timerId, 0, &timer_value, NULL);
        assert(returnValue == 0);

        while (tmpCycle.qtyCycles > 0)
            sleep(1);

        /* Reset timer */
        memset(&timer_value, 0, sizeof(struct itimerspec));
        returnValue = timer_settime(timerId, 0, &timer_value, NULL);
        assert(returnValue == 0);
        
        
    }

    //first.join();                // pauses until first finishes
    //second.join();               // pauses until second finishes
    return 0;
}
