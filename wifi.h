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
  display.drawString(0, 0, "Connecting...");
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

void printConnect(SSD1306Wire& display) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Connecting...");
  display.display();
}

void printWifi(SSD1306Wire& display, bool ip) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "WIFI: " + WiFi.SSID());
  if (ip) {
    display.drawString(0, 16, "IP: " + WiFi.localIP().toString());
  }
  display.display();
}

void setup(SSD1306Wire& display) {
  WiFiManager wm;

  // wm.resetSettings(); // 可以清除连接记录

  printTips(display);

  // 为了处理场景：
  // 网络偶发连接失败后，一直卡在设置模式
  //
  // 如果自动连接不成功，进入设置模式后，
  // 1分钟内没有进行设置，则 wm.autoConnect 不再等待
  // 后面会有重启的逻辑
  wm.setConfigPortalTimeout(60);

  bool success = wm.autoConnect("air");

  if (success) {
    printWifi(display, false);
  } else {
    printFail(display);
    delay(3000);
    // 重启，回到上面的 wm.autoConnect
    // 再次重试自动连接
    ESP.restart();
  }
}

}