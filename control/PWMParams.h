#ifndef PWMPARAMS_H
#define PWMPARAMS_H
#include "EEPROMHelperFunc.h"
struct PWMParams
{
    PWMParams(int pPin, unsigned long int ppulseLen, unsigned long pminHi, unsigned long pmaxHi)
      :pin(pPin), pulseLength(ppulseLen), minHigh(pminHi), maxHigh(pmaxHi)
    {}
    void readFromEeprom(unsigned int addr)
    {
        pin = EEPROM.read(addr);
        addr+= sizeof(unsigned char);
        pulseLength = EEPROMHelpers::readLongFromEeprom(addr);
        addr+= sizeof(unsigned long);
        minHigh = EEPROMHelpers::readLongFromEeprom(addr);
        addr+= sizeof(unsigned long);
        maxHigh = EEPROMHelpers::readLongFromEeprom(addr);

    }
    void writeToEeprom(unsigned int startAddr)const
    {
        EEPROM.write(startAddr, pin);
        startAddr += sizeof(unsigned char);
        EEPROMHelpers::writeToEeprom(startAddr, pulseLength);
        startAddr+= sizeof(unsigned long);
        EEPROMHelpers::writeToEeprom(startAddr, minHigh);
        startAddr+= sizeof(unsigned long);
        EEPROMHelpers::writeToEeprom(startAddr, maxHigh);
    }
    void writeToSerial(unsigned char pinNo)const
    {
      Serial.print("p,");
      Serial.print(pin);Serial.print(',');
      Serial.print(pulseLength);Serial.print(',');
      Serial.print(minHigh);Serial.print(',');
      Serial.println(maxHigh);
    }
    unsigned char pin;
    unsigned long pulseLength;
    unsigned long minHigh;
    unsigned long maxHigh;
};
#endif // PWMPARAMS_H
