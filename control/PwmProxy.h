#include <SoftwareSerial.h>

#ifndef PWMPROXY_H
#define PWMPROXY_H
#include "PWMParams.h"



class PwmProxy
{
public:
    PwmProxy(unsigned char pin, SoftwareSerial& port)
        :pin(pin), powerLevel(0), serialPort(port)// RX, TX
    {

        setPWM(powerLevel);
    }
    void setPWM(unsigned char val)
    {
        serialPort.print('l');
        serialPort.print(pin);
        serialPort.print(';');
        serialPort.print(val);
        serialPort.print(';');
        powerLevel = val;
    }
    unsigned int getPower()
    {
        return powerLevel;
    }
    void stop()
    {
        setPWM(0);
    }
private:
    unsigned char pin;
    unsigned char powerLevel;
    SoftwareSerial& serialPort;
};

#endif // PWMPROXY_H
