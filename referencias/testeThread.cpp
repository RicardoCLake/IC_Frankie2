// thread example
#include <iostream>       // cout
#include <thread>         // thread
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
 
 using namespace std;

void main1() 
{
    timeval tempo1;
    usleep(30000);
  while(1)
  {
    usleep(30000);
    gettimeofday(&tempo1, NULL);
    cout<< "tempo03 (" <<  tempo1.tv_sec << "." << tempo1.tv_usec << ")\n";
  }
  
}

void main2(int x)
{
timeval tempo1;
  while(1)
  {
    usleep(100000);
    gettimeofday(&tempo1, NULL);
    cout<< "Tempo10 (" <<  tempo1.tv_sec << "." << tempo1.tv_usec << ")\n";
  }
}

int main() 
{
  thread first (main1);     // spawn new thread that calls foo()
  thread second (main2,0);  // spawn new thread that calls bar(0)
  usleep(1000000);
  cout << "main, foo and bar now execute concurrently...\n";

  // synchronize threads:
  first.join();                // pauses until first finishes
  second.join();               // pauses until second finishes
  
  
  cout << "foo and bar completed.\n";

  return 0;
}
