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

// =============== Defining RELAYS ===================
//
byte relays[] = {
  2, 6
};

// =============== Defining PRESETS ===================
//
byte preset1[] = {
  2, 6
};
byte preset2[] = {
  2
};
byte preset3[] = {
  6
};

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

  // resettiamo tutti gli altri led
  for (byte btnNo = 0; btnNo < sizeof(buttons) / sizeof(Button); btnNo++) {
    if (btnNo==currBtnNo) continue;
        
      // spegni tutti i led che non sono del current button
      digitalWrite(buttons[btnNo].getLedPin(), LOW);
  }

  // resettiamo tutti gli altri rele
  /*byte *p = buttons[currBtnNo].getPresets();
  for (byte i = 0; i < sizeof(relays) / sizeof(byte); i++) {
    byte found=false;
    byte element=relays[i];
    
    for (byte j = 0; j < sizeof(p) / sizeof(byte); j++) {
      if (element==p[j]) found=true;
    }

    if (found) continue;

    // spegniamo il rele
    digitalWrite(element, HIGH);
  }*/
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // inizializziamo lo stato dei relay
  if(sizeof(relays) > 0 ) {
    for(byte i = 0; i < sizeof(relays); i++) {
      pinMode(relays[i], OUTPUT);
      digitalWrite(relays[i], HIGH);
    }
  }
  
  // associamo ad ogni tasto il proprio presets (al momento hard coded)
  buttons[0].setPresets(preset1);
  buttons[1].setPresets(preset2);
  buttons[2].setPresets(preset3);
  
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
        //Serial.println("torno al precedente");
        buttonSelect(precBtnNo, true);
      } else {
        //Serial.println("tasto corrente");
	      buttonSelect(btnNo, false);
      }

    }      
  }
  
}

