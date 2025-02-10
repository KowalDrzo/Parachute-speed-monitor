#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <LittleFS.h>

#include "pinout.h"

extern Adafruit_BMP085 bmp;
extern float initialPressure;

float getPressure();
void initFs();
void appendPressureFile(float pressure);
void clearPressureFile();
void readPressureFile();

#endif
