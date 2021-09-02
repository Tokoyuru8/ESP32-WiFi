/*
 * Main.ino
 * index.htmlに相当する部分。client.printを使用してhtmlを記述する。
 * HTMLのフォームに応じた処理などもここに書く
 */

#include "WiFi-Server.h"

void Root(String URI, String RequestMethod) {
  bool FileFound = false;                             // URIで指定されたファイルが見つかったらtrue
  String file = "";                                   // ファイル名抽出用
  for (int i = 1; (URI[i] != '?') && (URI[i] != '/') && (i < URI.length()); i++) file += URI[i];  // ファイル名を抽出する（URIの文字列のうち、?や/の直前までをファイル名とする）

  // ファイルを追加する際は、下のテンプレートをコピペして使う。{file_name}はURIで指定するファイル名、{function_name}は{file_name}が指定されたときに呼び出す関数。
  //if (file == "{file_name}")              {FileFound = Found(); if (RequestMethod == "GET") {function_name}(URI);}
  if (file == "" || file == "index.html") {FileFound = Found(); if (RequestMethod == "GET") file_index(URI);}

  if (FileFound == false) {             // ファイルが見つからなかったら、404を返す
    Serial.println("ERR: Server message:");
    client.println("HTTP/1.1 404 Not Found");       Serial.println("HTTP/1.1 404 Not Found");
    client.println();                               Serial.println();
    client.println("404 Not Found"); Serial.println("404 Not Found");
    return;
  }
  else if (RequestMethod == "GET") Serial.println("{Response Body}"); // ファイルが見つかり、リクエストメソッドがGETの場合レスポンスボディーが送信されたことをログする。
}

const char index_html1[] =
"<!DOCTYPE html>\n\
<html>\n\
  <head>\n\
    <meta charset='UTF-8'>\n\
    <title>ESP32-WiFi</title>\n\
  </head>\n\
  <body>\n\
    <p>Controller</p>\n\
    <form method='get'>\n\
      <p>モードセレクタ</p>\n\
      <input type='submit' name='MODE' value='MODE0' />\n\
      <input type='submit' name='MODE' value='MODE1' />\n\
      <input type='submit' name='MODE' value='MODE2' /><br>\n\
      <p>モード1用操作パネル</p>\n\
      <input type='submit' name='MODE1' value='LED0___UP' />\n\
      <input type='submit' name='MODE1' value='LED1___UP' />\n\
      <input type='submit' name='MODE1' value='LED2___UP' />\n\
      <input type='submit' name='MODE1' value='LED3___UP' /><br>\n\
      <input type='submit' name='MODE1' value='LED0_DOWN' />\n\
      <input type='submit' name='MODE1' value='LED1_DOWN' />\n\
      <input type='submit' name='MODE1' value='LED2_DOWN' />\n\
      <input type='submit' name='MODE1' value='LED3_DOWN' /><br>\n";
const char index_html2[] =
"   </form>\n\
    <p><a href=\"abstract.html\">ピタドローン概要</a></p>\n";

void file_index(String URI) {
  client.println(index_html1);
  if (URI.endsWith("?MODE=MODE0")) {
    led_mode = 0;
    i = 0;
  } else if (URI.endsWith("?MODE=MODE1")) {
    led_mode = 1;
  } else if (URI.endsWith("?MODE=MODE2")) {
    led_mode = 2;
  } else if (URI.endsWith("?MODE1=LED0___UP")) {
    led0 += 1;
  } else if (URI.endsWith("?MODE1=LED1___UP")) {
    led1 += 1;
  } else if (URI.endsWith("?MODE1=LED2___UP")) {
    led2 += 1;
  } else if (URI.endsWith("?MODE1=LED3___UP")) {
    led3 += 1;
  } else if (URI.endsWith("?MODE1=LED0_DOWN")) {
    led0 += -1;
  } else if (URI.endsWith("?MODE1=LED1_DOWN")) {
    led1 += -1;
  } else if (URI.endsWith("?MODE1=LED2_DOWN")) {
    led2 += -1;
  } else if (URI.endsWith("?MODE1=LED3_DOWN")) {
    led3 += -1;
  }

  if (led_mode == 1) {
  ledcWrite(PWM_CNL0, led0);
  ledcWrite(PWM_CNL1, led1);
  ledcWrite(PWM_CNL2, led2);
  ledcWrite(PWM_CNL3, led3);
  }
  client.print("<br>LED0:");  client.println(led0);
  client.print("<br>LED1:");  client.println(led1);
  client.print("<br>LED2:");  client.println(led2);
  client.print("<br>LED3:");  client.println(led3);
  client.println("</body></html>"); // html後半部分
}
