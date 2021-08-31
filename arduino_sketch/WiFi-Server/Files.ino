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
  if (file == "abstract.html")            {FileFound = Found(); if (RequestMethod == "GET") file_abstract(URI);}

  if (FileFound == false) {             // ファイルが見つからなかったら、404を返す
    Serial.println("ERR: Server message:");
    client.println("HTTP/1.1 404 Not Found");       Serial.println("HTTP/1.1 404 Not Found");
    client.println();                               Serial.println();
    client.println("404 Not Found"); Serial.println("404 Not Found");
    return;
  }
  else if (RequestMethod == "GET") Serial.println("{Response Body}"); // ファイルが見つかり、リクエストメソッドがGETの場合レスポンスボディーが送信されたことをログする。
}

bool Found() {      //ファイルが見つかったときの処理。
  Serial.println("INF: Server message:");
  client.println("HTTP/1.1 200 OK");              Serial.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");      Serial.println("Content-Type: text/html");
  client.println();                               Serial.println();
  return true;
}

bool pin32 = false; // ピン32がオンかオフか

const char index_html[] = // html前半部分
"<!DOCTYPE html><html><head><meta charset='UTF-8'><title>ESP32-WiFi</title></head>\
<body><p>Controller</p>\
<form method='get'><input type='submit' name='pin' value='IO32' /></form>\
<p><a href=\"abstract.html\">ピタドローン概要</a></p>";

const char abstract_html[] = 
"<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Pita Drone Abstract</title></head><body>\
Pita Droneはモジュールをピタッと取り付けるだけで簡単に機能の拡張ができるドローンです。\
</body>";

void file_index(String URI) {
  client.println(index_html);
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

void file_abstract(String URI) {
  client.println(abstract_html);  
}
