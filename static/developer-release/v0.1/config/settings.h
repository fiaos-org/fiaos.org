#ifndef SETTINGS_H
#define SETTINGS_H

const float ATM_PRESSURE_HPA = 1013.25;
const float SYSTEM_ALTITUDE = 15.0;
const float SYSTEM_LATITUDE = 7.0;

#define MAX_WATER_CAPACITY          6000
#define LOW_WATER_THRESHOLD         1500
#define FLOW_RATE_ML_PER_SEC        20.0

#define RAIN_DETECTION_THRESHOLD    85
#define RAIN_DETECTION_TIME         300000
#define RAIN_WATER_ADD              4200

#define SENSOR_READ_INTERVAL        1000
#define LCD_UPDATE_INTERVAL         3000
#define SERVO_MOVE_DELAY            2000
#define STATE_SAVE_INTERVAL         600
#define STATUS_PRINT_INTERVAL       30

#define MIN_IRRIGATION_DURATION     2
#define MAX_IRRIGATION_DURATION     15

#define MAX_ERROR_TOLERANCE         3
#define WATCHDOG_TIMEOUT            8000

const float EXTREME_MODE_MULTIPLIER = 0.5;
const float EXTREME_VPD_THRESHOLD = 2.5;
const float EXTREME_TEMP_THRESHOLD = 38.0;

const byte ESSENTIAL_HOURS[] = {9, 13, 17, 19};
#define NUM_ESSENTIAL_HOURS 4

#endif // SETTINGS_H
