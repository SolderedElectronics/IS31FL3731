#ifndef _IS31FL3731_H_
#define _IS31FL3731_H_

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#define ISSI_REG_CONFIG  0x00
#define ISSI_REG_CONFIG_PICTUREMODE 0x00
#define ISSI_REG_CONFIG_AUTOPLAYMODE 0x08
#define ISSI_REG_CONFIG_AUDIOPLAYMODE 0x18

#define ISSI_CONF_PICTUREMODE 0x00
#define ISSI_CONF_AUTOFRAMEMODE 0x04
#define ISSI_CONF_AUDIOMODE 0x08

#define ISSI_REG_PICTUREFRAME  0x01

#define ISSI_REG_SHUTDOWN 0x0A
#define ISSI_REG_AUDIOSYNC 0x06

#define ISSI_COMMANDREGISTER 0xFD
#define ISSI_BANK_FUNCTIONREG 0x0B 

#define _GAMMA 2.2

class IS31FL3731 : public Adafruit_GFX {
 public:
	const uint8_t _adr[4] = {0x74, 0x77, 0x75, 0x76};
	int32_t _maxPx = 0;
	uint8_t _n;
  	IS31FL3731(uint8_t n);
  	boolean begin();
  	void drawPixel(int16_t x, int16_t y, uint16_t color);
  	void display();
  	void clearDisplay();
  	int32_t getXPxMax();
  	void clearXPxMax();
	void drawGrayscaleBitmap(int16_t x0, int16_t y0, uint8_t *p, uint16_t xSize, uint16_t ySize, uint8_t bright);
	void shutdown();
	void powerUp();
	uint8_t getPixel(int16_t x, int16_t y);
  private:
  	union d {
		uint8_t cl[576];
		uint8_t full [9][64];
		uint8_t send [9][4][16];
	}d;
	uint16_t _xDim;
	uint16_t _size;
  	void writeData(uint8_t _adr, uint8_t b, uint8_t reg, uint8_t data);
};
#endif
