#include "src/EDTMF/EDTMF.h"
#include <HIDSerial.h>
#include <avr/wdt.h> /*Watchdog timer handling*/

HIDSerial serial;

#define LED_PIN 15
EDTMF decoder;
void setup() {

  //Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  serial.begin();

  while (millis() < 2000) {
    serial.poll();
  }

  serial.println(F("DTMF Decoder V0_08P"));
  //  pinMode(led, OUTPUT) ;
  decoder.init();
  serial.println("EDTMF initialised!");

  wdt_enable(WDTO_2S);
}



uint32_t lastHeartBit = 0;

void loop() {


  String dialStr = "";
  uint32_t lastCatchTs = millis();


  while ((millis() - lastCatchTs) < 250) {
    wdt_reset();
    serial.poll();

    String lastChar = decoder.getNextChar();
    if (lastChar != "") {

      //Serial.println(srt(millis()-lastCatchTs));
      lastCatchTs = millis();
      dialStr += lastChar;
      if (dialStr.length() == 1) {
        serial.print("Reading=> ");
      } else {
        serial.print(" . ");
      }
    }
  }

  if (dialStr != "") {
    serial.println("");
    serial.println(dialStr);
  }


  if ((millis() - lastHeartBit) > 2000) {
    //serial.println("Heat beat...-"+String(lastHeartBit));
    lastHeartBit = millis();
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  //serial.println("Heart Beat...");
  wdt_reset();
}