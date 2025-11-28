#include "state.h"

StateStorage::StateStorage() {
}

bool StateStorage::load(SystemState& state) {
    SystemState temp;
    EEPROM.get(0, temp);
    uint16_t calcSum = _fletcher16((uint8_t*)&temp, sizeof(SystemState) - 2);
    if (temp.header == EEPROM_MAGIC && temp.checksum == calcSum) {
        state = temp;
        Serial.println(F("[STORAGE] State loaded successfully"));
        return true;
    }
    Serial.println(F("[STORAGE] EEPROM invalid/corrupt, resetting"));
    reset(state);
    return false;
}

void StateStorage::save(const SystemState& state) {
    SystemState toSave = state;
    toSave.header = EEPROM_MAGIC;
    toSave.version = DATA_VERSION;
    toSave.checksum = _fletcher16((uint8_t*)&toSave, sizeof(SystemState) - 2);
    EEPROM.put(0, toSave);
    Serial.println(F("[STORAGE] State saved"));
}

void StateStorage::reset(SystemState& state) {
    state.header = EEPROM_MAGIC;
    state.version = DATA_VERSION;
    state.metabolicStress = 0.0;
    state.accumDamage = 0.0;
    state.checksum = 0;
    save(state);
}

uint16_t StateStorage::_fletcher16(const uint8_t* data, size_t len) {
    uint16_t sum1 = 0, sum2 = 0;
    for (size_t i = 0; i < len; ++i) {
        sum1 = (sum1 + data[i]) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;
}
