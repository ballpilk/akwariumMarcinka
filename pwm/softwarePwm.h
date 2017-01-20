#ifndef SOFTWAREPWM_H
#define SOFTWAREPWM_H
#include "EEPROMHelperFunc.h"
#include "PWMParams.h"
class Pwm
{
public:
    Pwm(const PWMParams& par)
        :isHigh(false),params(par), timestamp(0)
    {
           params.maxHigh = par.maxHigh==0?par.pulseLength:par.maxHigh;
           setPWM(1);
    }
    void onTime(unsigned long time)
    {
        if(isHigh)
        {
            if(time-timestamp >= highTime)
            {
                digitalWrite(params.pin,LOW);
                isHigh = false;
                timestamp = time;
            }
        }
        else
        {
            if(time-timestamp >= lowTime && highTime!= 0 )
            {
                digitalWrite(params.pin,HIGH);
                isHigh = true;
                timestamp = time;
            }
        }
    }
    void setPWM(unsigned char val)
    {
//      Serial.print("pwm ");Serial.print(params.pin);Serial.print(" ");Serial.println(val);
        unsigned long highLen = params.maxHigh-params.minHigh;
        highTime = params.minHigh+(highLen*val)/255;
        lowTime = params.pulseLength - highTime;
  //      Serial.print(params.pin);        Serial.print(" ");Serial.print(highTime);Serial.print(" ");Serial.println(lowTime);
    }
    const PWMParams& getPWMConfig() const
    {
      return params;
    }
    void setPWMConfig(const PWMParams& par)
    {
      params = par;
    }
    void stop()
    {
        lowTime = 0;//0 will remove this object from timer queue, and leave the pin on LOW value
    }
private:
    bool isHigh;
    PWMParams params;
    unsigned long highTime, lowTime, timestamp;

};

#endif // SOFTWAREPWM_H
