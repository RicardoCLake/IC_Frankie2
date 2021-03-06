#include "header.h"
//#include "mainReceiver.cpp"
//#include "mainGPIO.cpp"

Cycle tmpCycle;
void initGPIO();
void changeGPIO(int sig);

int main(int argc, char const *argv[])
{
    struct sigaction act;
    clockid_t clock_id;
    timer_t timer_id;
    struct sigevent clock_sig_event;
    struct itimerspec timer_value;
    int ret;

    condition_variable condVar1;
    CommandsQueue *cq = new CommandsQueue(&condVar1);
    
    /* Register new action for SIGUSR1 */
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler =  changeGPIO;
    ret = sigaction(SIGUSR1, &act, NULL);
    assert(ret == 0);
    
    clock_id = CLOCK_MONOTONIC;
    memset(&clock_sig_event, 0, sizeof( struct sigevent));
    clock_sig_event.sigev_notify = SIGEV_SIGNAL;
    clock_sig_event.sigev_signo = SIGUSR1;
    clock_sig_event.sigev_notify_attributes = NULL;
    /* Creating process interval timer */
    ret = timer_create(clock_id, &clock_sig_event, &timer_id);
    assert(ret == 0);
    
    cout << "[M_main] Starting Threads..." << endl;
    //starting threads
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
        ret = timer_settime(timer_id, 0, &timer_value, NULL);
        assert(ret == 0);

        while (tmpCycle.qtyCycles > 0)
            sleep(1);

        /* Reset timer */
        memset(&timer_value, 0, sizeof(struct itimerspec));
        ret = timer_settime(timer_id, 0, &timer_value, NULL);
        assert(ret == 0);
        
        
    }

    //first.join();                // pauses until first finishes
    //second.join();               // pauses until second finishes
    return 0;
}




/*

void action(int trash) 
{   
    timeval time1;  //######################
    timeval time2;  //######################
    gettimeofday(&time1, NULL);   //######################
    Command comm = cq->getNext();  //######################
    procedure(comm.electrode1);
    cq->processNext();
    gettimeofday(&time2, NULL);   //######################
    //cout << "\tHorario inicial: "<< time1.tv_sec << "." << time1.tv_usec << endl;   //######################
    //out << "\tHorario final: "<< time1.tv_sec << "." << time1.tv_usec << endl;   //######################
    //cout << "\tHorario marcado: "<< comm.time.tv_sec << "." << comm.time.tv_usec << endl; //################
    cout << "\tErro inicial: " << 1000000*(comm.time.tv_sec - time1.tv_sec) + comm.time.tv_usec - time1.tv_usec << endl; //######################
    cout << "\tDelay meu: " << 1000000*(time2.tv_sec - time1.tv_sec) + time2.tv_usec - time1.tv_usec << endl; //######################
}

void action(int trash) 
{   
    procedure(cq->getNext().electrode1);
    cq->processNext();
}

void procedure (int electrode1)
{
    cout << "Ativado o eletrodo " << electrode1 << endl;
}

void procedure (int electrode1)
{   
    //cout << "Procedure para eletrodo " << electrode1 << endl;
    Gpio* gp = new Gpio(GPIO3_OFFSET);
    //cout << "Criado o GPIO3" << endl;   //###########################
    gp->setOE(0b00000000000000111100000000000000);
    //cout << "Setadas as entradas e saidas" << endl;      //#####################
    gp->clearDataOut(0b00000000000000111100000000000000);
    //cout << "Saídas apagadas" << endl;  //###########################
    gp->setDataOut(1 << (13 + electrode1) );
    //cout << "Botou para sair" << endl;   //####################
    delete gp;
}

*/
