#include "patterns.h"

PatternTracker::PatternTracker() {
    for (byte i = 0; i < 24; i++) _hourlyEvents[i] = 0;
    for (byte i = 0; i < 3; i++) _topHours[i] = 255;
    _dayCount = 0;
    _weekCount = 0;
    _patternsAddr = PATTERNS_BASE_ADDR;
    _weeklyAddr = WEEKLY_GRAPH_ADDR;
}

void PatternTracker::recordEvent(byte hour) {
    if (hour < 24) {
        if (_hourlyEvents[hour] < 254) {
            _hourlyEvents[hour]++;
        }
    }
}

byte PatternTracker::getEventCount(byte hour) {
    if (hour < 24) {
        return _hourlyEvents[hour];
    }
    return 0;
}

void PatternTracker::endOfDay() {
    _updateTopHours();
    _dayCount++;
    
    for (byte i = 0; i < 24; i++) _hourlyEvents[i] = 0;
}

void PatternTracker::endOfWeek() {
    if (_dayCount >= 7) {
        _compressWeek();
        _dayCount = 0;
        _weekCount++;
    }
}

void PatternTracker::getTopHours(byte* hours) {
    for (byte i = 0; i < 3; i++) {
        hours[i] = _topHours[i];
    }
}

void PatternTracker::_updateTopHours() {
    byte temp[24];
    for (byte i = 0; i < 24; i++) temp[i] = _hourlyEvents[i];
    
    for (byte rank = 0; rank < 3; rank++) {
        byte maxIdx = 255, maxVal = 0;
        for (byte i = 0; i < 24; i++) {
            if (temp[i] > maxVal) {
                maxIdx = i;
                maxVal = temp[i];
            }
        }
        _topHours[rank] = (maxIdx < 24) ? maxIdx : 255;
        if (maxIdx < 24) temp[maxIdx] = 0;
    }
}

void PatternTracker::_storeDailyPattern(byte day) {
    if (_patternsAddr + 4 >= WEEKLY_GRAPH_ADDR) return;
    
    EEPROM.update(_patternsAddr++, day);
    for (byte i = 0; i < 3; i++) {
        EEPROM.update(_patternsAddr++, _topHours[i]);
    }
}

void PatternTracker::_compressWeek() {
    byte hourFreq[24] = {0};
    
    for (int i = _patternsAddr - 28; i < _patternsAddr; i += 4) {
        if (i >= PATTERNS_BASE_ADDR) {
            for (byte j = 1; j < 4; j++) {
                byte hour = EEPROM.read(i + j);
                if (hour < 24) {
                    hourFreq[hour]++;
                }
            }
        }
    }
    
    byte peakHour = 0, peakVal = 0;
    for (byte i = 0; i < 24; i++) {
        if (hourFreq[i] > peakVal) {
            peakVal = hourFreq[i];
            peakHour = i;
        }
    }
    
    if (_weeklyAddr + 4 < PATTERNS_MAX_ADDR) {
        EEPROM.update(_weeklyAddr++, _weekCount);
        EEPROM.update(_weeklyAddr++, peakHour);
        EEPROM.update(_weeklyAddr++, peakVal);
    }
}
