#include <FS.h>  //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>  //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager

#include <SSD1306Wire.h>

namespace MyWIFI {

void printTips(SSD1306Wire& display) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Connect Setting");
  display.drawString(0, 16, "1.Search the wifi 'air'");
  display.drawString(0, 24, "2.Select 'air'");
  display.drawString(0, 32, "3.Setting wifi config");
  display.display();
}

void printFail(SSD1306Wire& display) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Connect Failed");
  display.display();
}

void printIP(SSD1306Wire& display) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "WIFI: " + WiFi.SSID());
  display.drawString(0, 16, "IP: " + WiFi.localIP().toString());
  display.display();
}

void setup(SSD1306Wire& display) {
  WiFiManager wm;

  // wm.resetSettings();  // 并不能清除连接记录
  // ESP.eraseConfig();
  // delay(2000);
  // ESP.reset();

  printTips(display);

  bool success = wm.autoConnect("air");

  if (success) {
    printIP(display);
  } else {
    printFail(display);
    delay(3000);
    ESP.reset();
  }
}

}