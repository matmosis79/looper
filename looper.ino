/**

    INCLUDES

 **/
#include <EEPROM.h>
#include "Button.h"

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

// =============== Defining LOOPS & AMP CHANNEL ===================
//
byte loops[] = {
  2, 6
};
byte channel = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  
  // ========== codice temporaneo =============

  // riprogrammiamo la eeprom con i preset (hard coded)
  // disponibli 4096 byte su ATmega2560

  // 10 banchi di memoria (byte) per preset
  // gli utlimi due sono il rele per il cambio canale e il suo stato (spento/acceso)

  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }

  EEPROM.write((10) + 0, loops[0]); // preset 1
  EEPROM.write((10) + 1, loops[1]); // preset 1
  EEPROM.write((10) + 8, channel); // preset 1
  EEPROM.write((10) + 9, 0); // preset 1

  EEPROM.write((20) + 0, loops[0]); // preset 2
  EEPROM.write((20) + 8, channel); // preset 2
  EEPROM.write((20) + 9, 1); // preset 2

  EEPROM.write((30) + 0, loops[1]); // preset 3
  EEPROM.write((30) + 8, channel); // preset 3
  EEPROM.write((30) + 9, 1); // preset 3

  // ========== fine codice temporaneo =============


  // associamo ad ogni tasto l'area di memoria della eeprom
  // dalla quale leggere la configurazione salvata dei preset
  buttons[0].setPreset(10); // tasto 1
  buttons[1].setPreset(20); // tasto 2
  buttons[2].setPreset(30); // tasto 3
  

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
  byte currPresetQty = buttons[currBtnNo].getPresetQty() / sizeof(byte);
  byte currPreset[currPresetQty];
  buttons[currBtnNo].getPreset(currPreset);  
  for(byte i = 0; i < sizeof(loops); i++) {
    bool found = false;
    for(byte j = 0; j < currPresetQty; j++) {
      if (currPreset[j] == loops[i]) {
        found = true;
        break;
      }
    }
    if (found) continue;

    // spegni tutti i loop che non sono del current button
    digitalWrite(loops[i], HIGH);
  }

  // TODO
  // facciamo un giro per impostare i led dei loop
  

}
