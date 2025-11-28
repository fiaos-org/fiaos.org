#ifndef IRRIGATION_H
#define IRRIGATION_H

#include <Arduino.h>
#include "valve.h"
#include "../config/settings.h"
#include "../config/model.h"

enum IrrigationState { 
    STATE_IDLE, 
    STATE_IRRIGATING 
};

class IrrigationController {
public:
    IrrigationController(ValveController* valve);
    
    void begin();
    void update(unsigned long currentTime);
    
    bool start(int duration);
    bool shouldTrigger(float stress, float threshold);
    int calculateDuration(float stress, float optimalThreshold, float recoveryRate);
    int applyAdjustments(int duration, bool extremeMode, bool conserveMode, bool lowPower, bool essentialTime);
    
    IrrigationState getState();
    int getTargetDuration();
    unsigned int getTotalOpenings();
    unsigned long getTotalWateringTime();
    float getRecoveryPerTick(float dt_sec, float recoveryRate);

private:
    ValveController* _valve;
    IrrigationState _state;
    unsigned long _startTime;
    int _targetDuration;
    unsigned int _totalOpenings;
    unsigned long _totalWateringTime;
    unsigned long _lastRecoveryTick;
};

#endif // IRRIGATION_H
