# Birdhouse Peeper

An IoT sensor, based on Espressif, that monitors a birdhouse's inside.

Current implementation is based on these:
- the esp-who's camera-web-server example (without the face detection).
- http-request (repetitively sending an captured image to a hard-coded IP address and port)
  Current IP/port is hard-coded as 192.168.40.41/3334 (NUC)
  Use this script to receive the http request: `python3 http_server.py 3334`
  Note: HTTPS does not work properly, http2.0 did not work in python

It depends on the external repo esp32-camera.

## AI-thinker esp32-cam

https://randomnerdtutorials.com/esp32-cam-ai-thinker-pinout/

```bash
cp sdkconfig.ai-thinker.defaults sdkconfig
rm -rf build
```

## esp-eye

```bash
cp sdkconfig.esp32-cam.defaults sdkconfig
rm -rf build
```

## Building

Use one of the sdkconfig defaults

```bash
idf.py menuconfig # to configure AP SSID and passphrase (also for station mode)
idf.py build
idf.py flash
idf.py monitor
```
