#ifndef INSIGHTS_H
#define INSIGHTS_H

#include <Arduino.h>
#include "../config/settings.h"
#include "../config/model.h"

struct ClimateSnapshot {
    float temp;
    float humidity;
    float vpd;
    float solar;
};

class InsightsGenerator {
public:
    InsightsGenerator();
    
    void updateHistory(const ClimateSnapshot& current);
    void newHour();
    void newDay();
    
    void generate(const ClimateSnapshot& current, float stress, float stressThreshold,
                                bool waterLow, int tankLevel, bool extremeMode);
    
    const char* getText();
    
    byte getConfidence();

private:
    char _text[32];
    byte _confidence;
    ClimateSnapshot _yesterday;
    ClimateSnapshot _lastHour;
    bool _hasYesterday;
    bool _hasLastHour;
};

#endif
