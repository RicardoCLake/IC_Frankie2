#define PIN 7
#define VALUE HIGH
#define TIMES 50 

unsigned long vector[TIMES] = {0};

void setup ()
{
    Serial.begin(9600);
    pinMode(PIN, INPUT);   
}
void loop ()
{
    //getting values
    for (int i = 0; i < TIMES; i++)
    {
        vector[i] = pulseIn(PIN, VALUE, 10000000);
        //Serial.println(vector[i]); //############################
    }

    //getting max
    unsigned long maximum = 0;
    for (int i = 0; i < TIMES; i++)
    {
        maximum = max(vector[i], maximum);
    }

    //getting min
    unsigned long minimum = -1;
    for (int i = 0; i < TIMES; i++)
    {
        minimum = min(vector[i], minimum);
    }

    //getting sum
    unsigned long sum = 0;
    for (int i = 0; i < TIMES; i++)
    {
        sum += vector[i];
    }

    //printing values
    for (int i = 0; i < TIMES; i++)
    {
        Serial.println(vector[i]);
    }

    Serial.println("");
    Serial.print("min = ");
    Serial.println(minimum);
    Serial.print("max = ");
    Serial.println(maximum);
    Serial.print("average = ");
    Serial.println(sum/TIMES);
    Serial.println(""); 
     
}
