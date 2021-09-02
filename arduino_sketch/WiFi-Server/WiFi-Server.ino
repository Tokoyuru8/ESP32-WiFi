/*
 * ESP32-WiFi Web Server
 * ESP32向け簡易ウェブサーバー
 */

#include "WiFi-Server.h"

WiFiServer server(80);  // httpサーバーなのでポート80番を使います
WiFiClient client;

// Core0(PRO_CPU)は通信用、Core1(APP_CPU)はLED制御用に使います
// Core0のメインループ
void core0_task(void *pvParameters) {
  Serial.printf("INF: CPU Core %d started.\n", xPortGetCoreID());
  while (1) {
    HTTPListen();
    delay(1); // CPUの負担を減らすため
  }
  //vTaskDelete(NULL); // もし途中でループを脱しタスクを終了する場合、この関数を絶対呼ばなければならない（呼ばないとpanicになる）
}

void setup() { // Core1で実行される
  Serial.begin(115200);
  Serial.println();
  Serial.printf("INF: CPU Core %d started.\n", xPortGetCoreID());
  Serial.println("INF: Initializing the ESP32-WiFi web server...");

  // PWM制御の設定
  pinMode(PWM_PIN0, OUTPUT);
  pinMode(PWM_PIN1, OUTPUT);
  pinMode(PWM_PIN2, OUTPUT);
  pinMode(PWM_PIN3, OUTPUT);
  ledcSetup(PWM_CNL0, PWM_FRQ, PWM_BIT);
  ledcSetup(PWM_CNL1, PWM_FRQ, PWM_BIT);
  ledcSetup(PWM_CNL2, PWM_FRQ, PWM_BIT);
  ledcSetup(PWM_CNL3, PWM_FRQ, PWM_BIT);
  ledcAttachPin(PWM_PIN0, PWM_CNL0);
  ledcAttachPin(PWM_PIN1, PWM_CNL1);
  ledcAttachPin(PWM_PIN2, PWM_CNL2);
  ledcAttachPin(PWM_PIN3, PWM_CNL3);

  // WiFi、Webサーバーの設定
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, subnet);
  IPAddress myIP = WiFi.softAPIP(); // WiFi通信始動
  server.begin();                   // ウェブサーバー始動

  // Core0開始
  xTaskCreateUniversal(core0_task, "core0_task", 8192, NULL, 1, NULL, PRO_CPU_NUM);
  disableCore0WDT();
  
  Serial.print("INF: AP IP address: ");
  Serial.println(myIP);
  Serial.println("INF: Successfully initialized the ESP32-WiFi web server!");
}

int led_mode = 0; // 0:滑らかに点滅、1:手動で明るさを設定、2:姿勢情報から自動で明るさ設定（未実装）
int i = 0, diff = 1, led0 = 0, led1 = 0, led2 = 0, led3 = 0;

void loop(){ // Core1で実行される
  switch (led_mode) {
  case 0:
    ledcWrite(PWM_CNL0, i/2);
    ledcWrite(PWM_CNL1, i/2);
    ledcWrite(PWM_CNL2, i/2);
    ledcWrite(PWM_CNL3, i/2);
    if (i == 1023*2) diff = -1;
    if (i == 0) diff = 1;
    i += diff;
    break;
  case 1:
    break;
  case 2:
    break;
  }
  delay(1);
}

void HTTPListen() {                               // HTTPメッセージを受信。Core0で実行される。
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
          break;                                  // whileループを抜け、通信終了
        }
        if (c == '\n') NewLine = true;            // 改行した直後はNewLineフラグを立てておく（'/n'の連続を検知するため）。
        else if (c != '\r') NewLine = false;      // 改行でも'/r'でも無い場合NewLineフラグはfalse。
      }
    }
    
    // 通信の終了処理
    client.println();
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
    if (i >= 100) {                                           // URIが長すぎた場合のエラー処理
      Serial.println("ERR: Server message:");
      client.println("HTTP/1.1 414 URI Too Long");  Serial.println("HTTP/1.1 414 URI Too Long");
      client.println();                             Serial.println();
      client.println("414 URI Too Long");           Serial.println("414 URI Too Long");
      return;
    }
  }
  
  Serial.print("DBG: Request Method: ");  Serial.println(RequestMethod);
  Serial.print("DBG: URI: ");             Serial.println(URI);
  
  if (RequestMethod == "GET" || RequestMethod == "HEAD") Root(URI, RequestMethod);
  else {                                         // GET、HEAD以外の場合のエラー処理
    Serial.println("ERR: Server message:");
    client.println("HTTP/1.1 501 Not Implemented"); Serial.println("HTTP/1.1 501 Not Implemented");
    client.println();                               Serial.println();
    client.println("501 Not Implemented");          Serial.println("501 Not Implemented");
    return;
  }
}

bool Found() {      //ファイルが見つかったときの処理。
  Serial.println("INF: Server message:");
  client.println("HTTP/1.1 200 OK");              Serial.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");      Serial.println("Content-Type: text/html");
  client.println();                               Serial.println();
  return true;
}
