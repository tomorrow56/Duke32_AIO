/*
  duke32.cpp - Duke32AIO Library Implementation
  
  Copyright (c) 2025 tomorrow56. All rights reserved.
  SPDX-License-Identifier: MIT
*/

#include "duke32.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "driver/ledc.h"

/********************************
* for Wifi Connection
*********************************/
// Add Feature String
void AddFeatureString(String &S, const String F){
  if (S.length() != 0) S.concat(", ");
  S.concat(F);
}

uint64_t GetChipid(){
  // Chip Info
  const char* ModelStrings[] PROGMEM = {"", "ESP32"};
  uint64_t chipid;
  // Get Chip Information
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  Serial.printf("Model: %s\r\n", ModelStrings[chip_info.model]);

  // Features
  String Features = "";
  if (chip_info.features & CHIP_FEATURE_EMB_FLASH) AddFeatureString(Features, "Embedded Flash");
  if (chip_info.features & CHIP_FEATURE_WIFI_BGN ) AddFeatureString(Features, "Wifi-BGN"      );
  if (chip_info.features & CHIP_FEATURE_BLE      ) AddFeatureString(Features, "BLE"           );
  if (chip_info.features & CHIP_FEATURE_BT       ) AddFeatureString(Features, "Bluetooth"     );
  Serial.println("Features: " + Features);
  // Cores
  Serial.printf("Cores: %d\r\n", chip_info.cores);
  // Revision
  Serial.printf("Revision: %d\r\n", chip_info.revision);
  // MAC Address
  String MACString = "";
  chipid = ESP.getEfuseMac();
  for (int i=0; i<6; i++) {
    if (i > 0) MACString.concat(":");
    uint8_t Octet = chipid >> (i * 8);
    if (Octet < 16) MACString.concat("0");
    MACString.concat(String(Octet, HEX));
  }
  Serial.println("MAC Address: " + MACString);
  // Flash Size
  uint32_t FlashSize = ESP.getFlashChipSize();
  String ValueString = "";
  do {
    ValueString = String(FlashSize % 1000) + ValueString;
    FlashSize /= 1000;
    if (FlashSize > 0)
      ValueString = "," + ValueString;
  } while (FlashSize > 0);
  Serial.println("Flash Size: " + ValueString);
  // ChipID
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  return chipid;
}

void WiFiMgrSetup(char WiFiAPname[]){
  //SimpleWiFiManager
  Serial.printf("AP: %s\n", WiFiAPname);
  Serial.println("IP: 192.168.4.1");

  //Local intialization. Once its business is done, there is no need to keep it around
  SimpleWiFiManager wifiManager;

  // WebUIのタイトルを設定
  wifiManager.setWebUITitle("Duke32 WiFi Manager");
  // テーマをライトモードに設定
  wifiManager.setWebUITheme(WM_WEBUI_THEME_LIGHT);
  // テーマをダークモードに設定
  // wifiManager.setWebUITheme(WM_WEBUI_THEME_DARK);

  // WiFi接続試行のタイムアウトを10秒に設定
  wifiManager.setConnectTimeout(10);

  //or use this for auto generated name ESP + ChipID
  wifiManager.autoConnect(WiFiAPname);

  //if you get here you have connected to the WiFi
  Serial.println("connected(^^)");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.localIP());
}

void FwUploaderSetup(){
  // ESP32FwUploaderの設定
  ESP32FwUploader.setDebug(true);
  ESP32FwUploader.setDarkMode(false); // ライトモードを有効化
  
  // コールバックの設定
  ESP32FwUploader.onStart([]() {
    Serial.println("Firmware Update Started!");
  });
  
  ESP32FwUploader.onProgress([](size_t current, size_t total) {
    Serial.printf("Progress: %.1f%%\n", (float)current / total * 100.0);
  });
  
  ESP32FwUploader.onEnd([](bool success) {
    if (success) {
      Serial.println("Firmware Update Successful!");
    } else {
      Serial.println("Firmware Update Failed!");
    }
  });
  
  ESP32FwUploader.onError([](ESP32Fw_Error error, const String& message) {
    Serial.printf("Update Error: %s\n", message.c_str());
  });
  
  Serial.println("ESP32FwUploader initialized");
  Serial.println("Open http://" + WiFi.localIP().toString() + "/update");
}

/********************************
* for Servo Control
*********************************/
void servoInit(){
  // Setup timer and attach timer to a servo pin
  #if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    (void)ledcAttach(Servo_PIN1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
    (void)ledcAttach(Servo_PIN2, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  #else
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
    ledcAttachPin(Servo_PIN1, LEDC_CHANNEL_0);

    ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
    ledcAttachPin(Servo_PIN2, LEDC_CHANNEL_1);
  #endif
}

void setServo(uint8_t servoNo, uint16_t angle){
  uint16_t pwmWidth;

  if(servoNo < 0){
    servoNo = 0;
  }else if(servoNo > 1){
    servoNo = 1;
  }

  if(angle < 0){
    angle = 0;
  }else if(angle > 180){
    angle = 180;
  }

  pwmWidth = ((srvMax - srvMin) / 180) * angle + srvMin;

  #if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    if (servoNo == 0U) {
      (void)ledcWrite(Servo_PIN1, pwmWidth);
    } else {
      (void)ledcWrite(Servo_PIN2, pwmWidth);
    }
  #else
    ledcWrite(servoNo, pwmWidth);
  #endif
}

