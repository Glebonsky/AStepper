class GStepper
{
    const unsigned long minPeriod = 100;
    int _plug;
    bool _direction;
    unsigned long _pulsePin;
    unsigned long _directionPin;
    unsigned long _sleepPin;
    unsigned long _stepsPerRotate;
    unsigned long _cropFactor;
    unsigned long _period;
    unsigned long _stepsToDo;
    unsigned long _stepsDone;
    unsigned long _stepsBack;
    unsigned long _lastTime;
    void step();
    void stepWithoutRecord();

public:
    GStepper(const unsigned long pulsePin, const unsigned long directionPin, const unsigned long sleepPin,
                unsigned long stepsPerRotate = 200, unsigned long cropFactor = 16, int plug = 1);
    void begin();
    void rotateInfinit(int degreePerSec);
    void rotateAngle(int degree, unsigned int degreePerSec);
    void rotateAngleVibro(int degree, unsigned int degreePerSec, unsigned int stepsBack);
    void check();
    void checkAndOff();
    void on();
    void off();
    void stop();
};

