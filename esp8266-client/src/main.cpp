
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>

// ── Change these ──────────────────────────────────────────────────────────────
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* FIRMWARE_URL  = "http://192.168.x.x:8000/firmware.bin";  // <-- your PC IP
// ─────────────────────────────────────────────────────────────────────────────

ESP8266WebServer server(80);

// ── OTA callbacks ─────────────────────────────────────────────────────────────
void onOTAStart() {
    Serial.println("[ESP8266] Download started...");
}

void onOTAEnd() {
    Serial.println("[ESP8266] Download complete — rebooting...");
}

void onOTAProgress(int cur, int total) {
    static int lastPct = -1;
    int pct = (cur * 100) / total;
    if (pct != lastPct && pct % 10 == 0) {
        Serial.printf("[ESP8266] Progress: %d%%\n", pct);
        lastPct = pct;
    }
}

void onOTAError(int err) {
    Serial.printf("[ESP8266] OTA Error (%d): %s\n",
                  err,
                  ESPhttpUpdate.getLastErrorString().c_str());
}
// ─────────────────────────────────────────────────────────────────────────────

// GET /update — triggered by ESP32
void handleUpdate() {
    Serial.println("[ESP8266] Received GET /update — starting OTA...");

    // Reply BEFORE blocking on the download so ESP32 gets its 200
    server.send(200, "text/plain", "OTA triggered — downloading firmware...");
    delay(200);

    WiFiClient client;
    ESPhttpUpdate.onStart(onOTAStart);
    ESPhttpUpdate.onEnd(onOTAEnd);
    ESPhttpUpdate.onProgress(onOTAProgress);
    ESPhttpUpdate.onError(onOTAError);
    ESPhttpUpdate.rebootOnUpdate(true);   // auto-reboot after flash

    Serial.print("[ESP8266] Fetching: ");
    Serial.println(FIRMWARE_URL);

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, FIRMWARE_URL);

    // Reached only if update failed (successful update reboots the device)
    if (ret == HTTP_UPDATE_FAILED) {
        Serial.printf("[ESP8266] Update FAILED. Error (%d): %s\n",
                      ESPhttpUpdate.getLastError(),
                      ESPhttpUpdate.getLastErrorString().c_str());
    }
}

// GET / — quick status check
void handleRoot() {
    String msg = "ESP8266 OTA Client\n";
    msg += "IP: " + WiFi.localIP().toString() + "\n";
    msg += "Firmware server: " + String(FIRMWARE_URL) + "\n";
    msg += "Send GET /update to trigger OTA\n";
    server.send(200, "text/plain", msg);
}

void handleNotFound() {
    server.send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n=== ESP8266  OTA Client ===");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("[ESP8266] Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("[ESP8266] Connected!");
    Serial.print("[ESP8266] IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("[ESP8266] >>> Copy this IP into ESP32's ESP8266_IP variable <<<");

    server.on("/",       HTTP_GET, handleRoot);
    server.on("/update", HTTP_GET, handleUpdate);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("[ESP8266] HTTP server started on port 80.");
    Serial.println("[ESP8266] Waiting for GET /update from ESP32...");
}

void loop() {
    server.handleClient();   // must be called continuously
}
