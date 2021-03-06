#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>

#include <SoftwareSerial.h>

#include <Arduino.h>
#include <Wire.h>

#include <EEPROM.h>

#include "LEDDayDriver.h"
#include "clockControl.h"
#include "LCDDisplay.h"
#define numberOfLeds 16
DayParams day = {0, 255, 100/*daystart*/,40000 /*daylengthtime*/, 0 /*timeAdj*/};
ClockControl clock = ClockControl();
SoftwareSerial serialPort(3,4);//rx,tx
LEDDayDriver leds[] = 
{
  LEDDayDriver(day,2, serialPort, false),
  LEDDayDriver(day,3, serialPort, false),
  LEDDayDriver(day,4, serialPort, false),
  LEDDayDriver(day,5, serialPort, false),
  LEDDayDriver(day,6, serialPort, false),
  LEDDayDriver(day,7, serialPort, false),
  LEDDayDriver(day,8, serialPort, false),
  LEDDayDriver(day,9, serialPort, false),
  LEDDayDriver(day,10, serialPort, false),
  LEDDayDriver(day,11, serialPort, false),
  LEDDayDriver(day,12, serialPort, false),
  LEDDayDriver(day,13, serialPort, false),
  LEDDayDriver(day,14, serialPort, false),//A0
  LEDDayDriver(day,15, serialPort, false),//A1
  LEDDayDriver(day,16, serialPort, false),//A2
  LEDDayDriver(day,17, serialPort, false)//A3
};

void configureDay()
{
    int pin = Serial.parseInt();
    if(pin < numberOfLeds+2 && pin > 1 )
    {
          DayParams param = leds[pin-2].getDayConfig();
          param.daystarttime = Serial.parseInt();
          param.daylengthtime = Serial.parseInt();
          param.min = Serial.parseInt();
          param.max = Serial.parseInt();
          leds[pin-2].setDayConfig(param);
          unsigned int startAddr = sizeof(DayParams)*(pin-2);
          param.writeToEeprom(startAddr);
    }
}
void configureClock()
{
    DateTime clockConf;
    clockConf.year = Serial.parseInt();
    clockConf.month = Serial.parseInt();
    clockConf.monthDay = Serial.parseInt();
    clockConf.hour = Serial.parseInt();
    clockConf.minute = Serial.parseInt();
    clockConf.second = Serial.parseInt();
    clockConf.weekDay = Serial.parseInt();
    clock.setTime(clockConf);
    //add leddaydrivers reconfiguration
}


void writeConfigurationToSerial()
{
  for(unsigned char i=0;i<numberOfLeds;++i)
  {
    leds[i].getDayConfig().writeToSerial(i+2);
  }
}

void readConfFromEEPROM()
{
  unsigned int startAddr(0);
  DayParams day;
  unsigned long timeAdjustment = clock.getMillisFromMidnight();
  for(unsigned char i=0;i<numberOfLeds;++i)
  {
    Serial.println(startAddr);
    day.readFromEeprom(startAddr);
    day.timeAdjustment = timeAdjustment;
    leds[i].setDayConfig(day);
    startAddr += sizeof(DayParams);
  }  

}
void updateTimeAdjustment()
{
  unsigned long timeAdjustment = clock.getMillisFromMidnight();
  for(unsigned char i=0;i<numberOfLeds;++i)
  {
    DayParams day = leds[i].getDayConfig();
    day.timeAdjustment = timeAdjustment;
    leds[i].setDayConfig(day);
  }
}
void setup() {
    Wire.begin();
  Serial.begin(9600);                 // create the serial connection at 9600bit/s baudrate
  serialPort.begin(9600);
  //initialConfiguration from eeprom
  readConfFromEEPROM();
  writeConfigurationToSerial();
} 
long unsigned int timestamp = 0;
void loop()
{
  static LCDDisplay display(leds, numberOfLeds, clock, 16 ,2);
  while(Serial.available() > 0)
  {
    char data = Serial.read();
    switch(data)
    {
        case 'd':
        {
            configureDay();
            Serial.println("day configured");
            break;
        }
        case 't':
        {
            configureClock();
            Serial.println("clock set");
            break;
        }
        case 'y':
        {
          clock.writeTimeToSerial();
          
          break;
        }
        case 'c':
        {
            writeConfigurationToSerial();
            break;
        }
        case 'w':
        {
          for(int i=0;i<numberOfLeds;++i)
          {
              leds[i].setDemo(false);
          }
          break;
        }
        case 'q':
        {
          for(int i=0;i<numberOfLeds;++i)
          {
              leds[i].setDemo(true);
          }
          break;
        }
		default:
		{
			Serial.print(data);
			break;
		}
    }
  }
    unsigned long milliseconds = millis();
    for(int i=0;i<numberOfLeds;++i)
    {
        leds[i].onTime(milliseconds);
    }
    if(milliseconds%1000==0)
    {
        display.print();
    }
    if(milliseconds%3600000==1)
        updateTimeAdjustment();

}

