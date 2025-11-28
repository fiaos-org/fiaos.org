#ifndef PATTERNS_H
#define PATTERNS_H

#include <Arduino.h>
#include <EEPROM.h>

#define PATTERNS_BASE_ADDR 800
#define WEEKLY_GRAPH_ADDR  1000
#define PATTERNS_MAX_ADDR  1200

class PatternTracker {
public:
    PatternTracker();
    
    void recordEvent(byte hour);
    
    byte getEventCount(byte hour);
    
    void endOfDay();
    
    void endOfWeek();
    
    void getTopHours(byte* hours);

private:
    byte _hourlyEvents[24];
    byte _topHours[3];
    byte _dayCount;
    byte _weekCount;
    int _patternsAddr;
    int _weeklyAddr;
    
    void _updateTopHours();
    void _storeDailyPattern(byte day);
    void _compressWeek();
};

#endif
