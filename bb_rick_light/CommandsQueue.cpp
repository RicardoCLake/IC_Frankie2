#include "CommandsQueue.h"

CommandsQueue::CommandsQueue()
{
    queue = new list<Command>;
    numberOfCommands = 0;
    this->isVirgin = true;
}

CommandsQueue::~CommandsQueue()
{
    delete queue;
}

Command CommandsQueue::getFront()
{ 
    return queue->front();     //???????????????????????? precisa de locker??
}

int CommandsQueue::processNext()
{    
    if(isVirgin)
    {
        if (numberOfCommands != 0)
        {
            isVirgin = false;
        }
    } 
    else
    {
        //dequeue
        queue->pop_front();
        numberOfCommands--;
    }
    if (numberOfCommands == 0)
    {   
        return 0;
    } 
    
    //create delay with the next
    struct timeval end = queue->front().time;          //??????????????????????? precisa de locker?
    struct timeval now;
    int udelay;
    gettimeofday(&now, NULL);
    udelay = (end.tv_sec-now.tv_sec)*1000000 + end.tv_usec - now.tv_usec;

    //cout << "Processado o proximo da fila (com eletrodo " << queue->front().electrode1 << ")" << endl; //#########################
    return udelay;
}

void CommandsQueue::addCommand(Command a)
{
    queue->push_back(a); 
    numberOfCommands++;
    //cout << "Adicionado o comando no eletrodo " << a.electrode1 << endl; //#######################
}

int CommandsQueue::getNumberOfCommands()
{
    return numberOfCommands;
}