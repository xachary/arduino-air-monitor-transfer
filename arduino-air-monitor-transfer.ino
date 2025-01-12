// OLED
#include <Wire.h>
#include <SSD1306Wire.h>
#include <ESP8266HTTPClient.h>

#include "wifi.h"
#include "rx.h"

// OLED
SSD1306Wire display(0x3c, D5, D6);

void setup() {
  Serial.begin(115200);
  Serial.println();

  // 初始化OLED
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  // 初始化WIFI
  MyWIFI::setup(display);

  // 初始化接收器
  Transfer::RX.init();
}

// Last process time
long lastProcessSecond = 0;

void process() {
  if (WiFi.status() == WL_CONNECTED) {
    send();
  } else {
    MyWIFI::printFail(display);
    delay(1000);
    // 网络断开，直接重启
    ESP.restart();
  }
}

void loop() {
  // process each second
  if (millis() / 1000 - lastProcessSecond >= 1) {
    process();

    lastProcessSecond = millis() / 1000;
  }
}

void send() {
  // 客户端
  WiFiClient client;
  HTTPClient http;

  // 接口信息
  String host = "192.168.1.43";
  // String host = "192.168.1.12";
  // String host = "192.168.2.1";
  // String host = "192.168.217.55";
  int port = 8081;

  // 接收内容
  String json = Transfer::RX.read();
  Serial.println();
  Serial.print("JSON:");
  Serial.println(json);

  // 判断 display
  if (json[11] == '1') {
    display.displayOn();
  } else {
    display.displayOff();
  }

  // 显示 wifi 信息
  MyWIFI::printWifi(display, false);

  if (json.length() > 0) {
    // 有数据

    // 请求
    String url = "/batch";
    http.begin(client, "http://" + host + ":" + (String)port + url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json);
    http.end();

    // 请求结果
    if (httpCode == 200) {
      // 打印 json 信息，自动换行
      display.drawStringMaxWidth(0, 13, 128, json);
    } else {
      Serial.println(httpCode);
      display.drawString(0, 13, "Request failed: " + (String)httpCode);
    }

    display.display();
  }
}
