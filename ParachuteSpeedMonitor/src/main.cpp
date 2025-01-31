#include "functions.h"

uint32_t timer;

void setup() {

    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN);
    bmp.begin(BMP085_ULTRAHIGHRES, &Wire);

    vTaskDelay(200 / portTICK_PERIOD_MS);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    digitalWrite(LED_PIN, 1);

    initFs();
    timer = millis();
}

void loop() {

    if (millis() - timer >= 500) {
        
        timer = millis();
        float pressure = getPressure();
        Serial.println(pressure);

        digitalWrite(LED_PIN, 0);
        appendPressureFile(pressure);
        digitalWrite(LED_PIN, 1);
    }

    if (!digitalRead(BUTTON_PIN)) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        if (!digitalRead(BUTTON_PIN)) {
            serverEnable();
        }
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
}
