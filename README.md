# luminous-particle
This is an LED lighting controller for the Particle Photon. Much of the color modeling and mixing in Luminous is derived from [TeesnyLED](https://github.com/saikoLED/TeensyLED) by Brian Neltner.

Luminous is designed to compile locally using Particle Dev, and to boot and run offline. It can connect to the Particle cloud, but does not require the connection. If a device is connected to the cloud, `build.cmd` will compile and update the device. `build-dfu.cmd` will compile and flash using the DFU utility.

I'm using my own forks of the Adafruit_GFX, Adafruit_SSD1351, clickButton, and PCA9695 libraries to address minor issues, compiling for Particle, and compiling offline.

## Object model
- Emitter: a single emitter with a wavelength. Example: the red LED in a RGBW module.
- CompositeModule: a light source that combines multiple emitters, with logic to return each Emitter's out level based on desired light output (currently just HSI). Example: LedEngin LZ4.
- OutputInterface: generic interface to set emitter brightnesses.
- OutputPCA9685: OutputInterface wrapper for the PCA9685 I2C PWM controller.
- HSILight: a light fixture that has a CompositeModule in it and is addressable on an OutputInterface
- HSIColor: class to model colors in HSI; directly from TeensyLED

## Current application
Luminous expects a voltage-dividing potentiometer on A0 and a momentary switch on D4. The potentiometer sets global brightness for all LEDs. The mode button has a few functions:

- Click: advance through modes (currently "off", "test", and "rotate")
- Double-click: toggle Particle cloud connection (offline at boot)
- Long click: advance through debugging levels

## Notes
- You only need one Emitter per *type* of light emitter. Likewise, only one CompositeModule per *type* of module. It's the HSILights that represent actual lighting nodes; use one HSILight per addressable node.

- The global `globalBrightness` variable can be used as a blackout. All OutputInterfaces should scale the power for the emitters they are responsible for by `globalBrightness` before sending.

- At present only the PCA9685 is supported for output, but it should be trivial to write a new outputinterface for direct pin output or other I2C/SPI output device.

- Hue is represented in degrees; 0 = 360 = red.

- Saturation is a float, with values between 0 and 1. Value 0 is fully saturated (most pure color), 1 is fully desaturated (white).

- Intensity is a float, with values between 0 and 1. Value 0 is off, 1.0 is as bright as possible

- The system does not currently understand differences in brightnesses between different Emitters or CompositeModules; the same HSI on a 3W RGBW led will be much brighter than on a 150ma RGBW led.
