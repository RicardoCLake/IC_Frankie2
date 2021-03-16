#ifndef COMMANDSQUEUE_H
#define COMMANDSQUEUE_H

#include <iostream>
#include <time.h>
#include <list>
#include <signal.h>
#include <sys/time.h>
#include <mutex>
#include <condition_variable>

#include "Gpio.h"

using namespace std;

struct Command
{
    timeval time;       //starting point
    int electrode1;     //[0;(qtyElectrodes - 1)]
    int offset;         //electrode1 + offset = electrode2
    int qtyElectrodes;
    int qtyCycles;
    int durationCycle;  //usecs
};

class CommandsQueue
{
    private:
        list<Command> *queue;
        int numberOfCommands;
        
        mutex mtx;
        condition_variable* condVarPointer;

    public:
        CommandsQueue(condition_variable* condVarPointer);
        ~CommandsQueue();
        Command getNext();
        int processNext();
        void addCommand(Command a);
        int getNumberOfCommands();

};


#endif //COMMANDSQUEUE_H