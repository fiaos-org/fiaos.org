#include "irrigation.h"

IrrigationController::IrrigationController(ValveController* valve) {
    _valve = valve;
    _state = STATE_IDLE;
    _startTime = 0;
    _targetDuration = 0;
    _totalOpenings = 0;
    _totalWateringTime = 0;
    _lastRecoveryTick = 0;
}

void IrrigationController::begin() {
    _state = STATE_IDLE;
    _lastRecoveryTick = millis();
}

void IrrigationController::update(unsigned long currentTime) {
    if (_state != STATE_IRRIGATING) return;
    
    unsigned long elapsed = currentTime - _startTime;
    
    _lastRecoveryTick = currentTime;
    
    if (elapsed >= (_targetDuration * 1000UL)) {
        _valve->close();
        _state = STATE_IDLE;
        _totalWateringTime += _targetDuration;
        
        Serial.print(F("[IRRIGATION] Complete. Duration: "));
        Serial.print(_targetDuration);
        Serial.println(F("s"));
    }
}

bool IrrigationController::start(int duration) {
    if (_state != STATE_IDLE) return false;
    
    _targetDuration = constrain(duration, MIN_IRRIGATION_DURATION, MAX_IRRIGATION_DURATION);
    _startTime = millis();
    _state = STATE_IRRIGATING;
    _lastRecoveryTick = millis();
    
    _valve->open();
    _totalOpenings++;
    
    Serial.print(F("[IRRIGATION] Started for "));
    Serial.print(_targetDuration);
    Serial.println(F("s"));
    
    return true;
}

bool IrrigationController::shouldTrigger(float stress, float threshold) {
    return (stress > threshold);
}

int IrrigationController::calculateDuration(float stress, float optimalThreshold, float recoveryRate) {
    float stressToRemove = stress - optimalThreshold;
    float secondsRequired = stressToRemove / recoveryRate;
    return constrain((int)secondsRequired, MIN_IRRIGATION_DURATION, MAX_IRRIGATION_DURATION);
}

int IrrigationController::applyAdjustments(int duration, bool extremeMode, bool conserveMode, bool lowPower, bool essentialTime) {
    float adjusted = duration;
    
    if (extremeMode) {
        adjusted *= EXTREME_MODE_MULTIPLIER;
    }
    
    if (lowPower && !essentialTime) {
        adjusted *= 0.6;
    }
    
    if (conserveMode) {
        if (essentialTime) {
            adjusted *= 0.8;
        } else {
            adjusted *= 0.5;
        }
    }
    
    return constrain((int)adjusted, MIN_IRRIGATION_DURATION, MAX_IRRIGATION_DURATION);
}

IrrigationState IrrigationController::getState() {
    return _state;
}

int IrrigationController::getTargetDuration() {
    return _targetDuration;
}

unsigned int IrrigationController::getTotalOpenings() {
    return _totalOpenings;
}

unsigned long IrrigationController::getTotalWateringTime() {
    return _totalWateringTime;
}

float IrrigationController::getRecoveryPerTick(float dt_sec, float recoveryRate) {
    if (_state != STATE_IRRIGATING) return 0;
    return recoveryRate * dt_sec;
}
