#include <SoftwareSerial.h>

// 实际只用此引脚
#define _Pin_RX D1

// 实际可不接此引脚
#define _Pin_TX D2

// 起始符
#define _Frame_Start 0XFF

// 数据帧长度：
// 起始符 x 2 [2字节]
// display   [1字节]
// hcho      [4字节]
// tvoc      [4字节]
// co2       [4字节]
// temp      [4字节]
// hum       [4字节]
// uv        [4字节]
// 检验位     [4字节]
#define _Frame_Len 31

namespace Transfer {

SoftwareSerial _serial(_Pin_RX, _Pin_TX);

uint8_t floatSize = sizeof(float);
uint8_t int32Size = sizeof(int);

unsigned char floatBytes[4];
unsigned char int32Bytes[2];

struct _RX {
  void init() {
    _serial.begin(9600);
  }

  bool check(byte* buffer) {
    // 检验位计算
    int32_t sum = 0;
    for (int i = 2; i < _Frame_Len - int32Size; i++) {
      sum += buffer[i];
    }

    Serial.print("sum:");
    Serial.println(sum);

    // 检验位记录
    for (uint8_t i = 0; i < int32Size; i++) {
      int32Bytes[i] = buffer[_Frame_Len - int32Size + i];
    }

    int32_t sumCheck = 0;
    memcpy(&sumCheck, int32Bytes, int32Size);

    Serial.print("sumCheck:");
    Serial.println(sumCheck);
    Serial.println();

    // 对比、排除空数据
    return sum == sumCheck && sum > 0;
  }

  // 基于 JSON 结构：{"display":1,"hcho":1,"tvoc":2,"co2":3,"temp":4.5,"hum":6.7,"uv":8}
  String parse(byte* buffer) {
    if (check(buffer)) {
      // 下标
      uint8_t idx = 2;

      uint8_t display = buffer[idx];

      idx += 1;

      float hcho;
      for (uint8_t i = 0; i < floatSize; i++) {
        floatBytes[i] = buffer[idx + i];
      }
      memcpy(&hcho, floatBytes, floatSize);

      idx += floatSize;

      float tvoc;
      for (uint8_t i = 0; i < floatSize; i++) {
        floatBytes[i] = buffer[idx + i];
      }
      memcpy(&tvoc, floatBytes, floatSize);

      idx += floatSize;

      float co2;
      for (uint8_t i = 0; i < floatSize; i++) {
        floatBytes[i] = buffer[idx + i];
      }
      memcpy(&co2, floatBytes, floatSize);

      idx += floatSize;

      float temp;
      for (uint8_t i = 0; i < floatSize; i++) {
        floatBytes[i] = buffer[idx + i];
      }
      memcpy(&temp, floatBytes, floatSize);

      idx += floatSize;

      float hum;
      for (uint8_t i = 0; i < floatSize; i++) {
        floatBytes[i] = buffer[idx + i];
      }
      memcpy(&hum, floatBytes, floatSize);

      idx += floatSize;

      float uv;
      for (uint8_t i = 0; i < floatSize; i++) {
        floatBytes[i] = buffer[idx + i];
      }
      memcpy(&uv, floatBytes, floatSize);

      idx += floatSize;

      Serial.print("display:");
      Serial.println(display);
      Serial.print("hcho:");
      Serial.println(hcho);
      Serial.print("tvoc:");
      Serial.println(tvoc);
      Serial.print("co2:");
      Serial.println(co2);
      Serial.print("temp:");
      Serial.println(temp);
      Serial.print("hum:");
      Serial.println(hum);
      Serial.print("uv:");
      Serial.println(uv);

      return "{\"display\":" + (String)display + ",\"hcho\":" + (String)hcho + ",\"tvoc\":" + (String)tvoc + ",\"co2\":" + (String)co2 + ",\"temp\":" + (String)temp + ",\"hum\":" + (String)hum + ",\"uv\":" + (String)uv + "}";
    }

    return "";
  }

  String read() {
    // 帧数据
    byte buffer[_Frame_Len] = {};

    // 上一位
    byte last_byte = 0x00;

    // 下标
    uint8_t idx = 0;

    // 开始组装帧数据
    bool buffer_start = false;

    // 组装帧数据
    while (idx < _Frame_Len) {
      // 串口可用
      if (_serial.available() > 0) {
        // 按位读数
        int int_val = _serial.read();
        byte byte_val = (byte)int_val;

        // 识别起始（连续两个起始符）
        if (byte_val == _Frame_Start && last_byte == _Frame_Start) {
          // 标记开始
          buffer_start = true;

          buffer[0] = _Frame_Start;
          buffer[1] = _Frame_Start;

          idx = 2;

          Serial.println();
          Serial.print("[");
          Serial.print(buffer[0]);
          Serial.print(",");
          Serial.print(buffer[1]);
          Serial.print(",");
        } else {
          if (buffer_start) {
            buffer[idx] = byte_val;
            idx++;

            Serial.print(byte_val);
            if (idx < _Frame_Len) {
              Serial.print(",");
            }
          } else {
            last_byte = byte_val;
          }
        }
      }
    }

    Serial.println("]");
    Serial.println();

    return parse(buffer);
  }
} RX;

}