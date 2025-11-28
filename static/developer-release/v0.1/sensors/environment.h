#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "../config/pins.h"
#include "../config/settings.h"

struct ClimateReading {
    float temperature;
    float humidity;
    float pressure;
    float vpd;
    bool valid;
};

class EnvironmentSensor {
public:
    EnvironmentSensor();
    bool begin();
    ClimateReading read();
    ClimateReading getLastValid();
    bool isHealthy();
    byte getErrorCount();
    static float calculateVPD(float temperature, float humidity);

private:
    Adafruit_BME280 _bme;
    bool _initialized;
    byte _consecutiveErrors;
    float _tempBuffer[3];
    float _humidityBuffer[3];
    byte _bufferIndex;
    unsigned long _lastGoodReading;
    byte _reconnectAttempts;
    float _lastValidTemp;
    float _lastValidHumidity;
    unsigned long _lastValidTime;
    bool _validate(float temp, float humidity);
    void _storeReading(float temp, float humidity);
    float _getBufferAvgTemp();
    float _getBufferAvgHumidity();
    bool _attemptRecovery(float &temp, float &humidity);
};

#endif 
