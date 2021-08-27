/*
 * ESP32-WiFi Web Server
 * ESP32向け簡易ウェブサーバー
 */

#include "WiFi-Server.h"

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("INF: Initializing the ESP32-WiFi web server...");

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, subnet);
  IPAddress myIP = WiFi.softAPIP(); // WiFi通信始動
  server.begin();                   // ウェブサーバー始動
  Serial.print("INF: AP IP address: ");
  Serial.println(myIP);
  Serial.println("INF: Successfully initialized the ESP32-WiFi web server!");
  Serial.println("INF: Waiting for a client...");
}

void loop() {
  HTTPListen();
}

void HTTPListen() {                               // HTTPメッセージを受信。
  client = server.available();                    // クライアントの接続を待機。
  if (client) {                                   // クライアントと接続した場合実行。
    Serial.println("INF: Client connected.");
    bool NewLine = true;                          // 直前に受け取ったメッセージが'/n'のときはtrue。
    String ReceiveMessage = "";                   // 受信メッセージの保管用。
    while (client.connected()) {                  // クライアントが接続されている間ループ。
      if (client.available()) {                   // クライアントから送信されてきたデータがあれば、
        char c = client.read();                   // 一文字ずつ読み取り、ReceiveMessageに保管する。
        ReceiveMessage += c;
        if (c == '\n' && NewLine) {               // '/n'が連続した場合はHTTPメッセージの終わりなので、
          Serial.println("Client message:");
          Serial.println(ReceiveMessage);
          HTTPPursuer(ReceiveMessage);            // HTTPパーサーでメッセージを解読し対応する。
        }
        if (c == '\n') NewLine = true;            // 改行した直後はNewLineフラグを立てておく（'/n'の連続を検知するため）。
        else if (c != '\r') NewLine = false;      // 改行でも'/r'でも無い場合NewLineフラグはfalse。
      }
    }
    client.println();                             // 通信の終了処理
    delay(1);
    client.stop();
    Serial.println("INF: Client disconnected.");
  }
}

void HTTPPursuer(String ReceiveMessage) { // リクエストメソッドを解析して対応
  int i = 0;
  String RequestMethod = "", URI = "";
  for (; ReceiveMessage[i] != ' ' && i < ReceiveMessage.length(); i++) { // リクエストメソッドの抽出
    RequestMethod += ReceiveMessage[i];
  }
  for (i += 1; ReceiveMessage[i] != ' ' && i < ReceiveMessage.length(); i++) { // URIの抽出
    URI += ReceiveMessage[i];
    if (i == 100) {                                           // URIが長すぎた場合のエラー処理
      client.println("HTTP/1.1 414 URI Too Long");
      client.println();
      Serial.println("ERR: Server message:");
      Serial.println("HTTP/1.1 414 URI Too Long");
      Serial.println();
      return;
    }
  }
  Serial.print("INF: Request Method: ");
  Serial.println(RequestMethod);
  Serial.print("INF: URI: ");
  Serial.println(URI);
  if (RequestMethod == "GET" || RequestMethod == "HEAD") { // GET、HEAD共通部分の処理
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    Serial.println("INF: Server message:");
    Serial.println("HTTP/1.1 200 OK");
    Serial.println("Content-Type: text/html");
    Serial.println();
    if (RequestMethod == "GET") {
      Main(URI);         // GETの場合、htmlの送信、その他ロジックの実行などを行う。
      Serial.println("{Response Body}");
    }
  } else {                                         // GET、HEAD以外の場合のエラー処理
    client.println("HTTP/1.1 501 Not Implemented");
    client.println();
    Serial.println("ERR: Server message:");
    Serial.println("HTTP/1.1 501 Not Implemented");
    Serial.println();
    return;
  }
}

/* 公式レファレンスに乗っていた、役に立ちそうな機能
void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
} 
*/
