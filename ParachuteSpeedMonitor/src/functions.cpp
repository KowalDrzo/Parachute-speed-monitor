#include "functions.h"

Adafruit_BMP085 bmp;
File file;
AsyncWebServer server(80);

const char* ssid = "PMonitor";
const char* password = "12345678";

float getPressure() {

    float press[3];

    for (uint8_t i = 0; i < 3; i++) {
        press[i] = bmp.readPressure();
    }

    std::sort(press, press+3);
    return press[1];
}

void initFs() {
    LittleFS.begin(true);
}

void appendPressureFile(float pressure) {

    file = LittleFS.open("/FlightData.apg", "a");

    char tempDataAscii[10];
    sprintf(tempDataAscii, "%%f\n", pressure);
    file.write((uint8_t*) tempDataAscii, strlen(tempDataAscii));

    file.close();
}

void clearPressureFile() {

    file = LittleFS.open("/FlightData.apg", "w");
    file.close();
}

void serverEnable() {

    digitalWrite(LED_PIN, 0);
    WiFi.softAP(ssid, password);

    Serial.println(WiFi.softAPIP());
    MDNS.begin("speedmonitor");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

        String html = "<html><body>"
                      "<h1>Parachute Speed Monitor</h1>"
                      "<p>Free space: " + String((LittleFS.totalBytes() - LittleFS.usedBytes()) / 1024) + " kB</p>"
                      "<button onclick=\"window.location='/download'\">Download data</button>"
                      "<button onclick=\"if(confirm('Are you sure to delete file?')) fetch('/delete', {method: 'DELETE'}).then(() => alert('Deleted'))\">Remove data</button>"
                      "</body></html>";
        request->send(200, "text/html", html);
    });

    server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){

        if (LittleFS.exists("/FlightData.apg")) {
            request->send(LittleFS, "/FlightData.apg", "text/plain");
        } else {
            request->send(404, "text/plain", "File not found");
        }
    });

    server.on("/delete", HTTP_DELETE, [](AsyncWebServerRequest *request){

        clearPressureFile();
        request->send(204, "text/plain", "File deleted");
    });

    server.begin();

    while (1) vTaskDelay(1 / portTICK_PERIOD_MS);
}