# luminous-particle
Luminous implementation on Particle

Color mixing is derived from TeesnyLED by Brian Neltner:
<https://github.com/saikoLED/TeensyLED>

Object model:
  Emitter: a single emitter with a wavelength. Example: the red LED in a RGBW module.

  CompositeModule: a light source that combines multiple emitters, with logic to return each Emitter's out level based on desired light output (currently just HSI). Example: LedEngin LZ4.

  HSILight: a light fixture that has a CompositeModule in it. You'd create one CompositeModule for a particular model of multi-color LED, and one HSILight for each physical device. HSILight takes a CompositeModule, an OutputDevice, and an optional address offset in the OutputDevice

  HSIColor: class to model colors in HSI; directly from TeensyLED.
