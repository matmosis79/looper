/**

    INCLUDES

 **/
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "Button.h"
#include "Loop.h"

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
Loop loops[] = {
  // Loop-Pin   LED-Pin
  Loop( 2 , 40 ),
  Loop( 6 , 41 )
};
Loop channel = Loop( 8 , 50 );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  
  // ========== codice temporaneo =============

  // riprogrammiamo la eeprom con i preset (hard coded)
  // disponibli 4096 byte su ATmega2560

  // 10 banchi di memoria (byte) per preset
  // gli utlimi due sono il rele per il cambio canale e il suo stato (spento/acceso)

  for (int i = 0; i < 4096; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM_writeAnything((100) + 0, loops[0]); // preset 1
  EEPROM_writeAnything((100) + 1, loops[1]); // preset 1
  EEPROM_writeAnything((100) + 8, channel); // preset 1
  EEPROM_writeAnything((100) + 9, 0); // preset 1

  EEPROM_writeAnything((200) + 0, loops[0]); // preset 2
  EEPROM_writeAnything((200) + 8, channel); // preset 2
  EEPROM_writeAnything((200) + 9, 1); // preset 2

  EEPROM_writeAnything((300) + 0, loops[1]); // preset 3
  EEPROM_writeAnything((300) + 8, channel); // preset 3
  EEPROM_writeAnything((300) + 9, 1); // preset 3

  // ========== fine codice temporaneo =============


  // associamo ad ogni tasto l'area di memoria della eeprom
  // dalla quale leggere la configurazione salvata dei preset
  buttons[0].setPreset(100); // tasto 1
  buttons[1].setPreset(200); // tasto 2
  buttons[2].setPreset(300); // tasto 3
  

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
  byte* currPreset = buttons[currBtnNo].getPreset();
  byte currPresetQty = buttons[currBtnNo].getPresetQty() / sizeof(Loop);
  for(byte i = 0; i < sizeof(loops); i++) {
    bool found = false;
    for(byte j = 0; j < currPresetQty; j++) {
      if (currPreset[j].getLoopPin() == loops[i].getLoopPin()) found = true;
    }
    if (found) continue;

    // spegni tutti i loop che non sono del current button
    digitalWrite(loops[i], HIGH);
  }

}
