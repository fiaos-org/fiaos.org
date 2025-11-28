#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "../config/settings.h"

enum DisplayMode {
    DISPLAY_STATUS = 0,
    DISPLAY_CLIMATE,
    DISPLAY_STRESS,
    DISPLAY_POWER,
    DISPLAY_TANK,
    DISPLAY_VALVE,
    DISPLAY_SYSTEM,
    DISPLAY_INSIGHT,
    DISPLAY_MODE_COUNT
};

struct DisplayData {
    float temperature;
    float humidity;
    float vpd;
    float stress;
    float stressMax;
    float damage;
    float solarVoltage;
    float tegVoltage;
    bool powerOK;
    int tankLevel;
    int tankMax;
    bool isRaining;
    bool conserveMode;
    bool isWatering;
    unsigned int totalOpenings;
    unsigned long totalTime;
    bool systemFailure;
    bool sensorError;
    int freeRAM;
    byte errorCount;
    byte hour;
    byte minute;
    bool extremeMode;
    unsigned long cycleCount;
    char insightText[32];
};

class LCDDisplay {
public:
    LCDDisplay();
    bool begin();
    void update(const DisplayData& data);
    void showError(const char* message);
    void showStartup(const char* version);
    void setMode(DisplayMode mode);
    bool isAvailable();

private:
    LiquidCrystal_I2C* _lcd;
    bool _found;
    DisplayMode _currentMode;
    unsigned long _lastUpdate;
    static const byte _addresses[];
    static const byte _addressCount;
    void _showStatus(const DisplayData& data);
    void _showClimate(const DisplayData& data);
    void _showStress(const DisplayData& data);
    void _showPower(const DisplayData& data);
    void _showTank(const DisplayData& data);
    void _showValve(const DisplayData& data);
    void _showSystem(const DisplayData& data);
    void _showInsight(const DisplayData& data);
};

#endif // LCD_DISPLAY_H
