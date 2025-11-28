#include "environment.h"

EnvironmentSensor::EnvironmentSensor() {
    _initialized = false;
    _consecutiveErrors = 0;
    _bufferIndex = 0;
    _lastGoodReading = 0;
    _reconnectAttempts = 0;
    _lastValidTemp = 25.0;
    _lastValidHumidity = 60.0;
    _lastValidTime = 0;
    for (byte i = 0; i < 3; i++) {
        _tempBuffer[i] = 25.0;
        _humidityBuffer[i] = 60.0;
    }
}

bool EnvironmentSensor::begin() {
    if (_bme.begin(BME280_ADDR_PRIMARY)) {
        _initialized = true;
        Serial.println(F("[SENSOR] BME280 found at 0x76"));
        return true;
    }
    if (_bme.begin(BME280_ADDR_SECONDARY)) {
        _initialized = true;
        Serial.println(F("[SENSOR] BME280 found at 0x77"));
        return true;
    }
    Serial.println(F("[SENSOR] BME280 not found"));
    _initialized = false;
    return false;
}

ClimateReading EnvironmentSensor::read() {
    ClimateReading reading;
    reading.valid = false;
    if (!_initialized) {
        if (_attemptRecovery(reading.temperature, reading.humidity)) {
            reading.pressure = ATM_PRESSURE_HPA;
            reading.vpd = calculateVPD(reading.temperature, reading.humidity);
            reading.valid = true;
        }
        return reading;
    }
    float temp = _bme.readTemperature();
    float humidity = _bme.readHumidity();
    float pressure = _bme.readPressure() / 100.0F;
    if (_validate(temp, humidity)) {
        _storeReading(temp, humidity);
        reading.temperature = temp;
        reading.humidity = humidity;
        reading.pressure = pressure;
        reading.vpd = calculateVPD(temp, humidity);
        reading.valid = true;
        _lastValidTemp = temp;
        _lastValidHumidity = humidity;
        _lastValidTime = millis();
        if (_consecutiveErrors > 0) {
            _consecutiveErrors = 0;
            Serial.println(F("[SENSOR] Recovered"));
        }
    } else {
        _consecutiveErrors++;
        if (_attemptRecovery(reading.temperature, reading.humidity)) {
            reading.pressure = ATM_PRESSURE_HPA;
            reading.vpd = calculateVPD(reading.temperature, reading.humidity);
            reading.valid = true;
        }
    }
    return reading;
}

ClimateReading EnvironmentSensor::getLastValid() {
    ClimateReading reading;
    reading.temperature = _lastValidTemp;
    reading.humidity = _lastValidHumidity;
    reading.pressure = ATM_PRESSURE_HPA;
    reading.vpd = calculateVPD(_lastValidTemp, _lastValidHumidity);
    reading.valid = true;
    return reading;
}

bool EnvironmentSensor::isHealthy() {
    return _initialized && (_consecutiveErrors < MAX_ERROR_TOLERANCE);
}

byte EnvironmentSensor::getErrorCount() {
    return _consecutiveErrors;
}

float EnvironmentSensor::calculateVPD(float temperature, float humidity) {
    float es = 0.6108 * exp((17.27 * temperature) / (temperature + 237.3));
    float ea = es * (humidity / 100.0);
    return (es - ea);
}

bool EnvironmentSensor::_validate(float temp, float humidity) {
    if (isnan(temp) || isnan(humidity)) return false;
    if (temp < -20 || temp > 60) return false;
    if (humidity < 0 || humidity > 100) return false;
    if (_lastValidTime > 0) {
        if (abs(temp - _lastValidTemp) > 10) return false;
        if (abs(humidity - _lastValidHumidity) > 30) return false;
    }
    return true;
}

void EnvironmentSensor::_storeReading(float temp, float humidity) {
    _tempBuffer[_bufferIndex] = temp;
    _humidityBuffer[_bufferIndex] = humidity;
    _bufferIndex = (_bufferIndex + 1) % 3;
    _lastGoodReading = millis();
}

float EnvironmentSensor::_getBufferAvgTemp() {
    float sum = 0;
    for (byte i = 0; i < 3; i++) sum += _tempBuffer[i];
    return sum / 3.0;
}

float EnvironmentSensor::_getBufferAvgHumidity() {
    float sum = 0;
    for (byte i = 0; i < 3; i++) sum += _humidityBuffer[i];
    return sum / 3.0;
}

bool EnvironmentSensor::_attemptRecovery(float &temp, float &humidity) {
    _reconnectAttempts++;
    if (_reconnectAttempts <= MAX_ERROR_TOLERANCE) {
        Serial.print(F("[SENSOR] Reconnect attempt #"));
        Serial.println(_reconnectAttempts);
        delay(500);
        if (_bme.begin(BME280_ADDR_PRIMARY) || _bme.begin(BME280_ADDR_SECONDARY)) {
            delay(100);
            float testTemp = _bme.readTemperature();
            float testHumidity = _bme.readHumidity();
            if (!isnan(testTemp) && !isnan(testHumidity)) {
                temp = testTemp;
                humidity = testHumidity;
                _reconnectAttempts = 0;
                _initialized = true;
                Serial.println(F("[SENSOR] Reconnected successfully"));
                return true;
            }
        }
    }
    if (_lastGoodReading > 0) {
        temp = _getBufferAvgTemp();
        humidity = _getBufferAvgHumidity();
        Serial.print(F("[SENSOR] Using buffer avg - T:"));
        Serial.print(temp); Serial.print(F(" H:")); Serial.println(humidity);
        return true;
    }
    temp = 25.0;
    humidity = 60.0;
    Serial.println(F("[SENSOR] Using safe defaults"));
    return true;
}
