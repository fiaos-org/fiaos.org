#include "tank.h"

WaterTank::WaterTank() {
    _level = MAX_WATER_CAPACITY;
    _conserveMode = false;
    _isRaining = false;
    _rainDetected = false;
    _highHumidityStart = 0;
}

void WaterTank::begin(float level) {
    _level = constrain(level, 0, MAX_WATER_CAPACITY);
    _updateConserveMode();
    
    Serial.print(F("[TANK] Initialized at "));
    Serial.print((int)_level);
    Serial.println(F("ml"));
}

void WaterTank::checkRain(float humidity) {
    if (humidity >= RAIN_DETECTION_THRESHOLD) {
        if (_highHumidityStart == 0) {
            _highHumidityStart = millis();
            Serial.print(F("[TANK] High humidity: "));
            Serial.print(humidity);
            Serial.println(F("% - monitoring"));
        } else if (millis() - _highHumidityStart >= RAIN_DETECTION_TIME) {
            if (!_rainDetected) {
                _rainDetected = true;
                _isRaining = true;
                
                float overflow = add(RAIN_WATER_ADD);
                Serial.print(F("[TANK] Rain confirmed! Added water"));
                if (overflow > 0) {
                    Serial.print(F(", overflow: "));
                    Serial.print((int)overflow);
                    Serial.print(F("ml"));
                }
                Serial.println();
            }
        }
    } else {
        if (_highHumidityStart != 0) {
            Serial.println(F("[TANK] Humidity normalized"));
            _highHumidityStart = 0;
            _rainDetected = false;
            _isRaining = false;
        }
    }
}

float WaterTank::use(float mlRequested) {
    float actual = min(mlRequested, _level);
    _level -= actual;
    _updateConserveMode();
    return actual;
}

float WaterTank::add(float mlToAdd) {
    float newLevel = _level + mlToAdd;
    float overflow = 0;
    
    if (newLevel > MAX_WATER_CAPACITY) {
        overflow = newLevel - MAX_WATER_CAPACITY;
        _level = MAX_WATER_CAPACITY;
    } else {
        _level = newLevel;
    }
    
    _updateConserveMode();
    return overflow;
}

float WaterTank::getLevel() {
    return _level;
}

float WaterTank::getPercentage() {
    return (_level * 100.0) / MAX_WATER_CAPACITY;
}

bool WaterTank::isConserving() {
    return _conserveMode;
}

bool WaterTank::isRaining() {
    return _isRaining;
}

bool WaterTank::hasEnoughFor(float mlNeeded) {
    return _level >= mlNeeded;
}

void WaterTank::setLevel(float level) {
    _level = constrain(level, 0, MAX_WATER_CAPACITY);
    _updateConserveMode();
}

void WaterTank::_updateConserveMode() {
    _conserveMode = (_level < LOW_WATER_THRESHOLD);
}
