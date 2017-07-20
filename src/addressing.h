
// A fixture can have many lamps. A lamp can have many emitters.
class Fixture {
  private:
    char (&_name)[10];
    HSILamp _lamps[1];
  public:
    Fixture(char (&name)[10], HSILamp (&lamps));
    Fixrture(void);
    char* getName(void);
};
