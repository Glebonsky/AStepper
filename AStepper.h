class AStepper
{
public:
    AStepper(const unsigned long pulsePin, 
             const unsigned long directionPin, 
             const unsigned long sleepPin,
             unsigned long stepsPerRotate = 200, 
             unsigned long cropFactor = 16, 
             int plug = 1);
             
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
    const unsigned long minPeriod = 100;
    int _plug;
    bool _direction = false;
    unsigned long _pulsePin;
    unsigned long _directionPin;
    unsigned long _sleepPin;
    unsigned long _stepsPerRotate;
    unsigned long _cropFactor;
    unsigned long _period = 0;
    unsigned long _stepsToDo = 0;
    unsigned long _stepsDone = 0;
    unsigned long _stepsBack = 0;
    unsigned long _lastTime = 0;
    void step();
    void stepWithoutRecord();
};

