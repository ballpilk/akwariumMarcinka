TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=

HEADERS += \
    EEPROMHelperFunc.h \
    control.ino \
    LEDDayDriver.h \
    softwarePwm.h \
    stepperMotor.h \
    TimerEventQueue.h \
    clockControl.h

