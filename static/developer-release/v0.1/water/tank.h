#ifndef TANK_H
#define TANK_H

#include <Arduino.h>
#include "../config/settings.h"

class WaterTank {
public:
    WaterTank();
    void begin(float level);
    void checkRain(float humidity);
    float use(float mlRequested);
    float add(float mlToAdd);
    float getLevel();
    float getPercentage();
    bool isConserving();
    bool isRaining();
    bool hasEnoughFor(float mlNeeded);
    void setLevel(float level);

private:
    float _level;
    bool _conserveMode;
    bool _isRaining;
    bool _rainDetected;
    unsigned long _highHumidityStart;
    void _updateConserveMode();
};

#endif // TANK_H
