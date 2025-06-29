#include "functions.h"

Adafruit_BMP085 bmp;
float initialPressure = 0;
float initialTemperature = -100;
File file;

float getPressure() {

    float press[3];

    for (uint8_t i = 0; i < 3; i++) {
        press[i] = bmp.readPressure();
    }

    std::sort(press, press+3);
    return press[1];
}

void initFs() {
    SPIFFS.begin(true);
}

float savedAlt = 0;

float countAltitude(float pressure) {

    if (initialPressure < 10) return 0;

    float alt = (initialTemperature+273.15)/0.0065*(1.0 - pow(pressure/initialPressure, 0.1903));
    float newAlt = savedAlt * 0.2 + alt * 0.8;
    savedAlt = newAlt;
    return newAlt;
}

void appendPressureFile(float pressure) {

    file = SPIFFS.open("/FlightData.apg", "a");

    char tempDataAscii[200];
    sprintf(tempDataAscii, "%d;%0.1f;%0.1f\n", millis(), pressure, countAltitude(pressure));
    file.write((uint8_t*) tempDataAscii, strlen(tempDataAscii));

    file.close();
}

void clearPressureFile() {

    file = SPIFFS.open("/FlightData.apg", "w");
    file.close();
    Serial.println("CLEARED");
}

void readPressureFile() {

    file = SPIFFS.open("/FlightData.apg", "r");

    while (file.available()) {
        String fileContent = file.readString();
        Serial.print(fileContent);
    }

    file.close();
}