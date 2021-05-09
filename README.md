# Birdhouse Peeper

An IoT sensor, based on Espressif, that monitors a birdhouse's inside.

Current implementation uses the esp-who's camera-web-server example (without the face detection).
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
idf.py build
idf.py flash
idf.py monitor
```
