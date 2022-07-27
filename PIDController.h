#ifndef PID_H
#define PID_H
#include <Arduino.h>

class PIDController {
  private:
    float kp;
    float ki;
    float kd;
    float cumulativeError, previousError;
    float rateError;

  public:
    PIDController(float kp, float ki, float kd) {
      this->kp = kp;
      this->ki = ki;
      this->kd = kd;
    }

    //Calculates and returns PID value
    float calculate(float error) {
      cumulativeError = constrain(cumulativeError + error, -50, 50); //Cumulative error over time
      rateError = error - previousError; //Rate of change of the error
      previousError = error; //Update previous error
      return kp * error + ki * cumulativeError + kd * rateError; //Output tuned by manipulating gain constants
    }

    //Setters:
    void setKp(float kp) {
      this->kp = kp;
    }

    void setKi(float ki) {
      this->ki = ki;
    }

    void setKd(float kd) {
      this->kd = kd;
    }
};

#endif
