#include "header.h"

void mainReceiver (CommandsQueue* cq, int duration)
{
    struct timeval time0;
    gettimeofday(&time0, NULL);

    Command comTmp1;
    comTmp1.electrode1 = 0;
    comTmp1.offset = 1;
    comTmp1.qtyCycles = 1000000;
    comTmp1.qtyElectrodes = 15;
    comTmp1.durationCycle = 50;
    time0.tv_sec += 20;
    comTmp1.time = time0;
    
    Command comTmp2;
    comTmp2.electrode1 = 0;
    comTmp2.offset = 2;
    comTmp2.qtyCycles = 1000000;
    comTmp2.qtyElectrodes = 4;
    comTmp2.durationCycle = duration;
    time0.tv_sec += 14;
    comTmp2.time = time0;
    
    this_thread::sleep_for(chrono::seconds(4));

    //cq->addCommand(comTmp1);
    cq->addCommand(comTmp2);
    cout << "[M_receiver] Commands Added" << endl;

}

