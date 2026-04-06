# firmware-server

This folder is served by Python's built-in HTTP server.
The ESP8266 downloads firmware.bin from here during OTA.

## Steps

1. Build firmware-v2 in PlatformIO (checkmark button)
2. Copy the output binary here:

   Windows (PowerShell):
     copy ..\firmware-v2\.pio\build\nodemcuv2\firmware.bin .\firmware.bin

3. Start the server (run from THIS folder):

     python3 -m http.server 8000

4. Verify — open this in your browser:

     http://YOUR_PC_IP:8000/firmware.bin   ← should download

Leave the server running while flashing the boards.

## Note
firmware.bin is in .gitignore — binaries are not committed.
Always build from source.
