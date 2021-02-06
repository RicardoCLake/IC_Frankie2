#include <signal.h>
#include <iostream>

#ifndef ALARMER_H
#define ALARMER_H

class Alarmer
{
    private:
        struct sigaction action;
    public:
        Alarmer(void handler(int));
        ~Alarmer();
        void init();
};
#endif
