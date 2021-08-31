# 概要
ESP32をWiFiアクセスポイント＆ウェブサーバーとして使用するための試作プログラムです。  

# ファイルの説明
WiFi-Server.ino : setup()とloop()があります。WiFi接続の確立、HTTP通信の処理などの裏方作業が担当です。  
WiFi-Server.h   : 定数の定義や、ライブラリのインクルードなどをしています。  
Main.ino        : クライアントにhtmlを送信したり、ボタンが押された時の処理などをします。

# 関数の説明
HTTPListen()    : クライアントからデータを受け取ります。受け取ったデータはHTTPPursuer()にStringで渡します。  
HTTPPursuer()   : HTTPListen()から受け取ったデータからHTTPのリクエストメソッドを抽出し、適切に対応します。GETの場合、レスポンスボディーを送信するためにRoot()関数が呼び出されます。また、この時引数としてURIが渡されます。  
Root()          : URIで指定されたファイルが存在するか確認し、存在する場合htmlの送信などを行う関数を呼び出します。今回はサンプルとして、index.htmlとabstract.htmlをファイルとして登録し、index.htmlを送信するfile\_indexではボタンを押すとIO32がオンになったりオフになったりするプログラムを書きました。  

# 早見
シリアルモニターのビットレート：115200bps（WiFi-Server.inoで定義）  
SSIDとパスワード：WiFi-Server.hで定義されています。それぞれ"PitaDrone"と"PitaDrone123"です。  
接続方法：ブラウザのURL記入欄に192.168.0.1を入力してください。 
Arduinoのボード設定：NodeMCU-32S
