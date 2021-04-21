#include "CommandsQueue.h"

CommandsQueue::CommandsQueue(condition_variable* condVarPointer)
{
    queue = new list<Command>;
    numberOfCommands = 0;
    this->condVarPointer = condVarPointer;
    this->isVirgin = true;
}

CommandsQueue::~CommandsQueue()
{
    delete queue;
}

Command CommandsQueue::getFront()
{ 
    return queue->front();     
}

int CommandsQueue::processNext()
{    
    unique_lock<mutex> locker2(mtx);
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
            locker2.unlock();
            return 0;
        } 
    }
    locker2.unlock();
    
    //create delay with the next
    struct timeval end = queue->front().time;          
    struct timeval now;
    int udelay;
    gettimeofday(&now, NULL);
    udelay = (end.tv_sec-now.tv_sec)*1000000 + end.tv_usec - now.tv_usec;

    //cout << "Processado o proximo da fila (com eletrodo " << queue->front().electrode1 << ")" << endl; //#########################
    return udelay;
}

void CommandsQueue::addCommand(Command a)
{
    unique_lock<mutex> locker1(mtx);
    {
        queue->push_back(a); 
        numberOfCommands++;
    }
    locker1.unlock();

    //notifing that the queue is not empty
    (*condVarPointer).notify_all();

    //cout << "Adicionado o comando no eletrodo " << a.electrode1 << endl; //#######################
}

int CommandsQueue::getNumberOfCommands()
{
    return numberOfCommands;
}