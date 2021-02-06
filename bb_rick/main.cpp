#include "header.h"
//#include "mainReceiver.cpp"
//#include "mainGPIO.cpp"

int main(int argc, char const *argv[])
{
    Cycle tmpCycle;
    bool noMoreCycles = true;
    condition_variable condVar1, condVar2;
    CommandsQueue *cq = new CommandsQueue(&condVar1);
    
    cout << "[M_main] Starting Threads..." << endl;
    //starting threads
    thread first (mainReceiver, cq);     
    thread second (mainGPIO, &tmpCycle, &condVar2, &noMoreCycles);

    //creating locker used to wait signal of condition_variable
    mutex mu;
    unique_lock<mutex> locker1(mu);

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
        noMoreCycles = false;

        //doing cycles at the defined time
        this_thread::sleep_for(chrono::microseconds(cq->processNext()));
        cout << "[timer] Lets go (" << tmpCycle.electrode1 << " + " << tmpCycle.offset << ") * " << tmpCycle.qtyElectrodes << endl;  //############################
        for (int i = 0; i < comm.qtyCycles; i++)
        {
            condVar2.notify_one();
            this_thread::sleep_for(chrono::microseconds(comm.durationCycle));
            tmpCycle.electrode1 = (tmpCycle.electrode1 + 1) % comm.qtyElectrodes;
        }
        noMoreCycles = true;  
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