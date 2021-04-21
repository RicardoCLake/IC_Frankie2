#include "Statistic.h"

#define PIN 7
#define VALUE HIGH
#define TIMES 600 

unsigned int vector[TIMES] = {0};
Statistic myStats;

void setup ()
{
    Serial.begin(9600);
    pinMode(PIN, INPUT);
    myStats.clear();   
}

void loop ()
{
    //getting values
    for (int i = 0; i < TIMES; i++)
    {
        vector[i] = pulseIn(PIN, VALUE, 10000000);
        myStats.add(vector[i]);
    }

    //printing values
    for (int i = 0; i < TIMES; i++)
    {
        Serial.println(vector[i]);
    }

    Serial.println("");
    Serial.print("min = ");
    Serial.println(myStats.minimum());
    Serial.print("max = ");
    Serial.println(myStats.maximum());
    Serial.print("average = ");
    Serial.println(myStats.average());
    Serial.print("stdev = ");
    Serial.println(myStats.pop_stdev());
    Serial.print("avrg stdev = ");
    Serial.println(myStats.pop_stdev()/sqrt(TIMES));;
    Serial.println(""); 
     
}
