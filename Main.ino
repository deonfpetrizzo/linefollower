//Include class header files
#include <Arduino.h>
#include "LEDArray.h"
#include "QRD1114Array.h"
#include "PIDController.h"
#include "Button.h"
#include "Drivetrain.h"
#include "SerialReader.h"

//Arduino Nano outputs to L293D H-bridge motor driver
#define ENA 9
#define IN1 8
#define IN2 7
#define ENB 10
#define IN3 11
#define IN4 12

//Button constants
#define BUTTON_PIN 13
#define PRESS_DELAY 500

//Encoder constants
#define ENCODER_PIN A5
#define PULSES_PER_ROT 5
#define WHEEL_RADIUS 70

byte SENSOR_PINS[] = {A0, A1, A2, A3, A4}; //QRD1114 optical detector inputs to Arduino Nano
byte LED_PINS[] = {2, 3, 4, 5, 6}; //LED inputs to Arduino Nano

//Motor speed constants
#define INITIAL_SPEED 100
#define MIN_SPEED 0
#define MAX_SPEED 255
#define TURNING_SPEED 120

//Encoder linear distance (cm) constants
#define SMALL_STEP 1
#define MEDIUM_STEP 4
#define LARGE_STEP 5

//PID gain constants
#define KP 60
#define KI 2
#define KD 30

//Maze solving switch case constants
enum Intersection {
  FOLLOW_LINE,
  LEFT_TURN,
  RIGHT_TURN,
  COMPLETE_INTERSECTION,
  NO_LINE
};

//Array size constants
#define DATA_LEN 6
#define STR_LEN 7
#define PATH_LEN 100

//Global variables
int error = 0;
byte intersection = 0;
char path[PATH_LEN];
int pathLength = 0;
int pathIndex = 0;
byte passStatus = 0;
byte prevPos = 0;
byte pos = 0;
float data[DATA_LEN];
float prevData[DATA_LEN];
byte program = 0;

//Object instantiation
Button button(BUTTON_PIN);
LEDArray ledArray(LED_PINS);
Drivetrain drivetrain(
  ENA, IN1, IN2, ENB, IN3, IN4, MIN_SPEED, MAX_SPEED, INITIAL_SPEED,
  ENCODER_PIN, PULSES_PER_ROT, WHEEL_RADIUS
);
PIDController pidController(KP, KI, KD);
QRD1114Array senseArray(SENSOR_PINS, FOLLOW_LINE, LEFT_TURN, RIGHT_TURN, COMPLETE_INTERSECTION, NO_LINE);
SerialReader serialReader(DATA_LEN, STR_LEN);

void setup() {
  Serial.begin(9600);
  button.init();
  ledArray.init();
  drivetrain.init();
  senseArray.init();
}

void loop() {
  followLine(true);
}
