# Duke32_AIO

Bit Trade Oneで販売中の多機能リモコンボード「Duke32AIO」のサポートページです。

## 製品情報・購入はこちら
https://btoshop.jp/products/admu32f

## 各フォルダの内容

### DefaultFW
- デフォルトで書き込んであるファームウエア(バイナリ)
  - Local AP接続でWebブラウザよりコントロールできます
  - ファームウエアのバックアップとリストア用スクリプトも同じフォルダにあります

### examples/ESP32_WebserverSample
- アップデート版のファームウエアのソースコード
  - Webサーバ部分を別ファイルに分離して、読み込んで使用しています
  - #defineの切替で以下の機能のON/OFFが出来ます
    - Local IPと外部AP接続でのNet経由接続の切替
    - ファームウェア更新機能（WebベースのOTA）
    - 0.96"のOLED(I2C接続)を使用して画面表示
    - シリアルLED(WS2812B互換)のON/OFFと数の変更
    
### 無料のスクラッチベース プログラミングアプリ「つくるっち」対応
以下よりダウンロードできます
http://sohta02.web.fc2.com/familyday_about.html

## 必要な外部ライブラリ

このライブラリを使用するには、以下の外部ライブラリを別途インストールする必要があります。

### SimpleWiFiManager
WiFi設定を保存し、キャプティブポータル経由で簡単にネットワーク接続を設定するライブラリです。

- GitHub: https://github.com/tomorrow56/SimpleWiFiManager
- インストール方法:
  1. 上記GitHubリポジトリにアクセス
  2. 「Code」→「Download ZIP」でZIPファイルをダウンロード
  3. ArduinoIDEの「スケッチ」→「ライブラリをインクルード」→「.ZIP形式のライブラリをインストール」を選択
  4. ダウンロードしたZIPファイルを選択してインストール

### ESP32FwUploader
WebベースのOTA（Over-The-Air）ファームウェア更新機能を提供するライブラリです。

- GitHub: https://github.com/tomorrow56/ESP32FwUploader
- インストール方法:
  1. 上記GitHubリポジトリにアクセス
  2. 「Code」→「Download ZIP」でZIPファイルをダウンロード
  3. ArduinoIDEの「スケッチ」→「ライブラリをインクルード」→「.ZIP形式のライブラリをインストール」を選択
  4. ダウンロードしたZIPファイルを選択してインストール

## ライブラリの使い方

### インストール方法
1. このリポジトリをダウンロードまたはクローンします
2. ArduinoIDEの「スケッチ」→「ライブラリをインクルード」→「.ZIP形式のライブラリをインストール」を選択
3. ダウンロードしたZIPファイルを選択してインストール

### 基本的な使い方
```cpp
#include <duke32.h>

void setup() {
  // モーター制御の初期化
  setMotorSpeed(MotorL, 0);  // 左モーター停止
  setMotorSpeed(MotorR, 0);  // 右モーター停止
  
  // サーボ制御
  servo_write(Servo_PIN1, 90);  // サーボを中央位置に
  
  // LED制御
  led_write(0, 255, 0, 0);  // 赤色LED点灯
  
  // WiFi接続
  WiFiMgrSetup("Duke32-AP");
  
  // ファームウェアアップローダー設定
  FwUploaderSetup();
  ESP32FwUploader.begin(&server);
}

void loop() {
  // 前進
  motorL_forward(128);
  motorR_forward(128);
  delay(2000);
  
  // 停止
  motor_stop();
  delay(1000);
  
  // ファームウェアアップローダーの処理
  ESP32FwUploader.loop();
}
```

### 主な機能
- **モーター制御**: `motorL_forward()`, `motorR_back()`, `motor_stop()`など
- **サーボ制御**: `servo_write()`, `servo_write_us()`
- **LED制御**: `led_write()`でWS2812B LEDを制御
- **WiFi機能**: `WiFiMgrSetup()`でSimpleWiFiManagerを使用したネットワーク接続
- **ファームウェア更新**: `FwUploaderSetup()`, `ESP32FwUploader`でWebベースのOTA更新
- **Webサーバー**: ブラウザからのリモートコントロールに対応

### サンプルコード
`examples/ESP32_WebserverSample/`にWebサーバー経由で制御する完全なサンプルが含まれています。

## APIリファレンス

### WiFi関連
- `uint64_t GetChipid()` - ESP32のチップIDを取得
- `void WiFiMgrSetup(char WiFiAPname[])` - SimpleWiFiManagerをセットアップ、APモードで起動

### ファームウェア更新
- `void FwUploaderSetup()` - ESP32FwUploaderを初期化、WebベースのOTA更新を有効化
- `ESP32FwUploader.begin(&server)` - Webサーバーにファームウェアアップローダーを登録
- `ESP32FwUploader.loop()` - ファームウェアアップローダーの処理を実行（loop()内で呼び出し）

### モーター制御
- `void setMotorSpeed(uint8_t motor, uint8_t speed)` - 指定したモーターの速度を設定（0-255）
  - `motor`: `MotorL`（左）または `MotorR`（右）
  - `speed`: 0（停止）から255（最大速度）
- `void motorL_forward(uint8_t speed)` - 左モーターを前進させ、速度を指定
- `void motorL_back(uint8_t speed)` - 左モーターを後退させ、速度を指定
- `void motorL_stop()` - 左モーターを停止
- `void motorR_forward(uint8_t speed)` - 右モーターを前進させ、速度を指定
- `void motorR_back(uint8_t speed)` - 右モーターを後退させ、速度を指定
- `void motorR_stop()` - 右モーターを停止
- `void motor_stop()` - 両方のモーターを停止

### サーボ制御
- `void servo_write(uint8_t pin, uint8_t angle)` - サーボを角度で制御（0-180度）
  - `pin`: サーボのピン番号（`Servo_PIN1`または`Servo_PIN2`）
  - `angle`: 0度から180度までの角度
- `void servo_write_us(uint8_t pin, uint16_t us)` - サーボをパルス幅で制御（500-2400μs）
  - `us`: パルス幅（マイクロ秒単位）

### LED制御
- `void led_write(uint16_t index, uint8_t r, uint8_t g, uint8_t b)` - WS2812B LEDを制御
  - `index`: LEDのインデックス（0から始まる）
  - `r, g, b`: 赤、緑、青の輝度（0-255）

### 定数
- `NEO_PIN` - LEDのデータピン（27）
- `Servo_PIN1` - 1番目のサーボピン（23）
- `Servo_PIN2` - 2番目のサーボピン（4）
- `MotorL` - 左モーターの識別子
- `MotorR` - 右モーターの識別子
