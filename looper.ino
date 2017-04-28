/**

    INCLUDES

 **/

#include <EEPROM.h>
#include "Button.h"
#include "Set.h"

// ============== SETUP ===============================

// ============== Button-Definitions: =================
//
Button buttons[] = {
  // Button-Pin   LED-Pin
  Button( 5 , 10 ),
  Button( 4 , 9 ),
  Button( 3 , 8 )
};
byte precBtnNo = 0;
byte currBtnNo = 0;

// =============== Defining LOOPS ===================
//
Set loops;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // inizializziamo lo stato dei relay
  if(sizeof(relays) > 0 ) {
    for(byte i = 0; i < sizeof(loops); i++) {
      pinMode(relays[i], OUTPUT);
      digitalWrite(relays[i], HIGH); //pullup all relay outputs in case off low level relayboard
    }
  }

  // inizializziamo l'elenco dei loops
  loops.add(2);
  loops.add(6);
  // ...
  
  
  // ========== codice temporaneo =============

  // riprogrammiamo la eeprom con i preset (hard coded)
  // disponibli 4096 byte su ATmega2560
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.write((11) + 0, 2); // tasto 1
  EEPROM.write((11) + 1, 6); // tasto 1
  EEPROM.write((21) + 0, 2); // tasto 2
  EEPROM.write((31) + 0, 6); // tasto 3
  // ========== fine codice temporaneo =============

  // associamo ad ogni tasto l'area di memoria della eeprom
  // dalla quale leggere la configurazione salvata dei preset
  buttons[0].setPreset(11); // tasto 1
  buttons[1].setPreset(21); // tasto 2
  buttons[2].setPreset(31); // tasto 3  
  

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
    Serial.println("bypass");
  else
    Serial.println("loop");

  // resettiamo tutti gli altri led
  for (byte btnNo = 0; btnNo < sizeof(buttons) / sizeof(Button); btnNo++) {
    if (btnNo==currBtnNo) continue;
        
      // spegni tutti i led che non sono del current button
      digitalWrite(buttons[btnNo].getLedPin(), LOW);
  }

  // resettiamo tutti gli altri loops
  Set currPreset = buttons[currBtnNo].getPreset();
  int loop = loops.first();
  while (loop != -1) {
    if (currPreset.has(loop)) continue;

    // spegni tutti i loop che non sono del current button
    digitalWrite(loop, HIGH);
    loop = loops.next();
  }

}
