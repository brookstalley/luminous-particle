#ifndef OUTPUTPCA9685_H
#define OUTPUTPCA9685_H

class OutputPCA9685 {
  private:
    PCA9685 _pwm;
    
  protected:
    bool initImplmentation();
    bool sleepImplementation();
    bool allOffImplementation();

  public:
    bool sendCommands(std::vector<outputEmitter> emitters) = 0;

    bool init();
    bool sleep();
    bool allOff();

};

#endif
