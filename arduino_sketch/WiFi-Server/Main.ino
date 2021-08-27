/*
 * Main.ino
 * index.htmlに相当する部分。client.printを使用してhtmlを記述する。
 * HTMLのフォームに応じた処理などもここに書く
 */

#include "WiFi-Server.h"

bool pin32 = false; // ピン32がオンかオフか

const char html[] = // html前半部分
"<!DOCTYPE html><html><head><meta charset='UTF-8'><title>ESP32-WiFi</title></head>\
<body><p>Controller</p>\
<form method='get'>\
<input type='submit' name='pin' value='IO32' />\
</form></div>";

void Main(String URI) {
  client.println(html);
  if(URI.endsWith("?pin=IO32")) { // ボタンを押すとIO32がオン、オフになる。
    Serial.println("INF: Button was pushed.");
    if (pin32 == false) {
      digitalWrite(32, HIGH);
      pin32 = true;
      client.println("<br>IO32がオンになりました。");
      Serial.println("INF: IO32 turned ON.");
    } else {
      digitalWrite(32, LOW);
      pin32 = false;
      client.println("<br>IO32がオフになりました。");
      Serial.println("INF: IO32 turned OFF.");
    }
  }
  client.println("</body></html>"); // html後半部分
}
