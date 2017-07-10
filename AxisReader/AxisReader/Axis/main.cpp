
#include <Arduino.h>
#include <Wire.h>
#include <ArdusatSDK.h>


#define B_1A 11
#define B_1B 10
ArdusatSerial serialConnection(SERIAL_MODE_HARDWARE_AND_SOFTWARE, 8, 9);

Magnetic mag;
float distance(float a,float b)
{
    int d = abs(a - b) % 360;
    int r = d > 180 ? 360 - d : d;

    //calculate sign
    int sign = (a - b >= 0 && a - b <= 180) || (a - b <=-180 && a- b>= -360) ? 1 : -1;
    r *= sign;
    return r;
}
double Setpoint = 100, Input, Output;

void setup(void)
{

    //initialize the variables we're linked to
    Input = 90;
    Setpoint = 100;

    //turn the PID on
    

    Serial.begin(9600);

    
    /* Initialize the sensor */
    if(!mag.begin())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        while(1);
    }
    pinMode(B_1A,OUTPUT);
    pinMode(B_1B,OUTPUT);
    digitalWrite(B_1A, LOW);
    digitalWrite(B_1B,LOW);
    
}

void runFor(int power, int forMs, int direction)
{
    //	char buffer[100];
    //	sprintf(buffer,"Power: %i time %i: direction %i", power, forMs, direction);
    if(direction == 1)
    power -=1;
    int pin = (direction == -1 ? B_1A : B_1B);
    //	sprintf(buffer,"Pin %i", pin);
    analogWrite(pin,power += direction);
    delay(forMs);
    analogWrite(pin,0);
    digitalWrite(pin, LOW);
    //	delay(10);
    //Serial.println(buffer);
}
int lastReading = 0;

int listHead = 0;
typedef struct
{
    float x;
    float y;
}	VECT;
VECT averageMag(int of)
{
    VECT sumVect = {0};
    
    //Serial.println("Mag data");
    for(int i = 0; i < of; i++)
    {
        mag.read();
        sumVect.x += mag.x;
        sumVect.y += mag.y;
        //Serial.print(sumVect.x);
        //Serial.print(",");
        //Serial.print(sumVect.y);
        //Serial.print("|");
    }
    sumVect.x = sumVect.x / of;
    sumVect.y = sumVect.y / of;
    // 	Serial.println("Mag avg");
    // 	Serial.print(sumVect.x);
    // 	Serial.print(",");
    // 	Serial.print(sumVect.y);
    //	Serial.println();
    return sumVect;
}


float GetHeading()
{
    // Calculate the angle of the vector y,x
    auto avgVect = averageMag(20);

    float heading = 0.0;
    
    if(avgVect.y>0)
    heading = 90 - atan(avgVect.x/avgVect.y)*180/PI;
    else if(avgVect.y<0)
    heading = 270 - atan(avgVect.x/avgVect.y)*180/PI;
    else if (avgVect.y==0 && avgVect.x < 0)
    heading = 180;
    else if (avgVect.y==0 && avgVect.x > 0)
    heading = 0.0;
    return heading;
}

float setPoint = 0;
int loopCount = 0;
void loop(void)
{
    float heading = GetHeading();
    
    char buffer[100];
    float dist = distance(heading,setPoint);
    Serial.println(buffer);
    int travelFor = max(min(abs(dist),100),2);
    sprintf(buffer,"H\t\t%i\t\tS\t\t%i\t\tD\t\t%i\t\tT\t\t%i", (int)heading,(int)setPoint,  (int)dist,(int)travelFor);
    if(dist < -5)
    runFor(250, travelFor, 1);
    else if(dist > 5)
    runFor(250, travelFor, -1);
    
    delay(80);
}




