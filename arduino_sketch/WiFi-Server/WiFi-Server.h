/*
 * WiFi-Server.h
 * インクルード、宣言、ネットワーク設定など
 */

#ifndef WIFISERVER_H
#define WIFISERVER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

/* APのSSID、パスワード、ネットワークアドレス兼ゲートウェイアドレス、サブネットマスク
 * 注：AP自身のIPアドレスは多分192.168.0.2になる */
const char *ssid = "PitaDrone";
const char *password = "PitaDrone123";
const IPAddress ip(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80); // httpサーバーなのでポート80番を使います
WiFiClient client;

#endif
