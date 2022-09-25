#ifndef _PID_H_
#define _PID_H_

class pid
{
private:
    float kp;
    float ki;
    float kd;

    float p;
    float i;
    float d;
public:
    pid(float _kp, float _ki, float _kd){
        kp = _kp;
        ki = _ki;
        kd = _kd;
    }

    float calc(float _error, float _integral){
        p = kp * _error;
        return p;
    }

    float getPropotional(){
        return p;
    }

    float getIntegral(){
        return i;
    }

    float getDerivative(){
        return d;
    }
    ~pid();
};

#endif