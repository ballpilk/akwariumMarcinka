#include <SoftPWM.h>
#include <SoftPWM_timer.h>

void changePowerLevel()
{
    int pin = Serial.parseInt();
    Serial.print("pin ");     Serial.print(pin);

    if(pin < 18 && pin > 1 )
    {
        if(pin<15)
            SoftPWMSet(pin, Serial.parseInt());
        else
            SoftPWMSet(pin, 255 - Serial.parseInt());
    }
}


void setup() {
  SoftPWMBegin();
  for (int i=2;i<15; ++i)
    SoftPWMSet(i, 0);
  for (int i=15;i<18; ++i)
    SoftPWMSet(i, 255);
  Serial.begin(9600);                 // create the serial connection at 9600bit/s baudrate
} 
long unsigned int timestamp = 0;
void loop()
{
  while(Serial.available() > 0)
  {
    char data = Serial.read();
    switch(data)
    {
        case 'l':
        {
            changePowerLevel();
            Serial.println(" power level configured");
            break;
        }
    }
  }
  if(millis() - timestamp > 5000)
  {
    timestamp = millis();
    Serial.print(".");
  }
}
  
