#ifndef BoxJointJig_h
#define BoxJointJig_h

#include <Arduino.h>

class BoxJointJig
{
public:
  BoxJointJig(int pul_pin, int dir_pin, int max_speed, float mmPerRev, float stepsPerRev);
  void step(int step);
  void stepSteps(int steps);
  void stepMm(float mm);
  void setPosition(int step);
  long getPosition();
  void goToPosition(long position);
  long mmToSteps(float mm);
  float stepsToMm(unsigned long steps);
  float c0, d;

private:
  int _pul_pin;
  int _dir_pin;
  unsigned int _c0;
  float _d;
  int _max_speed;
  long _position;
  long _accel_position;
  long _ramp_up_step_count;
  float _mmPerRev;
  float _stepsPerRev;
};

#endif
