#ifndef LED_H
#define LED_H
#include <Arduino.h>

class LEDArray {
  private:
    byte *ledPins;
     
  public:
    LEDArray(byte *ledPins) {
      this->ledPins = ledPins;
    }

    void init() {
      for (byte i = 0; i < 5; i++) pinMode(ledPins[i], OUTPUT);
    }

    //Turns on/off a specific LED
    void changeState(byte ledNum, byte ledState) {
      digitalWrite(ledPins[ledNum], ledState);
    }

    //Turns on/off all LEDs
    void changeAllStates(byte ledState) {
      for (byte i = 0; i < 5; i++)
        changeState(i, ledState);
    }

    //Turns on LEDs corresponding to optical sensor states
    void indicatePosition(byte pos) {
      for (byte i = 5; i >= 0; i--) {
        byte state = bitRead(pos, i) == 1 ? HIGH : LOW;
        changeState(i, state);
      }
    }

    //Turns a specified number of LEDs on successively
    void fill(int fillDelay, byte upperBound) {
      for (byte i = 0; i <= upperBound; i++) {
        changeState(i, HIGH);
        delay(fillDelay);
      }
    }

    //Blinks all LEDs using millis
    void blink(int blinkInterval) {
      static unsigned long prevMillis = 0;
      static byte currentState = LOW;
      if (millis() - previousMillis >= blinkInterval) {
        previousMillis = millis();
        currentState = !currentState;
        changeAllStates(currentState);
      }
    }

    //Turns on LEDs incrementally then turns off LEDs incrementally by n iterations
    void stretch(int stretchDelay, byte numIterations) {
      for (byte i = 0; i < numIterations; i++) {
        for (byte j = 0; j < 5; j++) {
          changeState(j, HIGH);
          delay(stretchDelay);
        }
        for (byte k = 4; k >= 0; k--) {
          changeState(k, LOW);
          delay(stretchDelay);
        }
      }
    }

    //Turns on individual LEDs incrementally while turning all other leds off 
    void walk(int cntDelay, byte numIterations, bool bounceBack) {
      for (byte i = 0; i < numIterations; i++) {
        for (byte j = 0; j < 5; j++) {
          changeState(j, HIGH);
          if (j > 0) changeState(j - 1, LOW);
          delay(cntDelay);
        }
        if (bounceBack) {
          for (byte j = 3; j > 0; j--) {
            changeState(j, HIGH);
            if (j < 4) changeState(j + 1, LOW);
            delay(cntDelay);
          }
        }
        changeAllStates(LOW);
      }
    }

    //Creates a pulse effect, extending and retracting on either side of the center LED
    void pulse(int cntDelay) {
      static unsigned long prevMillis = 0;
      static byte cnt = 0;
      
      if (millis() - previousMillis >= cntDelay) {
        previousMillis = millis();
        cnt = cnt == 5 ? 0 : ++cnt;
      }

      switch (cnt) {
        case 0:
          changeState(2, HIGH);
          break;
        case 1:
          changeState(1, HIGH);
          changeState(3, HIGH);
          break;
        case 2:
          changeState(0, HIGH);
          changeState(4, HIGH);
          break;
        case 3:
          changeState(0, LOW);
          changeState(4, LOW);
          break;
        case 4:
          changeState(1, LOW);
          changeState(3, LOW);
          break;
        case 5:
          changeState(2, LOW);
          break;
        }
    }
};

#endif
