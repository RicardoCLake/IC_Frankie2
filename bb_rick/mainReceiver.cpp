#include "header.h"

void mainReceiver (CommandsQueue* cq)
{
    struct timeval time0;
    gettimeofday(&time0, NULL);

    Command comTmp1;
    comTmp1.electrode1 = 0;
    comTmp1.offset = 15;
    comTmp1.qtyCycles = 50;
    comTmp1.qtyElectrodes = 4;
    comTmp1.durationCycle = 200000;
    time0.tv_sec += 20;
    comTmp1.time = time0;
    
    Command comTmp2;
    comTmp2.electrode1 = 0;
    comTmp2.offset = 3;
    comTmp2.qtyCycles = 64;
    comTmp2.qtyElectrodes = 32;
    comTmp2.durationCycle = 900000;
    time0.tv_sec += 14;
    comTmp2.time = time0;
    
    this_thread::sleep_for(chrono::seconds(4));

    cq->addCommand(comTmp1);
    cq->addCommand(comTmp2);
    cout << "[M_receiver] Commands Added" << endl;

}

