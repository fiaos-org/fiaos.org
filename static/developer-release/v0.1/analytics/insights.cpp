#include "insights.h"

InsightsGenerator::InsightsGenerator() {
    memset(_text, 0, sizeof(_text));
    _confidence = 70;
    _hasYesterday = false;
    _hasLastHour = false;
    memset(&_yesterday, 0, sizeof(ClimateSnapshot));
    memset(&_lastHour, 0, sizeof(ClimateSnapshot));
}

void InsightsGenerator::updateHistory(const ClimateSnapshot& current) {
}

void InsightsGenerator::newHour() {
    _hasLastHour = true;
}

void InsightsGenerator::newDay() {
    _hasYesterday = true;
}

void InsightsGenerator::generate(const ClimateSnapshot& current, float stress, float stressThreshold,
                                                                    bool waterLow, int tankLevel, bool extremeMode) {
    memset(_text, 0, sizeof(_text));
    _confidence = 85;
    
    if (_hasYesterday && _yesterday.temp > 0) {
        float tempDiff = current.temp - _yesterday.temp;
        if (abs(tempDiff) > 2) {
            if (tempDiff > 0) {
                snprintf(_text, 31, "%.1fC warmer than yest.", tempDiff);
            } else {
                snprintf(_text, 31, "%.1fC cooler than yest.", -tempDiff);
            }
            _confidence = 90;
            return;
        }
    }
    
    if (current.vpd > 2.0) {
        strcpy(_text, "HIGH VPD - High demand");
        _confidence = 95;
        return;
    } else if (current.vpd > 1.5) {
        strcpy(_text, "Elevated VPD");
        _confidence = 85;
        return;
    } else if (current.vpd < 0.5) {
        strcpy(_text, "Low VPD - Min demand");
        _confidence = 85;
        return;
    }
    
    if (waterLow) {
        snprintf(_text, 31, "Water low: %dml", tankLevel);
        _confidence = 95;
        return;
    }
    
    if (current.temp > 40) {
        strcpy(_text, "EXTREME HEAT");
        _confidence = 95;
        return;
    } else if (current.temp > 35 && current.humidity < 50) {
        strcpy(_text, "Hot & Dry");
        _confidence = 90;
        return;
    } else if (current.temp < 20 && current.humidity > 80) {
        strcpy(_text, "Cool & Humid");
        _confidence = 85;
        return;
    }
    
    if (stress > stressThreshold * 1.5) {
        strcpy(_text, "High stress level");
        _confidence = 90;
        return;
    }
    
    strcpy(_text, "Conditions stable");
    _confidence = 70;
}

const char* InsightsGenerator::getText() {
    return _text;
}

byte InsightsGenerator::getConfidence() {
    return _confidence;
}
