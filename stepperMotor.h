#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H
#include "TimerEventQueue.h"
#define MINSTEPTIME 19000
#define PINSNUMBER 4
class StepperMotor : public TimerEventListener
{
public:
    StepperMotor(TimerQueue& ptimer,
                 unsigned short ppin1, unsigned short ppin2,
                 unsigned short ppin3, unsigned short ppin4, bool phalfstep = false)
        :timer(ptimer)
    {
        pins[0] = ppin1;
        pins[1] = ppin2;
        pins[2] = ppin3;
        pins[3] = ppin4;
        for(int i=0;i<PINSNUMBER;++i)
          pinMode(pins[i], OUTPUT);
        fullStepCount = 0;
        halfStep = phalfstep;
        digitalWrite(pins[(fullStepCount+1)%PINSNUMBER], HIGH);
        digitalWrite(pins[(fullStepCount)%PINSNUMBER], LOW);

    }
    void start(unsigned short speedPercentage=0)
    {
        if(speedPercentage !=0)
        stepTime = (100./speedPercentage) * MINSTEPTIME;
        timer.registerTimerEvent(this, stepTime);
    }
    void stopMotor()
    {
      Serial.println("StopMotor");
        stepTime = 0;
    }
    virtual long unsigned onTime()
    {
        step();
        return stepTime;
    }
    void setMotorSpeed(unsigned short speedPercentage)
    {
      if(speedPercentage != 0)
        stepTime = (100* MINSTEPTIME)/speedPercentage;
      else 
        stopMotor();
      Serial.println(stepTime);
    }
private:
    void step()
    {
        if(halfStep)
            doHalfStep();
        else
            doFullStep();
    }
    void doFullStep()
    {
     // Serial.println(currentPin);
        digitalWrite(pins[fullStepCount], LOW);
        fullStepCount = (fullStepCount+1)%PINSNUMBER;
        digitalWrite(pins[fullStepCount], HIGH);

    }
    void doHalfStep()
    {
        if(halfstepCount%2 ==0)//odd
        {
            digitalWrite(pins[((halfstepCount/2)+1)%PINSNUMBER], HIGH);
        }
        else//even
        {
            digitalWrite(pins[(halfstepCount/2)%PINSNUMBER], LOW);
        }
        halfstepCount = (halfstepCount+1)%(PINSNUMBER*2);

    }
    TimerQueue& timer;
    unsigned short pins[PINSNUMBER];
    unsigned short fullStepCount;
    unsigned long stepTime;
    unsigned short halfstepCount;
    bool halfStep;
};

#endif // STEPPERMOTOR_H
