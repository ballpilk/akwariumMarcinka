

#ifndef EEPROMHELPERFUNC_H
#define EEPROMHELPERFUNC_H

namespace EEPROMHelpers{


static void writeToEeprom(unsigned int addr, unsigned long par)
{
    EEPROM.write(addr++, (unsigned char) (par        & 255));
    EEPROM.write(addr++, (unsigned char)((par >> 8)  & 255));
    EEPROM.write(addr++, (unsigned char)((par >> 16) & 255));
    EEPROM.write(addr,   (unsigned char)((par >> 24) & 255));


}
static unsigned long readLongFromEeprom(unsigned int addr)
{
    unsigned long retval = 0;
    retval +=  EEPROM.read(addr++);
    retval += ((long unsigned int)(EEPROM.read(addr++))) <<  8 & 0x0000ff00;
    retval += ((long unsigned int)(EEPROM.read(addr++))) << 16 & 0x00ff0000;
    retval += ((long unsigned int)(EEPROM.read(addr  ))) << 24 & 0xff000000;
    return retval;
}

};

#endif // EEPROMHELPERFUNC_H
