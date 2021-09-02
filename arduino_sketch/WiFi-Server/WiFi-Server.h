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

const int PWM_FRQ  = 78125; // 分解能が10bitの場合の最大周波数
const int PWM_BIT  = 10;    // 1024段階のPWM制御
const int PWM_CNL0 = 0;
const int PWM_CNL1 = 1;
const int PWM_CNL2 = 2;
const int PWM_CNL3 = 3;
const int PWM_PIN0 = 26;    // GPIO26
const int PWM_PIN1 = 25;    // GPIO25
const int PWM_PIN2 = 33;    // GPIO33
const int PWM_PIN3 = 32;    // GPIO32

#endif
