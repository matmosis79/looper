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
int relaySSRPin = 6;

void buttonSelect(byte btn, boolean force) {
  precBtnNo = currBtnNo; // l'ultimo selezionato
  currBtnNo = btn; // il corrente
  
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
      
  if (currBtnNo==1) digitalWrite(relayPin, digitalRead(buttons[currBtnNo].getLedPin()));
  else digitalWrite(relayPin, LOW);

  if (currBtnNo==2) digitalWrite(relaySSRPin, !digitalRead(buttons[currBtnNo].getLedPin()));
  else digitalWrite(relaySSRPin, HIGH);


  // resettiamo tutti gli altri led
  for (byte btnNo = 0; btnNo < sizeof(buttons) / sizeof(Button); btnNo++) {
    if (btnNo==currBtnNo) continue;
        
      // spegni tutti i led che non sono del current button
      digitalWrite(buttons[btnNo].getLedPin(), LOW);
  }
   
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   
  pinMode(relayPin, OUTPUT);
  pinMode(relaySSRPin, OUTPUT);
  digitalWrite(relaySSRPin, HIGH);
  
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

      if (precBtnNo != btnNo && buttons[btnNo].isMomentary()) {
        Serial.println("torno al precedente");
        buttonSelect(precBtnNo, true);
      } else {
        Serial.println("tasto corrente");
	buttonSelect(btnNo, false);
      }

    }      
  }
  
}

