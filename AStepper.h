#include <stdint.h>

class AStepper
{
public:
    AStepper(uint8_t pulsePin, 
             uint8_t directionPin, 
             uint8_t sleepPin,
             uint64_t stepsPerRotate = 200, 
             uint64_t cropFactor = 16, 
             int8_t plug = 1);
             
    void begin();
    void rotateInfinit(int degreePerSec);
    void rotateAngle(int degree, unsigned int degreePerSec);
    void rotateAngleVibro(int degree, unsigned int degreePerSec, unsigned int stepsBack);
    void check();
    void checkAndOff();
    void on();
    void off();
    void stop();

private:
    const uint64_t minPeriod = 100;
    int8_t _plug;
    bool _direction = false;
    uint8_t _pulsePin;
    uint8_t _directionPin;
    uint8_t _sleepPin;
    uint32_t _stepsPerRotate;
    uint32_t _cropFactor;
    uint64_t _period = 0;
    uint64_t _stepsToDo = 0;
    uint64_t _stepsDone = 0;
    uint64_t _stepsBack = 0;
    uint64_t _lastTime = 0;
    void step();
    void stepWithoutRecord();
};

