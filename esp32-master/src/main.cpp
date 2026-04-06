
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ── Change these ──────────────────────────────────────────────────────────────
const char* WIFI_SSID     = "YOUR_WIFI_SSID";       // paste your network name
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";  // paste your password
const char* ESP8266_IP    = "192.168.x.x";        // paste ESP8266 IP here
// ─────────────────────────────────────────────────────────────────────────────

const int RETRY_DELAY_MS = 10000;

void triggerTask(void* pvParameters) {
    bool success = false;

    while (!success) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("[ESP32] WiFi lost — waiting...");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        HTTPClient http;
        String url = String("http://") + ESP8266_IP + "/update";

        Serial.println("[ESP32] Sending OTA trigger → " + url);
        http.begin(url);
        http.setTimeout(10000);

        int httpCode = http.GET();
        Serial.printf("[ESP32] Response code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) {
            Serial.println("[ESP32] ✓ Trigger accepted — ESP8266 is updating!");
            success = true;
        } else {
            Serial.printf("[ESP32] ✗ Failed (code %d). Retrying in %d s...\n",
                          httpCode, RETRY_DELAY_MS / 1000);
            vTaskDelay(RETRY_DELAY_MS / portTICK_PERIOD_MS);
        }

        http.end();
    }

    Serial.println("[ESP32] Trigger task done.");
    vTaskDelete(NULL);   // self-delete
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n=== ESP32  OTA Trigger Master ===");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("[ESP32] Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("[ESP32] Connected!");
    Serial.print("[ESP32] IP: ");
    Serial.println(WiFi.localIP());

    // Pin task to Core 1 (app core); Core 0 handles Wi-Fi stack
    xTaskCreatePinnedToCore(
        triggerTask,    // function
        "TriggerTask",  // name
        4096,           // stack (bytes)
        NULL,           // param
        1,              // priority
        NULL,           // handle
        1               // core
    );
}

void loop() {}   // everything runs in the FreeRTOS task
