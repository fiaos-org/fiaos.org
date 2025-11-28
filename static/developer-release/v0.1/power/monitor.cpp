#include "monitor.h"

const float PowerMonitor::MIN_TOTAL_VOLTAGE = 3.0;
const float PowerMonitor::MIN_TEG_VOLTAGE = 1.0;

PowerMonitor::PowerMonitor() {
    _solarVoltage = 0;
    _tegVoltage = 0;
    _totalPower = 0;
}

void PowerMonitor::update() {
    _solarVoltage = _readSolar();
    _tegVoltage = _readTEG();
    _totalPower = _readTotal();
}

float PowerMonitor::getSolarVoltage() {
    return _solarVoltage;
}

float PowerMonitor::getTEGVoltage() {
    return _tegVoltage;
}

float PowerMonitor::getTotalPower() {
    return _totalPower;
}

bool PowerMonitor::isSufficient() {
    return (_totalPower > MIN_TOTAL_VOLTAGE || _tegVoltage > MIN_TEG_VOLTAGE);
}

float PowerMonitor::_readSolar() {
    return analogRead(SOLAR_PIN) * (5.0 / 1023.0) * 2;
}

float PowerMonitor::_readTEG() {
    return analogRead(TEG_PIN) * (5.0 / 1023.0);
}

float PowerMonitor::_readTotal() {
    return analogRead(POWER_MONITOR_PIN) * (5.0 / 1023.0);
}
