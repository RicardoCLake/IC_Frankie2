#include "header.h"

void mainReceiver (CommandsQueue* cq)
{
    struct timeval time0;
    gettimeofday(&time0, NULL);

    Command comTmp1;
    comTmp1.electrode1 = 0;
    comTmp1.offset = 1;
    comTmp1.qtyCycles = 1;
    comTmp1.qtyElectrodes = 16;
    comTmp1.durationCycle = 50;
    time0.tv_sec += 20;
    comTmp1.time = time0;
    
    Command comTmp2;
    comTmp2.electrode1 = 0;
    comTmp2.offset = 2;
    comTmp2.qtyCycles = 32*60*3;
    comTmp2.qtyElectrodes = 32;
    comTmp2.durationCycle = 500;
    time0.tv_sec += 14;
    comTmp2.time = time0;
    
    while(1)
    {
        this_thread::sleep_for(chrono::seconds(3));

        //cq->addCommand(comTmp1);
        cq->addCommand(comTmp2);
        cout << "[M_receiver] Commands Added" << endl;
    }

}

