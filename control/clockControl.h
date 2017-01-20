#ifndef CLOCKCONTROL_H
#define CLOCKCONTROL_H
#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527
struct DateTime{
    DateTime()
       :second(0), //0-59
        minute(0), //0-59
        hour(0), //0-23
        weekDay(0), //1-7
        monthDay(1), //1-31
        month(1), //1-12
        year(0) //0-99
    {}

    byte second;
    byte minute;
    byte hour;
    byte weekDay;
    byte monthDay;
    byte month;
    byte year;
};

class ClockControl
{
public:
    ClockControl(){}
    void setTime(const DateTime& time)
    {
        Wire.beginTransmission(DS1307_ADDRESS);
        Wire.write(zero); //stop Oscillator
      
        Wire.write(decToBcd(time.second));
        Wire.write(decToBcd(time.minute));
        Wire.write(decToBcd(time.hour));
        Wire.write(decToBcd(time.weekDay));
        Wire.write(decToBcd(time.monthDay));
        Wire.write(decToBcd(time.month));
        Wire.write(decToBcd(time.year));
      
        Wire.write(zero); //start
      
        Wire.endTransmission();
    }
    DateTime getTime()
    {
        DateTime retval;
        Wire.beginTransmission(DS1307_ADDRESS);
        Wire.write(zero);
        Wire.endTransmission();

        Wire.requestFrom(DS1307_ADDRESS, 7);

        retval.second = bcdToDec(Wire.read());
        retval.minute = bcdToDec(Wire.read());
        retval.hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
        retval.weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
        retval.monthDay = bcdToDec(Wire.read());
        retval.month = bcdToDec(Wire.read());
        retval.year = bcdToDec(Wire.read());
        return retval;
    }
    unsigned long getMillisFromMidnight()
    {
        unsigned long retval(0);
        DateTime time = getTime();
        retval += time.second * 1000;
        retval += time.minute * 60000;
        retval += time.hour * 3600000;
        return retval;
    }
    void writeTimeToSerial()
    {
       Wire.beginTransmission(DS1307_ADDRESS);
        Wire.write(zero);
        Wire.endTransmission();
      
        Wire.requestFrom(DS1307_ADDRESS, 7);
      
        int second = bcdToDec(Wire.read());
        int minute = bcdToDec(Wire.read());
        int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
        int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
        int monthDay = bcdToDec(Wire.read());
        int month = bcdToDec(Wire.read());
        int year = bcdToDec(Wire.read());
      
        //print the date EG   3/1/11 23:59:59
        Serial.print(year);
        Serial.print("/");
        Serial.print(month);
        Serial.print("/");
        Serial.print(monthDay);
        Serial.print(" ");
        Serial.print(hour);
        Serial.print(":");
        Serial.print(minute);
        Serial.print(":");
        Serial.println(second);
    }
private:
    byte decToBcd(byte val){
    // Convert normal decimal numbers to binary coded decimal
      return ( (val/10*16) + (val%10) );
    }
    
    byte bcdToDec(byte val)  {
    // Convert binary coded decimal to normal decimal numbers
      return ( (val/16*10) + (val%16) );
    }

};

#endif // CLOCKCONTROL_H
