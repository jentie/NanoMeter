/* 
  NanoMeter - perform some simple measurements

  Jens, 20239898

  based on:
  FreqCount - https://www.pjrc.com/teensy/td_libs_FreqCount.html

  TM1637 - https://github.com/avishorp/TM1637

  inputs:
  D5 - frequency input
  A0 - voltage input

 */


#include <FreqCount.h>
#include <TM1637Display.h>

// #define LED_BUILTIN 13

#define CLK 8
#define DIO 9

char mode = ' ';

TM1637Display display(CLK, DIO);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("\nNanoMeter");

  display.setBrightness(0x0f);
  uint8_t data[] = { 0x40, 0x40, 0x40, 0x40 };  // ----
  display.setSegments(data);

  FreqCount.begin(1000);

  analogReference(INTERNAL);  // 1.1 V
}


void loop() {
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    unsigned long freq = (count * 16) / 10028;
    Serial.println(freq);
    // display.showNumberDecEx(freq, 0x40, false);  // enable dots :

    unsigned long val = analogRead(A0);  // 1023 == 1.1V
    val = val * 11 * 1075 / 10000;       //  (100k+10k)/10k=11   1.1V/1023=0,001075

    Serial.println(val);
    //   display.showNumberDec(val, false);
    display.showNumberDecEx(val, 0x40, false);  // enable dots :

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // toogle every measurement cycle
  }
}
