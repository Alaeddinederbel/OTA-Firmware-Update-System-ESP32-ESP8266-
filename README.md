# OTA Firmware Update System — ESP32 + ESP8266

This project implements an *Over-The-Air (OTA)* firmware update system for IoT devices. Instead of physically plugging a USB cable into every device to update its software, this system lets you push new firmware wirelessly over Wi-Fi to any number of devices on your network.

Think of it like your phone downloading a software update in the background. The device updates itself, reboots, and runs the new version all without you touching it

---

# Why This Matters in Real Life

Imagine you have 500 temperature sensors deployed in a factory. A bug is discovered in the firmware. Without OTA, you would need to physically visit each device, plug in a cable, and flash it. That takes weeks and costs a fortune.

With OTA, you push a fix from your laptop in minutes all 500 devices update themselves overnight

----

# Mon projet

![Overview](Overview.png)

----

## Project Structure

```
ota-esp32-esp8266/
├── esp32-master/           ESP32 — sends GET /update trigger
│   ├── platformio.ini
│   └── src/main.cpp
├── esp8266-client/         ESP8266 — HTTP server + OTA download
│   ├── platformio.ini
│   └── src/main.cpp
├── firmware-v2/            New firmware to push via OTA
│   ├── platformio.ini
│   └── src/main.cpp
├── firmware-server/        Python serves firmware.bin from here
│   └── README.md
├── .gitignore
└── README.md
```

---

## Variables to Configure

| File | Variable | Value |
|------|----------|-------|
| esp32-master/src/main.cpp | WIFI_SSID | Your network name |
| esp32-master/src/main.cpp | WIFI_PASSWORD | Your password |
| esp32-master/src/main.cpp | ESP8266_IP | IP shown on ESP8266 serial monitor |
| esp8266-client/src/main.cpp | WIFI_SSID | Your network name |
| esp8266-client/src/main.cpp | WIFI_PASSWORD | Your password |
| esp8266-client/src/main.cpp | FIRMWARE_URL | http://YOUR_PC_IP:8000/firmware.bin |

---

## Find Your PC IP

- Windows: `ipconfig` → look for 192.168.x.x

---

## The real workflow, in order

┌─────────────────────────────────────────────────────┐
│  1. Open firmware-v2/                               │
│     → Build                                         │
│     → Copy .pio/build/nodemcuv2/firmware.bin        │
│       into firmware-server/firmware.bin             │
│     → Close the folder (File → Close Folder)        │
├─────────────────────────────────────────────────────┤
│  2. Open esp8266-client/                            │
│     → Edit src/main.cpp (SSID, PC IP address)       │
│     → Upload                                        │
│     → Open Serial Monitor → note the displayed IP   │
│     → Keep VS Code open on this project             │
├─────────────────────────────────────────────────────┤
│  3. Open a 2nd VS Code window                       │
│     File → New Window                               │
│     → Open Folder → esp32-master/                   │
│     → Edit src/main.cpp (SSID, ESP8266 IP address)  │
│     → Upload                                        │
└─────────────────────────────────────────────────────┘


---

## Tech Stack
- ESP32 + ESP8266
- Arduino framework (PlatformIO)
- Wi-Fi / TCP-IP / HTTP
- FreeRTOS (ESP32 task)
- ESP8266WebServer + ESP8266httpUpdate

## License
MIT

## Author
**Ala Eddine Derbel**, 
*Embedded Systems Engineer*

