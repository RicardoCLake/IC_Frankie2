#include "Alarmer.h"

Alarmer::Alarmer(void handler(int))
{
    /* What to do with a SIGALRM */
    action.sa_handler = handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;
}

Alarmer::~Alarmer()
{
}

void Alarmer::init()
{
    sigaction (SIGALRM, &action, NULL);
}
