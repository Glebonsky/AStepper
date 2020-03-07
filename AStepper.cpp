#include "AStepper.h"
#include <Arduino.h>

AStepper::AStepper(uint8_t pulsePin, 
                   uint8_t directionPin, 
                   uint8_t sleepPin,
                   uint64_t stepsPerRotate, 
                   uint64_t cropFactor, 
                   int8_t plug)
{
    _plug = plug;
    _pulsePin = pulsePin;
    _directionPin = directionPin;
    _sleepPin = sleepPin;
    _stepsPerRotate = stepsPerRotate;
    _cropFactor = cropFactor;
}

void AStepper::begin()
{
    pinMode(_pulsePin, OUTPUT);
    pinMode(_directionPin, OUTPUT);
    pinMode(_sleepPin, OUTPUT);

    digitalWrite(_directionPin, LOW);
    off();
}

void AStepper::step()
{
    digitalWrite(_pulsePin, HIGH);
    digitalWrite(_pulsePin, LOW);
    _stepsDone++;
    _lastTime = micros();
}

void AStepper::stepWithoutRecord()
{
    digitalWrite(_pulsePin, HIGH);
    digitalWrite(_pulsePin, LOW);
}

void AStepper::rotateInfinit(int degreePerSec)
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

void AStepper::rotateAngle(int degree,unsigned int degreePerSec)
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

void AStepper::rotateAngleVibro(int degree, unsigned int degreePerSec, unsigned int stepsBack)
{
    rotateAngle(degree, degreePerSec);

    _stepsBack = stepsBack;
}

///put it in "loop" function
void AStepper::check()
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

void AStepper::checkAndOff()
{

    if (_stepsToDo <= _stepsDone)
    {
        off();
        return;
    }

    check();
}

void AStepper::on()
{
    digitalWrite(_sleepPin, HIGH);
//    _lastTime = micros() + 1000;
}

void AStepper::off()
{
    digitalWrite(_sleepPin, LOW);
}

void AStepper::stop()
{
    _stepsToDo = 0;
    _stepsDone = 0;
}
