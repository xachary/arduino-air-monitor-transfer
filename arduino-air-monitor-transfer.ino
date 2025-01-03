// OLED
#include <Wire.h>
#include <SSD1306Wire.h>

#include "wifi.h"

// OLED
SSD1306Wire display(0x3c, D5, D6);

void setup() {
  Serial.begin(115200);
  Serial.println();

  // 初始化OLED
  display.init();
  display.flipScreenVertically();

  // 初始化WIFI
  MyWIFI::setup(display);
}

void loop() {
  delay(10);
}
