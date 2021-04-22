#include "Statistic.h"

#define PIN 7
#define VALUE HIGH
#define TIMES1 500 //less than 600
#define TIMES2 12
#define RATIO  25  //devisor of TIMES1

unsigned int vector[TIMES1] = {0};
Statistic myStats;

void setup ()
{
    Serial.begin(9600);
    pinMode(PIN, INPUT);    
}

void loop ()
{
    //restarting statistic
    myStats.clear();

    for (int j = 0; j < TIMES2; j++)
    {
        //getting values
        for (int i = 0; i < TIMES1; i++)
        {
            vector[i] = pulseIn(PIN, VALUE, 10000000);
            myStats.add(vector[i]);
        }
    
        //printing values
        Serial.print(j + 1);
        Serial.print(" / ");
        Serial.print(TIMES2);
        Serial.println(" - - - - - - -");
        for (int i = 0; i < TIMES1/RATIO; i++)
        {
            for (int k = 0; k < RATIO; k++)
            {
                Serial.print(vector[k + RATIO * i]);
                Serial.print(" ");
            }
            Serial.println("");
        } 
    }
    
    //printing statistics results
    Serial.println("");
    Serial.println("##################################");
    Serial.print("# num of cycles = ");
    Serial.println(myStats.count());
    Serial.print("# min = ");
    Serial.println(myStats.minimum());
    Serial.print("# max = ");
    Serial.println(myStats.maximum());
    Serial.print("# average = ");
    Serial.println(myStats.average());
    Serial.print("# stdev = ");
    Serial.println(myStats.pop_stdev());
    Serial.print("# avrg stdev = ");
    Serial.println(myStats.pop_stdev()/sqrt(myStats.count()));;     
    Serial.println("##################################");
    Serial.println("");
}
