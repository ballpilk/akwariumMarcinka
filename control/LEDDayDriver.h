#ifndef LEDDAYDRIVER_H
#define LEDDAYDRIVER_H
#include "PwmProxy.h"
#include "EEPROMHelperFunc.h"
#define UPDATE_PERIOD 100
#define MILLISINDAY 86400000
struct DayParams
{
unsigned char min;
unsigned char max;
unsigned long daystarttime;
unsigned long daylengthtime;
unsigned long timeAdjustment;
void readFromEeprom(unsigned int addr)
{
    daystarttime = EEPROMHelpers::readLongFromEeprom(addr);
    addr+= sizeof(unsigned long);
    daylengthtime = EEPROMHelpers::readLongFromEeprom(addr);
    addr+= sizeof(unsigned long);
    min = EEPROM.read(addr++);
    max = EEPROM.read(addr);
}
void writeToEeprom(unsigned int startAddr) const
{
    EEPROMHelpers::writeToEeprom(startAddr, daystarttime);
    startAddr+= sizeof(unsigned long);
    EEPROMHelpers::writeToEeprom(startAddr, daylengthtime);
    startAddr+= sizeof(unsigned long);
    EEPROM.write(startAddr++, min);
    EEPROM.write(startAddr, max);
}
void writeToSerial(unsigned char pin) const
{
  Serial.print("d,");
  Serial.print(pin);Serial.print(',');
  Serial.print(daystarttime);Serial.print(',');
  Serial.print(daylengthtime);Serial.print(',');
  Serial.print(min);  Serial.print(',');
  Serial.println(max);
}
};

class LEDDayDriver
{
public:
    LEDDayDriver( const DayParams& pday, const int pwmPin, SoftwareSerial& serialPort, bool demo = true)
        :ledpin( pwmPin, serialPort), day(pday), timestamp(0), currentState(0)

    {
        setDemo(demo);
        adjustPwm();
    }
    void onTime( unsigned long ptime)
    {
        if(UPDATE_PERIOD <= ptime - timestamp)
        {
    //      Serial.print("gaaa");
            adjustPwm();
            timestamp = ptime;
        }
    }
    void setDayConfig(const DayParams& pday)//add getter and rework to use DayParams structure
    {
        day = pday;
    }
    const DayParams& getDayConfig()const
    {
        return day;
    }
    void setDemo(bool demo)
    {
        if(demo)
          millisInDay = MILLISINDAY/1000;
        else
          millisInDay = MILLISINDAY;
    }
    inline PwmProxy& getPwm(){return ledpin;}
private:
    void adjustPwm()
    {
        unsigned char newState = day.min;
        unsigned long currentTime = (millis()+day.timeAdjustment)%millisInDay;//modulo miliseconds in 24h
        if(currentTime < day.daystarttime+day.daylengthtime && currentTime > day.daystarttime)
        {
             newState = day.min+(day.max-day.min)*(((-1.*cos(2.*PI * (double)(currentTime - day.daystarttime)/(double)day.daylengthtime))+1)/2);
        }//min(^^*1./k, 255)
        if(newState != currentState)
        {
          ledpin.setPWM(newState);
          currentState = newState;
        }
    }
    unsigned long millisInDay;
    PwmProxy ledpin;
    unsigned long timestamp;
    DayParams day;
    unsigned char currentState;
};

#endif // LEDDAYDRIVER_H
