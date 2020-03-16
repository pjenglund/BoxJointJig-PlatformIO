#include "BoxJointJig.h"
#include <Arduino.h>

BoxJointJig::BoxJointJig(int pul_pin, int dir_pin, int max_speed, float mmPerRev, float stepsPerRev)
{
  _dir_pin = dir_pin;
  _pul_pin = pul_pin;
  _max_speed = max_speed;
  _mmPerRev = mmPerRev;
  _stepsPerRev = stepsPerRev;
  _position = 0;
  pinMode(_pul_pin, OUTPUT);
  pinMode(_dir_pin, OUTPUT);
  digitalWrite(_dir_pin, LOW);
}

void BoxJointJig::step(int step)
{
  // determine direction
  int x = 0;
  x = step < 0 ? LOW : HIGH;
  digitalWrite(_dir_pin, x);

  // do the step
  digitalWrite(_pul_pin, HIGH);
  digitalWrite(_pul_pin, LOW);

  // set the position according to direction
  _position = step < 0 ? _position += 1 : _position -= 1;
}

void BoxJointJig::stepSteps(int steps)
{
  //determine direction
  int x = 0;
  x = steps > 0 ? HIGH : LOW;
  digitalWrite(_dir_pin, x);

  // do the steps
  for (int i = 1; i <= abs(steps); i++)
  {
    digitalWrite(_pul_pin, HIGH);
    digitalWrite(_pul_pin, LOW);
    // update position. if steps positive, add one step. if steps negative, subtract one step
    _position = steps > 0 ? _position += 1 : _position -= 1;
    delayMicroseconds(_max_speed);
  }
}

void BoxJointJig::stepMm(float mm)
{
  this->stepSteps(this->mmToSteps(mm));
}

void BoxJointJig::setPosition(int pos)
{
  _position = +pos;
}

long BoxJointJig::getPosition()
{
  return _position;
}

void BoxJointJig::goToPosition(long position)
{
  int x = 0;
  x = (position - this->getPosition() > 0) ? LOW : HIGH;
  if (x != 0)
    digitalWrite(_dir_pin, x);
  while (position != this->getPosition())
  {
    this->step(position - this->getPosition());
    delayMicroseconds(_max_speed);
  }
}

long BoxJointJig::mmToSteps(float mm)
{
  float temp;
  temp = float(this->_mmPerRev) / float(this->_stepsPerRev);
  temp = mm / temp;
  return temp;
}

float BoxJointJig::stepsToMm(unsigned long steps)
{
  float temp;
  temp = float(this->_mmPerRev) / float(this->_stepsPerRev) * float(steps);
  return temp;
}
