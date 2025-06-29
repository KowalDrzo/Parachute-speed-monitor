#include "functions.h"

uint32_t timer;

void setup() {

    Serial.begin(115200);

    pinMode(GND_PIN, OUTPUT);
    vTaskDelay(200 / portTICK_PERIOD_MS);

    Wire.begin(SDA_PIN, SCL_PIN, 50000);
    Serial.println(bmp.begin(BMP085_ULTRAHIGHRES, &Wire));

    vTaskDelay(200 / portTICK_PERIOD_MS);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    digitalWrite(LED_PIN, 1);

    initFs();
    timer = millis();
}

void loop() {

    if (millis() - timer >= 200) {

        timer = millis();
        while (initialTemperature < -50) {
            float temperature = bmp.readTemperature();
            if (temperature > -50 && temperature < 100) {
                initialTemperature = temperature;
            }
            Serial.println(initialTemperature);
        }

        float pressure = getPressure();
        if (initialPressure < 10) initialPressure = pressure;
        Serial.println(pressure);

        if (pressure > 300 && pressure < 120000) {
            digitalWrite(LED_PIN, 0);
            appendPressureFile(pressure);
            digitalWrite(LED_PIN, 1);
        }
    }

    if (!digitalRead(BUTTON_PIN)) {
        int8_t i;
        for (i = 0; i < 10; i++) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            if (digitalRead(BUTTON_PIN)) break;
        }

        if (i < 9) readPressureFile();
        else clearPressureFile();
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
}
