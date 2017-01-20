
#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H
#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>


#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "LEDDayDriver.h"
#include "clockControl.h"

#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

class LCDDisplay
{
public:
    LCDDisplay(LEDDayDriver* leds, unsigned char ledsno, ClockControl& pclock, unsigned char szer=20, unsigned char wys=4, unsigned char addr=0x27)
    :lcd(addr,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin), clock(pclock), led(leds), ledno(ledsno)
    {
        lcd.begin (szer,wys);
        lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
        lcd.setBacklight(HIGH);
        lcd.home (); // go home
    }
    void print()
    {
      static unsigned char lednumber = 0;
      lcd.setCursor (0,0);
      DateTime time = clock.getTime();
      if(time.hour<10)
        lcd.print("0");
      lcd.print(time.hour,DEC);
      lcd.print(time.second % 2 ? ":" : " ");
      if(time.minute<10)
        lcd.print("0");
      lcd.print(time.minute,DEC);
      lcd.print(time.second % 2 ? ":" : " ");
      if(time.second<10)
        lcd.print("0");
      lcd.print(time.second);
      lcd.setCursor (0,1);
      lednumber = (lednumber+1)%(ledno*10);
      lcd.print((int)(lednumber/10)+2);
      lcd.print(": ");
      lcd.print(led[(int)(lednumber/10)].getPwm().getPower()); 
      lcd.print("    ");

    }
private:
    LiquidCrystal_I2C   lcd;
    unsigned char ledno;
    ClockControl& clock;
    LEDDayDriver* led;
};
#endif // LCDDISPLAY_H
