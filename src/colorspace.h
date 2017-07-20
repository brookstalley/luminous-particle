//*********************************************************
//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

class Colorspace {
  private:
    std::vector<Emitter> _emitters;
    Emitter _white;
    std::vector<float> _slope;
    std::vector<float> _angle;
  public:
    Colorspace(Emitter &white);
    Colorspace(void);
    void addLED(Emitter &emitter);
    float getAngle(int emitternum);
    float getSlope(int emitternum);
    std::vector<float> Hue2EmitterPower(HSIColor &HSI);
    std::vector<float> getMaxValues(void);
};

class CIELamp {
  private:
    int _resolution;
    std::vector<int> _pins;
    std::vector<float> _maxvalues;
    std::shared_ptr<Colorspace> _colorspace;
  public:
    CIELamp();
    void addColorspace(std::shared_ptr<Colorspace> colorspace);
    void setColor(HSIColor &color);
    void setLEDs(std::vector<float> &LEDs, std::vector<int> &pins);
    void begin(void);
};
