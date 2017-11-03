/*
 *  Halloween Pumpkin with Servo Eyes
 *  
 *  MCU:
 *   - Arduino Nano / Uno
 *  
 *  Servo: 
 *   - MG 90S Micro xNUM_EYES
 *   
 *  LED strip:
 *   - WS2812 xNUM_EYES
 *   
 *   Wiring:
 *     Arduino       Servo
 *      D2   <----->  Servo1 Signal 
 *      ...          ...
 *      D7   <----->  Servo6 Signal 
 *      
 *     Arduino       LED Strip
 *      D12  <----->  Signal
 * 
 *  Author:
 *   Vladimir Dobrikov <hedin.mail@gmail.com>
 */


#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define NUM_EYES 6  // Max 6 for Nano/Uno, 12 for Mega

#define PIN_1ST_SERVO 2
#define MAX_ANGLE 179
#define MIN_ANGLE 0
#define DELAY_AFTER_MOVE 100

#define PIN_PIXEL 12
#define NUMPIXELS 6
#define BRIGHTNESS_MAX 0xff
#define BRIGHTNESS_MIN 0x40
#define BRIGHTNESS_OFF 0x00
#define DELAY_FADE 20

Servo servos[NUM_EYES] = {};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN_PIXEL, NEO_GRB + NEO_KHZ800);

int randomIdx;

void setup() {
  Serial.begin(9600);
  delay(20);
  strip.begin();
  setAllPixels(BRIGHTNESS_MAX, BRIGHTNESS_OFF, BRIGHTNESS_OFF);

  randomSeed(analogRead(0));
}

void loop() {
  randomIdx = random(NUM_EYES); // Upper bound is exlusive
  Serial.print("Moving eye idx=");
  Serial.println(randomIdx);

  moveEye(randomIdx);
}

void moveEye(int idx) {
  setPixel(idx, BRIGHTNESS_MAX, BRIGHTNESS_OFF, BRIGHTNESS_OFF);
  moveServo(idx);
  setPixel(idx, BRIGHTNESS_MIN, BRIGHTNESS_OFF, BRIGHTNESS_OFF);
}

void moveServo(int i) {
  int angle = servos[i].read() <= MIN_ANGLE + 1 ? MAX_ANGLE : MIN_ANGLE;
  
  Serial.print("Moving servo. angle=");
  Serial.println(angle);
  
  if (!servos[i].attached()) {
    servos[i].attach(servoPinFromIdx(i));
  }
  servos[i].write(angle);
  delay(DELAY_AFTER_MOVE);
  servos[i].detach();
  
  Serial.println("Moving completed");
}

int servoPinFromIdx(int idx) {
  return PIN_1ST_SERVO + idx;
}

void setAllPixels(byte red, byte green, byte blue) {
  for(int i = 0; i < NUMPIXELS; i++ ) {
    setPixel(i, red, green, blue); 
  }
}

void setPixel(int idx, byte red, byte green, byte blue) {
  strip.setPixelColor(idx, strip.Color(red, green, blue));
  strip.show(); 
}


