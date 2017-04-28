#ifndef Button_h
#define Button_h

/**
    @file		Button.h
    Project		axefx.de MIDI Borad
 	@brief		Header-File for single class representing one Button
 	@version	1.0.3
    @author		Bastian Buehrig
 	@date		14/11/13
    license		GPL axefx.de - 2013
*/

// ==== Arduino-Standard-Lib
#include <Arduino.h>
#include "Set.h"

class Button {
  public:
    // Class Variable


    // Public Attributes


    // Constructors
    Button();
    Button(byte btnPin, byte ledPin);

    // Destructors
    ~Button();

    // Public functions
    byte checkState();
    void select();

    void setPreset(int eepromAddr);
    Set getPreset();
    int getEepromAddr();

    byte getLedPin();
    byte isMomentary();

  private:
    // Private Attributes

    byte _btnPin;                 // Pin-Number for Buttons
    byte _ledPin;                 // Pin-Number for Button Status-LED

    int _eepromAddr;		// indirizzo area di memoria eeporm dedicata
    Set _preset;		// elenco dei loop gestiti
    byte _ampChannel;		// pin canale ampli
    byte _ampChannelState;	// stato canale ampli

    byte _previous;    // the previous reading from the input pin
    byte _momentaryOn; // button in momentary mode
    
    // the follow variables are long's because the time, measured in miliseconds,
    // will quickly become a bigger number than can be stored in an int.
    long _time = 0;         // the last time the output pin was toggled
    long _debounce = 500;   // the debounce time, increase if the output flickers
    
    // Private Functions
};

#endif

