#include "valve.h"

ValveController::ValveController() {
    _isOpen = false;
    _lastMoveTime = 0;
}

void ValveController::begin() {
    _servo.attach(PIN_SERVO);
    _lastMoveTime = millis();
    _safeWrite(0);
    Serial.println(F("[VALVE] Initialized"));
}

void ValveController::open() {
    _safeWrite(90);
    digitalWrite(PIN_LED_STATUS, HIGH);
    Serial.println(F("[VALVE] Opened"));
}

void ValveController::close() {
    _safeWrite(0);
    digitalWrite(PIN_LED_STATUS, LOW);
    Serial.println(F("[VALVE] Closed"));
}

void ValveController::emergencyClose() {
    if (_isOpen) {
        Serial.println(F("[VALVE] Emergency close"));
        _servo.write(0);
        _isOpen = false;
        digitalWrite(PIN_LED_STATUS, LOW);
    }
}

bool ValveController::isOpen() {
    return _isOpen;
}

unsigned long ValveController::getLastMoveTime() {
    return _lastMoveTime;
}

void ValveController::_safeWrite(int angle) {
    unsigned long now = millis();
    if (now - _lastMoveTime < SERVO_MOVE_DELAY) {
        delay(SERVO_MOVE_DELAY - (now - _lastMoveTime));
    }
    
    _servo.write(angle);
    _lastMoveTime = millis();
    _isOpen = (angle > 10);
}
