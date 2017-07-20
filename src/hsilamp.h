
class HSILamp {
  private:
    std::vector<float> _maxvalues;
    std::shared_ptr<Colorspace> _colorspace;
  public:
    HSILamp(std::shared_ptr<Colorspace> colorspace, int i2caddress)
    void addColorspace(std::shared_ptr<Colorspace> colorspace);
    void setColor(HSIColor &color);
    void setEmitters(std::vector<float> &LEDs, std::vector<int> &pins);
};
