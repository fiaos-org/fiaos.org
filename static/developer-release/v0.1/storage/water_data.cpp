#include "water_data.h"

WaterStorage::WaterStorage() {
    _baseAddr = WATER_DATA_ADDR;
}

void WaterStorage::load(float& tankLevel, AllTimeIndex& index, DailyProgress& daily) {
    int addr = _baseAddr;
    
    int tankInt;
    EEPROM.get(addr, tankInt);
    addr += sizeof(int);
    
    if (tankInt >= 0 && tankInt <= MAX_WATER_CAPACITY) {
        tankLevel = tankInt;
    } else {
        tankLevel = MAX_WATER_CAPACITY;
    }
    
    EEPROM.get(addr, index);
    addr += sizeof(AllTimeIndex);
    
    EEPROM.get(addr, daily);
    
    Serial.print(F("[STORAGE] Loaded tank level: "));
    Serial.print((int)tankLevel);
    Serial.println(F("ml"));
}

void WaterStorage::save(float tankLevel, const AllTimeIndex& index, const DailyProgress& daily) {
    int addr = _baseAddr;
    
    int tankInt = (int)tankLevel;
    EEPROM.put(addr, tankInt);
    addr += sizeof(int);
    
    EEPROM.put(addr, index);
    addr += sizeof(AllTimeIndex);
    
    EEPROM.put(addr, daily);
    
    Serial.println(F("[STORAGE] Water data saved"));
}
