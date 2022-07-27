#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "Encoder.h"

class Drivetrain {
  private:
    byte enA;
    byte in1;
    byte in2;
    byte enB;
    byte in3;
    byte in4;
    byte minSpeed;
    byte maxSpeed;
    byte initialSpeed;

    Encoder encoder;
    byte encoderPin;
    byte ticsPerRot;
    byte wheelRadius;

  public:
    Drivetrain(byte enA, byte in1, byte in2, byte enB, byte in3, byte in4, byte minSpeed, byte maxSpeed, byte initialSpeed, 
               byte encoderPin, byte ticsPerRot, byte wheelRadius) : encoder(encoderPin, ticsPerRot, wheelRadius) {
      this->enA = enA;
      this->in1 = in1;
      this->in2 = in2;
      this->enB = enB;
      this->in3 = in3;
      this->in4 = in4;

      this->minSpeed = minSpeed;
      this->maxSpeed = maxSpeed;
      this->initialSpeed = initialSpeed;

      this->encoderPin = encoderPin;
      this->ticsPerRot = ticsPerRot;
      this->wheelRadius = wheelRadius;
    }

    void init() {
      pinMode(enA, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(enB, OUTPUT);
      pinMode(in3, OUTPUT);
      pinMode(in4, OUTPUT);

      encoder.init();
    }
    
    //Controls the motors based on PID values generated from the IR sensor array's readings
    void followLine(float pidVal) {
      byte spdR = initialSpeed + pidVal;
      byte spdL = initialSpeed - pidVal;
      set(spdR, spdL, 1, 1);
    }
    
    //Controls the motors based on x and y values sent through serial
    void runJsControl(float xAxis, float yAxis) {
      byte spdR, spdL, xMapped;
      bool dir;
      if (yAxis < 470) {
        spdR = map(yAxis, 470, 0, minSpeed, maxSpeed);
        spdL = map(yAxis, 470, 0, minSpeed, maxSpeed);
        dir = 0;
      }
      else if (yAxis > 550) {
        spdR = map(yAxis, 550, 1023, minSpeed, maxSpeed);
        spdL = map(yAxis, 550, 1023, minSpeed, maxSpeed);
        dir = 1;
      }
      else {
        spdR = 0;
        spdL = 0;
      }
      
      if (xAxis > 550) {
        xMapped = map(xAxis, 550, 1023, minSpeed, maxSpeed);
        spdR += xMapped;
        spdL -= xMapped;
      }
      else if (xAxis < 470) {
        xMapped = map(xAxis, 470, 0, minSpeed, maxSpeed);
        spdR -= xMapped;
        spdL += xMapped;
      }
      set(spdR, spdL, dir, dir);
    }

    //Drives each motor by a specified distance and direction
    void drive(int targetDist, char dir) {
      byte dirR = dir == 'L' ? 1 : dir == 'S' ? 1 : 0;
      byte dirL = dir == 'L' ? 0 : dir == 'S' ? 1 : 1;
      
      if (targetDist == -1)
        set(initialSpeed, initialSpeed, dirR, dirL);
      else {
        while (encoder.pulsesToDist(encoder.cntPulses()) < targetDist)
          set(initialSpeed, initialSpeed, dirR, dirL);

        set(0, 0, 0, 0);
        encoder.setNumPulses(0);
      }
    }

    void stop() {
      set(0, 0, 0, 0);
      delay(250);
    }
    
    //Sets the rotational speed and direction of the right and left motors
    void set(byte spdR, byte spdL, byte dirR, byte dirL) {
      analogWrite(enA, constrain(spdR, minSpeed, maxSpeed));
      analogWrite(enB, constrain(spdL, minSpeed, maxSpeed));
      //If 1, rotate clockwise; otherwise, rotate counterclockwise
      digitalWrite(in1, dirL);
      digitalWrite(in2, !dirL);
      digitalWrite(in3, dirR);
      digitalWrite(in4, !dirR);
    }

    //Sets the max speed
    void setSpeed(byte maxSpeed) {
      this->maxSpeed = maxSpeed;
    }
};

#endif
