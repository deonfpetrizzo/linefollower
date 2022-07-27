#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button {
  private:
    byte buttonPin;

  public:
    Button(byte buttonPin) {
      this->buttonPin = buttonPin;
    }

    void init() {
      pinMode(buttonPin, INPUT);
    }
    
    //Reads the button and returns a boolean value
    bool isPressed() {
      return digitalRead(buttonPin);
    }

    //Returns true if the button is pressed for a certain duration
    bool isLongPressed(int pressDuration) {
      static unsigned long pressTime = 0;
      static bool isPressing = false;
      static bool isLongPress = false;
      static bool prevState = false;
      bool currState = isPressed();

      if (!prevState && currState) {
        pressTime = millis();
        isPressing = true;
      }
      else if ((prevState && !currState) || isLongPress) {
        isPressing = false;
        isLongPress = false;
      }

      if (isPressing && millis() - pressTime >= pressDuration) isLongPress = true;

      prevState = currState;
      return isLongPress;
    }

    //Returns the number of successive button presses within a specified time interval
    int handleSuccessivePresses(int interval) {
      static byte numPresses = 0;
      static unsigned long debounceDelay = 50;
      static unsigned long debounceTime = 0;
      static unsigned long firstPressTime = 0;
      static bool currState = false;
      static bool prevState = false;
      static bool prevReading = false;
      bool reading = isPressed();

      if (reading != prevReading) debounceTime = millis();
      if (millis() - debounceTime > debounceDelay) {
        currState = reading;
        if (currState && !prevState) {
          numPresses++;
          if (numPresses == 1) firstPressTime = millis();
        }
      }
      
      prevReading = reading;
      prevState = currState;
      
      if (millis() - firstPressTime > interval) {
        byte tempNumPresses = numPresses;
        numPresses = 0;
        return tempNumPresses;
      }
      return -1;
    }

    //Tests the handleSuccessivePresses method
    void debugSuccessivePresses() {
      int numPresses = handleSuccessivePresses(2000);
      switch (numPresses) {
        case 1:
          Serial.println("Single click");
          break;
        case 2:
          Serial.println("Double click");
          break;
        case 3:
          Serial.println("Triple click");
          break;
        case 4: 
          Serial.println("Quadruple click");
          break;
      }
    }
};

#endif
