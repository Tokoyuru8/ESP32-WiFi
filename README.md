# 概要
ESP32をWiFiアクセスポイント＆ウェブサーバー&PWM制御機として使用するための試作プログラムです。  
GPIO25,26,32,33ピンにPWM信号を送ります。PWM信号は、ブラウザで192.168.0.1に接続し設定を行うことで制御ができます。  

# ファイルの説明
WiFi-Server.ino : WiFi通信、Webサーバー、LEDのPWM制御プログラムなど。  
WiFi-Server.h   : 定数の定義や、ライブラリのインクルードなど。  
Files.ino       : ブラウザから要求されたファイルがあるかどうか確認し、htmlを送信します。  

# 主な関数の説明
setup(),loop()  : CPU Core 1で実行されます。初期化を行ったのち、LED制御を行います。  
core0_task()    : CPU Core 0で実行されます。Core 1のloop()関数に相当します。HTTPListen()を呼び出し続けます。  
HTTPListen()    : CPU Core 0で実行されます。クライアントからデータを受け取り、データをHTTPPursuer()に渡します。  
HTTPPursuer()   : CPU Core 0で実行されます。HTTPListen()から受け取ったデータからHTTPのリクエストメソッドを抽出し、対応します。GETやHEADの場合、Root()関数が呼び出されます。  
Root()          : CPU Core 0で実行されます。URIで指定されたファイルが存在するか確認し、存在する場合htmlの送信などを行う関数を呼び出します。  

# 早見
シリアルモニターのビットレート：115200bps（WiFi-Server.inoで定義）  
SSIDとパスワード：WiFi-Server.hで定義されています。それぞれ"PitaDrone"と"PitaDrone123"です。  
接続方法：ブラウザのURL記入欄に192.168.0.1を入力してください。  
Arduinoのボード設定：NodeMCU-32S  
