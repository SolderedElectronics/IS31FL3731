#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <IS31FL3731.h>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

IS31FL3731::IS31FL3731(uint8_t n) : Adafruit_GFX(16*n, 9) {
	_n = n;
	_xDim = 16*_n;
	_size = _xDim*9;
}

boolean IS31FL3731::begin() {
  	Wire.begin(4, 5);
  	twi_setClock(400000);
	
	for(int i = 0; i<_n; i++) {
		//shutdown
		writeData(_adr[i], ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);
		delay(10);
		// out of shutdown
		writeData(_adr[i], ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);
		// picture mode
		writeData(_adr[i], ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG, ISSI_REG_CONFIG_PICTUREMODE);
		//displayFrame(_frame);
		writeData(_adr[i], ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTUREFRAME, 0);
		//audioSync(false);
		writeData(_adr[i], ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, 0x0);
	}
  	// all LEDs on & 0 PWM
  	clearDisplay();
  	display();
  	
	//clear(); // set each led to 0 PWM
	for(int k = 0; k<_n; k++) {
		for (uint8_t f=0; f<8; f++) {
			for (uint8_t i=0; i<=0x11; i++)
				writeData(_adr[k], f, i, 0xff);     // each 8 LEDs on
		}
	}
	
  return true;
}

void IS31FL3731::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if(x > _maxPx) _maxPx = x;
  if ((x < 0) || (x >= _xDim)) return;
  if ((y < 0) || (y >= 9)) return;
  if (color > 255) color = 255; // PWM 8bit max
 // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    _swap_int16_t(x, y);
    x = 16 - x - 1;
    break;
  case 2:
    x = 16 - x - 1;
    y = 9 - y - 1;
    break;
  case 3:
    _swap_int16_t(x, y);
    y = 9 - y - 1;
    break;
  }

  //setLEDPWM(x + y*16, color, _frame);
  //buf[x+(y*_xDim)] = color;
  d.full[y][x] = color;
  return;
}

void IS31FL3731::display() {
	for(int k = 0; k<_n; k++) {
		Wire.beginTransmission(_adr[k]);
		Wire.write((byte)ISSI_COMMANDREGISTER);
		Wire.write(0);
		Wire.endTransmission();
		
		Wire.beginTransmission(_adr[k]);
		Wire.write((byte)0x24);
		int i, j, n = 0, z;
		for(i = 0; i<4; i++) {
			for(j = 0;j<16;j++) {
				Wire.write((byte)d.send[i][k][j]);
			}
		}
		Wire.endTransmission();
		
		Wire.beginTransmission(_adr[k]);
		Wire.write((byte)0x24+64);
		for(i = 4; i<9; i++) {
			for(j = 0;j<16;j++) {
				Wire.write((byte)d.send[i][k][j]);
			}
		}
		Wire.endTransmission();
	}
}

void IS31FL3731::clearDisplay() {
	//for(int i = 0; i<576; i++) {
	//	d.cl[i] = 0;
	//}
	memset(d.cl, 0, 576);
	setCursor(0,0);
}

void IS31FL3731::writeData(uint8_t _adr, uint8_t b, uint8_t reg, uint8_t data) {
	Wire.beginTransmission(_adr);
 	Wire.write((byte)ISSI_COMMANDREGISTER);
 	Wire.write(b);
 	Wire.endTransmission();
 
  	Wire.beginTransmission(_adr);
  	Wire.write((byte)reg);
  	Wire.write((byte)data);
  	Wire.endTransmission();
}

int32_t IS31FL3731::getXPxMax() {
	return _maxPx;
}

void IS31FL3731::clearXPxMax() {
	_maxPx = 0;
}

void IS31FL3731::drawGrayscaleBitmap(int16_t x0, int16_t y0, uint8_t *p, uint16_t xSize, uint16_t ySize, uint8_t bright) {
  int endPosX = xSize + x0;    //Calculate end position for x cooridinate (needed for x position offest).
  int endPosY = ySize + y0;

  int n = 0;                                         //Variable for indexing picture array
  for (int j = y0; j < endPosY; j++) {                  //First select row, then write data into seleced row
    for (int i = x0; i < endPosX; i++) {
	  drawPixel(i, j + y0, (uint8_t)(bright * pow(p[n] / 255.0, _GAMMA)));
      n++;                   //Increment variable for array indexing
    }
  }
}

void IS31FL3731::shutdown() {
	for(int i = 0; i<_n; i++) {
		writeData(_adr[i], ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);
	}
}

void IS31FL3731::powerUp() {
	for(int i = 0; i<_n; i++) {
		writeData(_adr[i], ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);
	}
}

uint8_t IS31FL3731::getPixel(int16_t x, int16_t y) {
	return d.full[y][x];
}