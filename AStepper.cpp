#include "gstepper.h"
#include <Arduino.h>

GStepper::GStepper(const unsigned long pulsePin, const unsigned long directionPin, const unsigned long sleepPin,
                unsigned long stepsPerRotate, unsigned long cropFactor, int plug)
{
    _direction = false;
    _plug = plug;
    _pulsePin = pulsePin;
    _directionPin = directionPin;
    _sleepPin = sleepPin;
    _stepsPerRotate = stepsPerRotate;
    _cropFactor = cropFactor;
    _period = 0;
    _stepsToDo = 0;
    _stepsDone = 0;
    _lastTime = 0;
    _stepsBack = 0;
}

void GStepper::begin()
{
    pinMode(_pulsePin, OUTPUT);
    pinMode(_directionPin, OUTPUT);
    pinMode(_sleepPin, OUTPUT);

    digitalWrite(_directionPin, LOW);
    off();
}

void GStepper::step()
{
    digitalWrite(_pulsePin, HIGH);
    digitalWrite(_pulsePin, LOW);
    _stepsDone++;
    _lastTime = micros();
}

void GStepper::stepWithoutRecord()
{
    digitalWrite(_pulsePin, HIGH);
    digitalWrite(_pulsePin, LOW);
}

void GStepper::rotateInfinit(int degreePerSec)
{
    if (degreePerSec == 0) return;
    if (degreePerSec < 0)
    {
        degreePerSec = -degreePerSec;
        digitalWrite(_directionPin, HIGH);
    } else
        digitalWrite(_directionPin, LOW);

    _stepsToDo = 4294967200;
    _stepsDone = 0;

    _period = 360000000 / degreePerSec / _stepsPerRotate / _cropFactor;

    if (_period < minPeriod) _period = minPeriod;
}

void GStepper::rotateAngle(int degree,unsigned int degreePerSec)
{
    if ((degree == 0) || (degreePerSec == 0)) return;
    if (degree < 0)
    {
        _direction = true;
        degree = -degree;
//        digitalWrite(_directionPin, _direction);
    } else
        _direction = false;

    digitalWrite(_directionPin, _direction);

    _stepsToDo = _stepsPerRotate * _cropFactor * degree / 360;
    _stepsDone = 0;

    _period = 360000000 / degreePerSec / _stepsPerRotate / _cropFactor;

    if (_period < minPeriod) _period = minPeriod;

    _stepsBack = 0;
}

void GStepper::rotateAngleVibro(int degree, unsigned int degreePerSec, unsigned int stepsBack)
{
    rotateAngle(degree, degreePerSec);

    _stepsBack = stepsBack;
}

void GStepper::check()
{
    if (_stepsToDo <= _stepsDone) return;

    if (micros() - _lastTime <= _period) return;
    
    if (_stepsBack && (_stepsDone % 3 == 0)) {

        digitalWrite(_directionPin, !_direction);
        delay(1);
        for (int i = 0; i < _stepsBack; i++) {
            delayMicroseconds(100);
            stepWithoutRecord();
            }
        digitalWrite(_directionPin, _direction);
        delay(1);
        for (int i = 0; i < _stepsBack; i++) {
            delayMicroseconds(100);
            stepWithoutRecord();
            }
        }
        step();
}

void GStepper::checkAndOff()
{

    if (_stepsToDo <= _stepsDone)
    {
        off();
        return;
    }

    check();
}

void GStepper::on()
{
    digitalWrite(_sleepPin, HIGH);
//    _lastTime = micros() + 1000;
}

void GStepper::off()
{
    digitalWrite(_sleepPin, LOW);
}

void GStepper::stop()
{
    _stepsToDo = 0;
    _stepsDone = 0;
}
