#include "header.h"
//#include "mainReceiver.cpp"
//#include "mainGPIO.cpp"

Cycle tmpCycle;
void initGPIO();
void changeGPIO(int sig);

int main(int argc, char const *argv[])
{
    struct sigaction action;
    timer_t timerID;
    struct sigevent clockSigEvent;
    struct itimerspec timer_value;
    int returnValue;

    condition_variable condVar1;
    CommandsQueue *cq = new CommandsQueue(&condVar1);
    
    // Register new action for SIGUSR1 
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler =  changeGPIO;
    returnValue = sigaction(SIGUSR1, &action, NULL);
    assert(returnValue == 0);
    
    // Creating process interval timer
    memset(&clockSigEvent, 0, sizeof(struct sigevent));
    clockSigEvent.sigev_notify = SIGEV_SIGNAL;
    clockSigEvent.sigev_signo = SIGUSR1;
    clockSigEvent.sigev_notify_attributes = NULL;
    returnValue = timer_create(CLOCK_MONOTONIC, &clockSigEvent, &timerID);
    assert(returnValue == 0);
    
    cout << "[M_main] Starting Receiver Thread..." << endl;

    // Starting receiver threads
    thread first (mainReceiver, cq);

    initGPIO();

    //creating locker used to wait signal of condition_variable
    mutex mu;
    unique_lock<mutex> locker1(mu);

    //sleep(2);            //#####################
    //Gpio* gpio1 = new Gpio(GPIO1_OFFSET); //#########################

    cout << "[main] Main OK" << endl;
    while (1)
    {
        // Waiting signal of condition_variable
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
        
        //doing cycles at the defined time
        this_thread::sleep_for(chrono::microseconds(cq->processNext()));
        cout << "[timer] Lets go (" << tmpCycle.electrode1 << " + " << tmpCycle.offset << ") * " << tmpCycle.qtyElectrodes << endl;  //############################
        
        /* setitng timer interval values */
        memset(&timer_value, 0, sizeof(struct itimerspec));
        timer_value.it_interval.tv_sec = 0;
        timer_value.it_interval.tv_nsec = comm.durationCycle*1000;

        long expiration = comm.qtyCycles * comm.durationCycle;
        
        /* setting timer initial expiration values*/
        timer_value.it_value.tv_sec = expiration / 1000000000;;
        timer_value.it_value.tv_nsec = expiration % 1000000000;

        /* Create timer */
        returnValue = timer_settime(timerID, 0, &timer_value, NULL);
        assert(returnValue == 0);
        
    }

    //first.join();                // pauses until first finishes
    //second.join();               // pauses until second finishes
    return 0;
}

