#include "lcd.h"

const byte LCDDisplay::_addresses[] = {0x27, 0x3F, 0x38, 0x20};
const byte LCDDisplay::_addressCount = 4;

LCDDisplay::LCDDisplay() {
    _lcd = nullptr;
    _found = false;
    _currentMode = DISPLAY_STATUS;
    _lastUpdate = 0;
}

bool LCDDisplay::begin() {
    Serial.println(F("[LCD] Scanning for display..."));
    
    for (byte i = 0; i < _addressCount; i++) {
        Serial.print(F("[LCD] Trying 0x"));
        Serial.println(_addresses[i], HEX);
        
        if (_lcd) delete _lcd;
        _lcd = new LiquidCrystal_I2C(_addresses[i], 16, 2);
        
        _lcd->init();
        delay(100);
        _lcd->backlight();
        _lcd->clear();
        
        _found = true;
        Serial.print(F("[LCD] Found at 0x"));
        Serial.println(_addresses[i], HEX);
        return true;
    }
    
    Serial.println(F("[LCD] Not found"));
    return false;
}

void LCDDisplay::showStartup(const char* version) {
    if (!_found) return;
    
    _lcd->clear();
    _lcd->setCursor(0, 0);
    _lcd->print("Umbrella ");
    _lcd->print(version);
    _lcd->setCursor(0, 1);
    _lcd->print("Init...");
}

void LCDDisplay::showError(const char* message) {
    if (!_found) return;
    
    _lcd->clear();
    _lcd->setCursor(0, 0);
    _lcd->print("ERR: ");
    _lcd->print(message);
}

void LCDDisplay::update(const DisplayData& data) {
    if (!_found) return;
    
    unsigned long now = millis();
    if (now - _lastUpdate < LCD_UPDATE_INTERVAL && _lastUpdate != 0) {
        return;
    }
    
    _lcd->clear();
    
    switch (_currentMode) {
        case DISPLAY_STATUS:  _showStatus(data); break;
        case DISPLAY_CLIMATE: _showClimate(data); break;
        case DISPLAY_STRESS:  _showStress(data); break;
        case DISPLAY_POWER:   _showPower(data); break;
        case DISPLAY_TANK:    _showTank(data); break;
        case DISPLAY_VALVE:   _showValve(data); break;
        case DISPLAY_SYSTEM:  _showSystem(data); break;
        case DISPLAY_INSIGHT: _showInsight(data); break;
        default: break;
    }
    
    _currentMode = (DisplayMode)(((int)_currentMode + 1) % DISPLAY_MODE_COUNT);
    _lastUpdate = now;
}

void LCDDisplay::setMode(DisplayMode mode) {
    _currentMode = mode;
}

bool LCDDisplay::isAvailable() {
    return _found;
}

void LCDDisplay::_showStatus(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    _lcd->print("Cycle #");
    _lcd->print(data.cycleCount);
    
    _lcd->setCursor(0, 1);
    if (data.hour < 10) _lcd->print("0");
    _lcd->print(data.hour);
    _lcd->print(":");
    if (data.minute < 10) _lcd->print("0");
    _lcd->print(data.minute);
    _lcd->print(" ");
    _lcd->print(data.extremeMode ? "EXTREME" : "NORMAL");
}

void LCDDisplay::_showClimate(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    _lcd->print("T:");
    _lcd->print(data.temperature, 1);
    _lcd->print("C H:");
    _lcd->print(data.humidity, 0);
    _lcd->print("%");
    
    _lcd->setCursor(0, 1);
    _lcd->print("VPD:");
    _lcd->print(data.vpd, 2);
    _lcd->print(" kPa");
}

void LCDDisplay::_showStress(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    _lcd->print("Strs:");
    _lcd->print((int)data.stress);
    _lcd->print("/");
    _lcd->print((int)data.stressMax);
    
    _lcd->setCursor(0, 1);
    _lcd->print("Dmg:");
    _lcd->print(data.damage, 2);
}

void LCDDisplay::_showPower(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    _lcd->print("Sol:");
    _lcd->print(data.solarVoltage, 1);
    _lcd->print("V");
    
    _lcd->setCursor(0, 1);
    _lcd->print("TEG:");
    _lcd->print(data.tegVoltage, 1);
    _lcd->print("V ");
    _lcd->print(data.powerOK ? "OK" : "LOW");
}

void LCDDisplay::_showTank(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    _lcd->print("Tank:");
    _lcd->print(data.tankLevel);
    _lcd->print("/");
    _lcd->print(data.tankMax);
    
    _lcd->setCursor(0, 1);
    if (data.isRaining) {
        _lcd->print("RAIN DETECTED!");
    } else if (data.conserveMode) {
        _lcd->print("CONSERVE MODE");
    } else {
        _lcd->print("Level OK");
    }
}

void LCDDisplay::_showValve(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    _lcd->print("State:");
    _lcd->print(data.isWatering ? "WATERING" : "IDLE");
    
    _lcd->setCursor(0, 1);
    _lcd->print("Opens:");
    _lcd->print(data.totalOpenings);
    _lcd->print(" T:");
    _lcd->print(data.totalTime);
}

void LCDDisplay::_showSystem(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    if (data.systemFailure) {
        _lcd->print("SYSTEM FAILURE!");
    } else if (data.sensorError) {
        _lcd->print("SENSOR ERROR");
    } else {
        _lcd->print("System: OK");
    }
    
    _lcd->setCursor(0, 1);
    _lcd->print("RAM:");
    _lcd->print(data.freeRAM);
    _lcd->print(" Err:");
    _lcd->print(data.errorCount);
}

void LCDDisplay::_showInsight(const DisplayData& data) {
    _lcd->setCursor(0, 0);
    
    if (strlen(data.insightText) > 16) {
        char temp[17];
        strncpy(temp, data.insightText, 16);
        temp[16] = '\0';
        _lcd->print(temp);
        
        _lcd->setCursor(0, 1);
        _lcd->print(&data.insightText[16]);
    } else {
        _lcd->print(data.insightText);
    }
}
