# luminous-particle
This is an LED lighting controller for the Particle Photon. Much of the color modeling and mixing in Luminous is derived from [TeesnyLED](https://github.com/saikoLED/TeensyLED) by Brian Neltner.

Luminous is designed to compile locally using Particle Dev, and to boot and run offline. It can connect to the Particle cloud, but does not require the connection. If a device is connected to the cloud, `build.cmd` will compile locally and update the device using the cloud. `build-dfu.cmd` will compile locally and flash over USB using the DFU utility.

I'm using my own forks of the Adafruit_GFX, Adafruit_SSD1351, Adafruit_Ads1X15, clickButton, and PCA9695 libraries to address minor issues, compiling for Particle, and compiling offline.

## Object model
- Emitter: a single emitter with either a wavelength (color) or a CUELUV coordinate (color). Example: the red LED in a RGBW module.
- CompositeModule: a light source that combines multiple emitters, with logic to return each channel's output level based on desired light output from an HSI input. Example: LedEngin LZ4.
- HSIColor: class to model colors in HSI; can convert from HSL or RGB.
- Mode: base class that defines how lighting modes offer start, run, and end functions

- OutputInterface: base class for methods to set emitter brightnesses.
- OutputPCA9685: derived from OutputInterface; wraps the PCA9685 I2C PWM controller.
- E131: A fork with minor changes to accommodate Luminous' debugging mechanism.

- HSILight: a light fixture that has a CompositeModule in it and is addressable on an OutputInterface. Optionally has a TemperatureInterface to monitor temperature. Optionally has an E131 to receive commands. Currently only supports 16-bit HSI mode.

- TemperatureInterface: base class for temperature reading
- TemperatureAds1115: derived from TemperatureInterface; wraps the Ads1115 I2C PWM controller.

## Current application
Luminous expects a voltage-dividing potentiometer on A0 and momentary switches to control the menus on D4 (back), D3 (previous/up), D2 (next/down), and A1 (select). The potentiometer sets global brightness for all LEDs.

Long-clicking the select button connects to the Particle cloud.

Currently, a few  modes are defined in `modes.cpp`: Off, Test, Rotate, and E131. There are also utility functions to advance to the next mode and to set a mode by name or number.

## Notes
- You only need one Emitter per *type* of light emitter. Likewise, only one CompositeModule per *type* of module. It's the HSILights that represent actual lighting nodes; use one HSILight per addressable node.

- The global `globalBrightness` variable can be used as a blackout. All OutputInterfaces should scale the power for the emitters they are responsible for by `globalBrightness` before sending.

- At present only the PCA9685 is supported for output, but it should be trivial to write a new outputinterface for direct pin output or other I2C/SPI output device.

- Hue is represented in degrees; 0 = 360 = red.

- Saturation is a float, with values between 0 and 1. Value 0 is fully saturated (most pure color), 1 is fully desaturated (white).

- Intensity is a float, with values between 0 and 1. Value 0 is off, 1.0 is as bright as possible

- The system does not currently understand differences in brightnesses between different Emitters or CompositeModules; the same HSI on a 3W RGBW led will be much brighter than on a 150ma RGBW led.

- In order for Wifi / E131 to work, you'll need to copy credentials.h.example to credentials.h and update it with your network's SSID and password. The `.gitignore` file is already configured to not commit/push that file.

- If WiFi connects successfully, Luminous defaults to E131 control mode. If WiFi does not connect, it defaults to all lights off.
