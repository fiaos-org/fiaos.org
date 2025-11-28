#ifndef POWER_H
#define POWER_H

#include <Arduino.h>
#include "../config/pins.h"

class PowerMonitor {
public:
    PowerMonitor();
    void update();
    float getSolarVoltage();
    float getTEGVoltage();
    float getTotalPower();
    bool isSufficient();
    static const float MIN_TOTAL_VOLTAGE;
    static const float MIN_TEG_VOLTAGE;

private:
    float _solarVoltage;
    float _tegVoltage;
    float _totalPower;
    float _readSolar();
    float _readTEG();
    float _readTotal();
};

#endif // POWER_H
