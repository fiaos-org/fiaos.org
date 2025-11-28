#ifndef STATE_STORAGE_H
#define STATE_STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_MAGIC 0xCAFEBABE
#define DATA_VERSION 2

struct SystemState {
    uint32_t header;
    uint8_t version;
    float metabolicStress;
    float accumDamage;
    uint16_t checksum;
};

class StateStorage {
public:
    StateStorage();
    bool load(SystemState& state);
    void save(const SystemState& state);
    void reset(SystemState& state);

private:
    static uint16_t _fletcher16(const uint8_t* data, size_t len);
};

#endif
