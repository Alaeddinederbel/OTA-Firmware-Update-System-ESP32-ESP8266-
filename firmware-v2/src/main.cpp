
//  firmware-v2 — New firmware pushed via OTA

#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("============================");
    Serial.println("  OTA Update Successful!");
    Serial.println("  Device is updated and running");
    Serial.println("  Firmware v2.0");
    Serial.println("============================");
}

void loop() {
    Serial.println("Device running firmware v2.0... ");
    delay(2000);
}
