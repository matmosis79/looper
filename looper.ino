/**

    INCLUDES

 **/

// ==== Button-Class
#include "Button.h"

// ============== SETUP ===============================

// ============== Button-Definitions: =================
//
Button buttons[] = {
  //    Button-Pin   LED-Pin
  Button(    5,       10 ),
  Button(    4,       9 ),
  Button(    3,       8 )
};
byte precBtnNo = 0;
byte currBtnNo = 0;

// =============== Defining LED-Groups ===================
//
byte groupPresets[] = {
  10, 9, 8
};

int relayPin = 2;

void buttonSelect(byte btnNo, boolean force) {
  precBtnNo = currBtnNo; // l'ultimo selezionato
  currBtnNo = btnNo; // il corrente
  
  if (force)
    // forziamo la selezione visto che non arriva da alterazione
    // del bottone stesso
    buttons[currBtnNo].select();

  Serial.print("tasto premuto: ");
  Serial.println(currBtnNo);
      
  byte state=digitalRead(buttons[currBtnNo].getLedPin());      
  if (state==LOW)
    Serial.println("true bypass");
  else
    Serial.println("loop");
      
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   
  pinMode(relayPin, OUTPUT);
  
  // Setting LED-Groups to Buttons if needed
  buttons[0].setLEDGroup(groupPresets, sizeof(groupPresets) );
  buttons[1].setLEDGroup(groupPresets, sizeof(groupPresets) );
  buttons[2].setLEDGroup(groupPresets, sizeof(groupPresets) );
  
  // per adesso di default accendiamo il primo button
  buttonSelect(0, true);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Read Button-State und check for activation!
  for (byte btnNo = 0; btnNo < sizeof(buttons) / sizeof(Button); btnNo++) {
    byte ret = buttons[btnNo].checkState();

    if (ret == 1) {
      buttonSelect(btnNo, false);
      
      for (byte btnNo1 = 0; btnNo1 < sizeof(buttons) / sizeof(Button); btnNo1++) {
        if (btnNo1==currBtnNo) continue;
        
        // spegni tutti i led che non sono del current button
        digitalWrite(buttons[btnNo1].getLedPin(), LOW);
      }
      
    }      
  }
  
}

