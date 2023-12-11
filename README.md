# Duke32_AIO

Bit Trade Oneで多機能リモコンボード「Duke32AIO」のサポートページです。

## 製品情報・購入はこちら
https://btoshop.jp/products/admu32f

### 2020年8月4日時点で公開しているソースコードは以下になります。
- デフォルトで書き込んであるファームウエア(バイナリ)
  - Local AP接続でWebブラウザよりコントロールできます
  - ファームウエアのバックアップとリストア用スクリプトも同じフォルダにあります
- デフォルトファームウエアのソースコード(Arduino Sketch)
  - Webサーバ部分を別ファイルに分離して、読み込んで使用しています
  - #defineの切替で以下の機能のON/OFFが出来ます
    - Local IPと外部AP接続でのNet経由接続の切替
    - OTA機能
    - 0.96"のOLED(I2C接続)を使用して画面表示
    - シリアルLED(WS2812B互換)のON/OFFと数の変更
    
### 無料のスクラッチベース プログラミングアプリ「つくるっち」対応
以下よりダウンロードできます
http://sohta02.web.fc2.com/familyday_about.html
