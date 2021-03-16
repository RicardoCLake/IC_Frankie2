#include "header.h"

void initReceiver()
{
    return;
}

void receive(CommandsQueue *cq, int duration)
{
    struct timeval time0;
    gettimeofday(&time0, NULL);

    Command comTmp1;
    comTmp1.electrode1 = 0;
    comTmp1.offset = 2;
    comTmp1.qtyCycles = 1000000;
    comTmp1.qtyElectrodes = 4;
    comTmp1.durationCycle = duration;
    time0.tv_sec += 10;
    comTmp1.time = time0;
    
    // (Now, not used)
    Command comTmp2;
    comTmp2.electrode1 = 0;
    comTmp2.offset = 2;
    comTmp2.qtyCycles = 32*60*3;
    comTmp2.qtyElectrodes = 32;
    comTmp2.durationCycle = 500;
    time0.tv_sec += 14;
    comTmp2.time = time0;

    cq->addCommand (comTmp1);
    cout << "[Receiver] Command Added" << endl;
}

