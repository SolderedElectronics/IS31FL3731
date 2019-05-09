#include "IS31FL3731.h"
#define NUMBER_OF_DRIVERS 4
#define X_SIZE NUMBER_OF_DRIVERS*16
#define BRIGHTNESS 64
IS31FL3731 led(NUMBER_OF_DRIVERS);

void setup() {
  led.begin();
  led.setTextColor(BRIGHTNESS, 0);
  led.print("Hello");
  led.display();
  delay(1500);
  led.clearDisplay();
  led.print("world");
  led.display();
  delay(1500);
  led.clearDisplay();
}

void loop() {
  led.clearDisplay();
  for(int i = 0; i<X_SIZE; i++) {
    led.drawLine(0, 0, i, 9, BRIGHTNESS);
    delay(75);
    led.display();
  }
  delay(1000);

  led.clearDisplay();
  for(int i = 0; i<X_SIZE; i++) {
    led.drawFastVLine(i, 0, 9, BRIGHTNESS);
    delay(75);
    led.display();
  }
  delay(1000);

  led.clearDisplay();
  for(int i = 0; i<9; i++) {
    led.drawFastHLine(0, i, X_SIZE, BRIGHTNESS);
    delay(75);
    led.display();
  }
  delay(1000);

  led.clearDisplay();
  led.drawRect(X_SIZE/2-7, 0, 14, 9, BRIGHTNESS);
  led.display();
  delay(1000);

  led.fillRect(X_SIZE/2-5, 2, 10, 5, BRIGHTNESS);
  led.display();
  delay(1000);

  led.clearDisplay();
  led.drawRoundRect(X_SIZE/2-7, 0, 14, 9, 2, BRIGHTNESS);
  led.display();
  delay(1000);

  led.fillRoundRect(X_SIZE/2-5, 2, 10, 5, 2, BRIGHTNESS);
  led.display();
  delay(1000);

  led.clearDisplay();
  led.drawCircle(X_SIZE/2-4, 4, 4, BRIGHTNESS);
  led.display();
  delay(1000);

  led.fillCircle(X_SIZE/2-4, 4, 2, BRIGHTNESS);
  led.display();
  delay(1000);

  for(int i = 0; i<256; i++) {
    led.clearDisplay();
    led.print("Text");
    led.setTextColor(255-i, i);
    led.display();
    delay(10);
  }
  delay(1000);
  led.setTextColor(BRIGHTNESS, 0);
  led.clearDisplay();
  led.print("OFF!");
  led.display();
  delay(1500);
  led.shutdown();
  led.clearDisplay();
  led.print("ON!");
  led.display();
  delay(1500);
  led.powerUp();
  delay(1000);

  for(int i = 0; i<5; i++) {
    led.clearDisplay();
    led.setRotation(i);
    led.print("Rotation");
    led.display();
    delay(500);
  }
}
