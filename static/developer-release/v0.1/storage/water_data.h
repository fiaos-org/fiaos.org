#ifndef WATER_STORAGE_H
#define WATER_STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "../config/settings.h"

#define MAX_DAILY_OPENINGS 50
#define MAX_HISTORY_DAYS 30
#define WATER_DATA_ADDR 2000

struct DailyProgress {
    int openingDurations[MAX_DAILY_OPENINGS];
    int openingCount;
    int totalWaterUsed;
    byte date;
    byte month;
};

struct AllTimeIndex {
    DailyProgress days[MAX_HISTORY_DAYS];
    int currentDayIndex;
    long totalLifetimeUsage;
};

class WaterStorage {
public:
    WaterStorage();
    
    void load(float& tankLevel, AllTimeIndex& index, DailyProgress& daily);
    void save(float tankLevel, const AllTimeIndex& index, const DailyProgress& daily);

private:
    int _baseAddr;
};

#endif // WATER_STORAGE_H
