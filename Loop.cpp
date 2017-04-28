#include "Loop.h"

/**
   Standard-Constructor

*/
Loop::Loop() {
}

/**
   Constructor for Loop-Initialization

   @param btnPin          Pin-Number where the loop is connected
   @param ledPin          Pin-Number where the LED is connected
*/
Loop::Loop(byte    loopPin,           /** Pin-Number where the loop is connected */
               byte    ledPin            /** Pin-Number where the LED is connected */
              ) {

  // Setting instance-variables
  _loopPin = loopPin;
  _ledPin = ledPin;

  // Set Pin-Mode für Switch and LED
  pinMode(_loopPin, OUTPUT);
  digitalWrite(_loopPin, HIGH); // pullup all relay outputs in case off low level relayboard
  pinMode(_ledPin, OUTPUT);
}

/**
   Standard-Destructor
*/
Loop::~Loop() {
}

void Loop::select(byte active) {
  // accendiamo/spegniamo
  digitalWrite(_loopPin, !active);
  digitalWrite(_ledPin, active);
}

byte Button::getLoopPin() {
  return _loopPin;
}

byte Button::getLedPin() {
  return _ledPin;
}

