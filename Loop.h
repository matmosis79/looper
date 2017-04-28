#ifndef Loop_h
#define Loop_h

// ==== Arduino-Standard-Lib
#include <Arduino.h>

class Loop {
  public:
    // Class Variable


    // Public Attributes


    // Constructors
    Loop();
    Loop(byte loopPin, byte ledPin);

    // Destructors
    ~Loop();

    // Public functions
    void select(byte active);
    byte getLoopPin();
    byte getLedPin();

  private:
    // Private Attributes

    byte _loopPin;                 // Pin-Number for Loop
    byte _ledPin;                 // Pin-Number for Loop Status-LED
   
    // Private Functions
};

#endif

