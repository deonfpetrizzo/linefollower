#ifndef SENSOR_H
#define SENSOR_H
#include <Arduino.h>

class QRD1114Array {
  private:
    byte *sensorPins;
    byte followLine;
    byte leftTurn;
    byte rightTurn;
    byte completeIntersection;
    byte noLine;

    byte pos = 0;

  public:
    QRD1114Array(byte *sensorPins, byte followLine, byte leftTurn, byte rightTurn, byte completeIntersection, byte noLine) {
      this->sensorPins = sensorPins;
      this->followLine = followLine;
      this->leftTurn = leftTurn;
      this->rightTurn = rightTurn;
      this->completeIntersection = completeIntersection;
      this->noLine = noLine;
    }
    
    void init() {
      for (byte i = 0; i < 5; i++) pinMode(sensorPins[i], INPUT);
    }
    
    //Calculates and returns the error (distance from setpoint)
    int getError() {
      int err = 0;
      pos = getPosition();
      switch (pos) {
        case B10000:
          err = -4;
          break;
        case B11000:
          err = -3;
          break;
        case B01000:
          err = -2;
          break;
        case B01100:
          err = -1;
          break;
        case B00100:
          err = 0;
          break;
        case B00110:
          err = 1;
          break;
        case B00010:
          err = 2;
          break;
        case B00011:
          err = 3;
          break;
        case B00001:
          err = 4;
          break;
        default:
          err = 5;
          break;
      }
      return err;
    }

    //Returns an integer value based on all the possible intersections the sensor can detect
    byte checkIntersection() {
      byte intersection;
      pos = getPosition();
      switch (pos) {
        case B11111:
          intersection = completeIntersection;
          break;
        case B00000:
          intersection = noLine;
          break;
        case B11100:
        case B11110:
          intersection = leftTurn;
          break;
        case B00111:
        case B01111:
          intersection = rightTurn;
          break;
        default:
          intersection = followLine;
          break;
      }
      return intersection;
    }
    
    //Reads all sensors and returns a binary representation of the robot's position on the line
    byte getPosition() {
      byte pos = 0;
      for (byte i = 0; i < 5; i++) 
        if (digitalRead(sensorPins[i])) 
          pos += (1 << i); //Build the binary number by left shifting 1 by the index of any sensor with a HIGH state
      return pos;
    }

    //Displays the state of each sensor
    void debug() {
      static byte readings[5];
      static byte prevReadings[5];
      for (byte i = 0; i < 5; i++) {
        readings[i] = digitalRead(sensorPins[i]);
        if (readings[i] != prevReadings[i]) {
          for (byte j = 0; j < 5; j++) {
            Serial.print(readings[j]);
            Serial.print("  ");
          }
          Serial.println();
        }
        prevReadings[i] = readings[i];
      }
    }
};

#endif
