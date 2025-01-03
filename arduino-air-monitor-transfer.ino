// OLED
#include <Wire.h>
#include <SSD1306Wire.h>
#include <ESP8266HTTPClient.h>

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
  Serial.println("loop");
  if (WiFi.status() == WL_CONNECTED) {
    send();
  } else {
    MyWIFI::printFail(display);
  }
  delay(3000);
}

void send() {
  MyWIFI::printIP(display);

  // 客户端
  WiFiClient client;
  HTTPClient http;

  String host = "192.168.1.43";
  // String host = "192.168.1.12";
  int port = 8081;

  float value = 123.4;

  // 将需要发送的数据信息放入客户端请求
  String url = "/tvoc";

  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawString(0, 24, "Request:");
  display.drawString(0, 32, host + ":" + port + url);
  display.drawString(0, 40, "Value: " + (String)value);

  http.begin(client, "http://" + host + ":" + (String)port + url);  //Specify request destination
  http.addHeader("Content-Type", "text/plain");

  int httpCode = http.POST((String)value);

  http.end();

  if (httpCode == 200) {
    display.drawString(0, 48, "Request success!");
  } else {
    Serial.println(httpCode);
    display.drawString(0, 48, "Request failed: " + (String)httpCode);
  }

  display.display();
}
