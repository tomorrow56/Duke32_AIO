/******************************************
* Duke32-AIO FW書き込みスクリプトについて *
*                              2020/05/13 *
*              Masao Yamazaki(@tomorrow56)*
*******************************************/

<フォルダの内容>
readme.txt                  このファイル
esptool.exe                 書込みソフト本体
esp32_BTO_firmware_v1.bin   デフォルトFW
Firmware_Backup_dd.cmd      FWバックアップ用スクリプト
Firmware_Restore_dd.cmd     FW書き込み用スクリプト

<動作確認環境>
Windows10 PC (FTDIドライバ導入済)
USBポート: USB2.0を推奨(3.0では書きこめない・速度が出ないケースが稀にあり)

<書込み前準備>
1. ボードをMicro USBケーブル(書き込み対応)で接続
2. シリアルポートとして認識されるので、デバイスマネジャー等でポート番号を確認
3. Firmware_Restore_dd.cmd の以下の部分を接続されているポート番号に修正
 @echo off
 SET COM_PORT=COM8  <- ここ

<書込み手順>
1. 同一フォルダに以下のファイルを格納する
  esptool.exe
  esp32_BTO_firmware_v1.bin
  Firmware_Restore_dd.cmd

2. PCとボードをUSBケーブルで接続する

3. Windowsのエクスプローラで1.のフォルダを開く

4. Firmware_Restore_dd.cmd に esp32_BTO_firmware_v1.bin をドラッグドロップする

5. コマンドプロンプトが開き書込みが始まるので、終了するまで待つ
   (約30秒程度で書き込みは完了する)

<追加: 書き込んであるFWのバックアップ>
1. Firmware_Backup_dd.cmd の以下の部分を接続されているポート番号に修正
 @echo off
 SET COM_PORT=COM8  <- ここ

2. 同一フォルダに以下のファイルを格納する
  esptool.exe
  Firmware_Backup_dd.cmd

3. バックアップしたいファイル名の空ファイルを作成する
  - テキストエディタでファイルを新規作成して中身が空のまま名前を適当につけるのが簡単
  - ファイル名の拡張子は*.binがおすすめ

4. PCとボードをUSBケーブルで接続する

5. Windowsのエクスプローラで1.のフォルダを開く

4. Firmware_Backup_dd.cmd に 上記で作成した空ファイルをドラッグドロップする
  - 間違って別ファイルをドラッグドロップすると無条件で上書きされるので注意

5. コマンドプロンプトが開きバックアップが始まるので、終了するまで待つ
   (約60秒程度でバックアップは完了する)

6. バックアップファイルのサイズが"4096KB"であればバックアップは完了



