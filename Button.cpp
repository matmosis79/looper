/**
    @file		Button.cpp
    Project		axefx.de MIDI Borad
 	@brief		single class representing one Button
 	@version	1.0.3
    @author		Bastian Buehrig
 	@date		14/11/13
    license		GPL axefx.de - 2013
*/

#include "Button.h"

/**
   Standard-Constructor

*/
Button::Button() {
}

/**
   Constructor for Button-Initialization

   @param btnPin          Pin-Number where the switch is connected
   @param ledPin          Pin-Number where the LED is connected
*/
Button::Button(byte    btnPin,           /** Pin-Number where the switch is connected */
               byte    ledPin            /** Pin-Number where the LED is connected */
              ) {

  // Setting instance-variables
  _btnPin = btnPin;
  _ledPin = ledPin;

  // Set Pin-Mode für Switch and LED
  pinMode(_btnPin, INPUT_PULLUP);
  pinMode(_ledPin, OUTPUT);

  // Set initial-Button-State
  _previous = HIGH;
  _momentaryOn = false;
}

/**
   Standard-Destructor
*/
Button::~Button() {
}


/**
   This method will called, to check Button-State and will
   send a configured MIDI-Message. Depending on the
   Button-Configuration will send a PC or CC Message

   @return    void
*/
byte Button::checkState() {
  // Check if Button-State is different from the call before  
  byte ret = 0;  
  byte reading = digitalRead(_btnPin);  // read input value

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading != _previous && millis() - _time > _debounce) {
    _time = millis();
    
    // impostiamo l'azione in base allo stato del led
    // (che fa da nostro latching)
    _setAction(!digitalRead(_ledPin));
    
    ret = 1;    
  }

  if (millis() - _time > _debounce && reading==LOW && !_momentaryOn) {
    _momentaryOn = true;
    Serial.println("momentary ON");
  }
  if (millis() - _time > _debounce && reading==HIGH && _momentaryOn) {
    _momentaryOn = false;
    Serial.println("momentary OFF");
  }

  _previous = reading;
  return ret;
}

void Button::select() {
  // forziamo l'azione ad ACCESO
  _setAction(HIGH);
}

void Button::setPreset(int eepromAddr) {
  _eepromAddr = eepromAddr;
  memset(_preset, 0, sizeof(_preset)); // clear array

  // leggiamo la configurazione del preset dalla memoria
  for(byte i=0; i<10; i++) {
    byte v = EEPROM.read((_eepromAddr)+i);
    
    switch (i) {
      case 8: // caso pin canale ampli
        _ampChannel = v;        
        break;
      case 9: // caso stato pin canale ampli
        _ampChannelState = v;        
        break;
      default: // caso loop
        _preset[i] = v;        
    }
  }
}

void Button::getPreset(byte preset[]) {
  memcpy( preset, _preset, 10*sizeof(byte) );
}

byte Button::getPresetQty() {
  return sizeof(_preset);
}

int Button::getEepromAddr() {
  return _eepromAddr;
}

byte Button::getLedPin() {
  return _ledPin;
}

byte Button::isMomentary() {
  return _momentaryOn;
}

void Button::_setAction(byte state){
  // accendiamo/spegniamo il nostro LED
  digitalWrite(_ledPin, state );

  // accendiamo/spegniamo i nostri loop
  for(byte i = 0; i < sizeof(_preset); i++) {
    if (_preset[i] == 0) continue;

    Serial.print("toggle loop: ");
    Serial.println(_preset[i]);
    digitalWrite(_preset[i], !digitalRead(_ledPin));
  }

  // impostiamo o stato del canale dell'ampli
  digitalWrite(_ampChannel, _ampChannelState);
}
