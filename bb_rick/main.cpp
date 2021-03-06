#include "header.h"
//#include "mainReceiver.cpp"
//#include "mainGPIO.cpp"

int main(int argc, char const *argv[])
{
    // Declarations for CommandsQueue
    condition_variable condVar1;
    CommandsQueue *cq = new CommandsQueue(&condVar1);

    // Declarations for timer
    struct sigaction action;
    timer_t timerId;
    struct sigevent clockSignalEvent;
    struct itimerspec timerValue;
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
    while (1)
    {
        // Waiting signal of condition_variable == nwe command in queue
        while (cq->getNumberOfCommands() == 0)
        {
            condVar1.wait(locker1);
        }

        cout << "[timer] Next command..." << endl;  //############################
        // Initialising
        Command comm = cq->getNext();
        tmpCycle.electrode1 = comm.electrode1;
        tmpCycle.offset = comm.offset;
        tmpCycle.qtyElectrodes = comm.qtyElectrodes;
        tmpCycle.qtyCycles = comm.qtyCycles;
        
        cout << "[timer] Lets go (" << tmpCycle.electrode1 << " + " << tmpCycle.offset << ") * " << tmpCycle.qtyElectrodes << endl;  //############################

        // Setting timer interval values 
        memset(&timerValue, 0, sizeof(struct itimerspec));
        //timerValue.it_interval.tv_sec = 0;
        timerValue.it_interval.tv_nsec = comm.durationCycle*1000;
        

        // Setting timer initial delay
        //timerValue.it_value.tv_sec = 0;
        timerValue.it_value.tv_nsec = cq->processNext()*1000;

        // Creating timer 
        returnValue = timer_settime(timerId, 0, &timerValue, NULL);
        assert(returnValue == 0);

        while (tmpCycle.qtyCycles > 0)
        {
            this_thread::sleep_for(chrono::seconds(1));
        }

        // Reset timer 
        memset(&timerValue, 0, sizeof(struct itimerspec));
        returnValue = timer_settime(timerId, 0, &timerValue, NULL);
        assert(returnValue == 0);   
    }

    //first.join();                // pauses until first finishes
    return 0;
}
