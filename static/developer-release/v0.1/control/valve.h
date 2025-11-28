
#ifndef VALVE_H
#define VALVE_H

#include <Servo.h>
#include "../config/pins.h"
#include "../config/settings.h"

class ValveController {
public:
  ValveController();
  
  void begin();
  void open();
  void close();
  void emergencyClose();
  
  bool isOpen();
  unsigned long getLastMoveTime();

private:
  Servo _servo;
  bool _isOpen;
  unsigned long _lastMoveTime;
  
  void _safeWrite(int angle);
};

#endif // VALVE is here.
