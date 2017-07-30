# luminous-particle
Luminous implementation on Particle

### Color mixing is derived from [TeesnyLED](https://github.com/saikoLED/TeensyLED) by Brian Neltner:

## Object model:
- Emitter: a single emitter with a wavelength. Example: the red LED in a RGBW module.
- CompositeModule: a light source that combines multiple emitters, with logic to return each Emitter's out level based on desired light output (currently just HSI). Example: LedEngin LZ4.
- OutputInterface: generic interface to set emitter brightnesses.
- OutputPCA9685: OutputInterface wrapper for the PCA9685 I2C PWM controller.
- HSILight: a light fixture that has a CompositeModule in it and is addressable on an OutputInterface
- HSIColor: class to model colors in HSI; directly from TeensyLED

## Notes
- You only need one Emitter per *type* of light emitter. Likewise, only one CompositeModule per *type* of module. It's the HSILights that represent actual lighting nodes; use one HSILight per addressable node.

- The global `globalBrightness` variable can be used as a blackout. All OutputInterfaces should scale the power for the emitters they are responsible for by `globalBrightness` before sending.

- At present only the PCA9685 is supported for output, but it should be trivial to write a new outputinterface for direct pin output.

- Hue is represented in degrees; 0 = 360 = red.

- Saturation is a float, with values between 0 and 1. Value 0 is fully saturated (most pure color), 1 is fully desaturated (white).

- Inensity is a float, with values between 0 and 1. Value 0 is off, 1.0 is as bright as possible

- The system does not currently understand differences in brightnesses between different Emitters or CompositeModules.