/********************************
* for Motor Control
*********************************/
void Motor_INIT(){
  pinMode(MTR_A1, OUTPUT);
  pinMode(MTR_A2, OUTPUT);
//  pinMode(MTR_AE, OUTPUT);
  pinMode(MTR_B1, OUTPUT);
  pinMode(MTR_B2, OUTPUT);
//  pinMode(MTR_BE, OUTPUT);

  digitalWrite(MTR_A1, LOW);
  digitalWrite(MTR_A2, LOW);
//  digitalWrite(MTR_AE, LOW);
  digitalWrite(MTR_B1, LOW);
  digitalWrite(MTR_B2, LOW);
//  digitalWrite(MTR_BE, LOW);

  // Setup timer and attach timer to a servo pin
  #if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    (void)ledcAttach(MTR_AE, LEDC_BASE_FREQ_MTR, LEDC_TIMER_BIT_MTR);
    (void)ledcAttach(MTR_BE, LEDC_BASE_FREQ_MTR, LEDC_TIMER_BIT_MTR);
  #else
    ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ_MTR, LEDC_TIMER_BIT_MTR);
    ledcAttachPin(MTR_AE, LEDC_CHANNEL_2);

    ledcSetup(LEDC_CHANNEL_3, LEDC_BASE_FREQ_MTR, LEDC_TIMER_BIT_MTR);
    ledcAttachPin(MTR_BE, LEDC_CHANNEL_3);
  #endif
}

void setMotorSpeed(uint8_t motorNo, uint8_t speed){
  if(motorNo < 2){
    motorNo = 2;
  }else if(motorNo > 3){
    motorNo = 3;
  }

  if(speed < 0){
    speed = 0;
  }else if(speed > 255){
    speed = 255;
  }

  #if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    if (motorNo == MotorL) {
      (void)ledcWrite(MTR_AE, speed);
    } else {
      (void)ledcWrite(MTR_BE, speed);
    }
  #else
    ledcWrite(motorNo, speed);
  #endif
}

void motorL_forward(uint8_t speed){
//  digitalWrite(MTR_AE, HIGH);
  setMotorSpeed(MotorL, speed);
  digitalWrite(MTR_A1, HIGH);
  digitalWrite(MTR_A2, LOW);
}

void motorL_back(uint8_t speed){
//  digitalWrite(MTR_AE, HIGH);
  setMotorSpeed(MotorL, speed);
  digitalWrite(MTR_A1, LOW);
  digitalWrite(MTR_A2, HIGH);
}

void motorL_stop(){
//  digitalWrite(MTR_AE, LOW);
  setMotorSpeed(MotorL, 0);
  digitalWrite(MTR_A1, LOW);
  digitalWrite(MTR_A2, LOW);
}

void motorR_forward(uint8_t speed){
//  digitalWrite(MTR_BE, HIGH);
  setMotorSpeed(MotorR, speed);
  digitalWrite(MTR_B1, HIGH);
  digitalWrite(MTR_B2, LOW);
}
void motorR_back(uint8_t speed){
//  digitalWrite(MTR_BE, HIGH);
  setMotorSpeed(MotorR, speed);
  digitalWrite(MTR_B1, LOW);
  digitalWrite(MTR_B2, HIGH);
}

void motorR_stop(){
//  digitalWrite(MTR_BE, LOW);
  setMotorSpeed(MotorR, 0);
  digitalWrite(MTR_B1, LOW);
  digitalWrite(MTR_B2, LOW);
}

void motor_stop(){
  motorL_stop();
  motorR_stop();
}

void motor_forward(){
  motorL_forward(255);
  motorR_forward(255);
}

void motor_back(){
  motorL_back(255);
  motorR_back(255);
}

void motor_left(){
  motorL_forward(128);
  motorR_forward(255);
}

void motor_right(){
  motorL_forward(255);
  motorR_forward(128);
}

void motor_turnleft(){
  motorL_stop();
  motorR_forward(255);
}

void motor_turnright(){
  motorL_forward(255);
  motorR_stop();
}

void motor_spinleft(){
  motorL_back(255);
  motorR_forward(255);
}

void motor_spinright(){
  motorL_forward(255);
  motorR_back(255);
}

// --------------------------------------
// i2c_scanner
// --------------------------------------

void i2c_scan(){
  Serial.println("\nI2C Scanner");
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16){
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16){
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0){
    Serial.println("No I2C devices found\n");
  }else{
    Serial.println("done\n");
  }
}