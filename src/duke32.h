/*
  duke32.h - Duke32AIO Library Header
  
  Copyright (c) 2025 tomorrow56. All rights reserved.
  SPDX-License-Identifier: MIT
*/

#ifndef _duke32_h_
#define _duke32_h_

/********************************
* for Wifi Connection
*********************************/
#include <WiFi.h>
// https://github.com/zhouhan0126/DNSServer---esp32
#include <DNSServer.h>
// https://github.com/zhouhan0126/WebServer-esp32
#include <WebServer.h>
#include <ESPmDNS.h>
// SimpleWiFiManager
 #include <SimpleWiFiManager.h>

#include <WiFiUdp.h>
#include <ESP32FwUploader.h>

#include <Wire.h>

uint64_t GetChipid();
void WiFiMgrSetup(char WiFiAPname[]);
void FwUploaderSetup();

/********************************
* for LED Control
*********************************/
#define NEO_PIN     27

/********************************
* for Servo Control
*********************************/
#define Servo_PIN1  23
#define Servo_PIN2   4

#define LEDC_CHANNEL_0  0  // channel of 16 channels (started from zero)
#define LEDC_CHANNEL_1  1  // channel of 16 channels (started from zero)
#define LEDC_TIMER_BIT 12  // use 12 bit precission for LEDC timer
#define LEDC_BASE_FREQ 50  // use 50 Hz as a LEDC base frequency

// Servo Range
// int srvMin = 103; // (103/4096)*20ms = 0.5 ms (-90c)
// int srvMax = 491; // (491/4096)*20ms = 2.4 ms (+90c)
#define srvMin 103
#define srvMax 491

void servoInit();
void setServo(uint8_t servoNo, uint16_t angle);

/********************************
* for Motor Control
*********************************/
#define MTR_A1  13  // moterA IN1
#define MTR_A2  12  // moterA IN2
#define MTR_AE  18  // moterA EN
#define MTR_B1  14  // moterB IN1
#define MTR_B2  15  // moterB IN2
#define MTR_BE  19  // moterB EN

#define LEDC_CHANNEL_2  2   // channel of 16 channels (started from zero)
#define LEDC_CHANNEL_3  3   // channel of 16 channels (started from zero)
#define LEDC_TIMER_BIT_MTR 8    // use 8 bit precission for LEDC timer
#define LEDC_BASE_FREQ_MTR 255  // use 50 Hz as a LEDC base frequency

#define MotorL 2
#define MotorR 3

void Motor_INIT();
void setMotorSpeed(uint8_t motorNo, uint8_t speed);
void motorL_forward(uint8_t speed);
void motorL_back(uint8_t speed);
void motorL_stop();
void motorR_forward(uint8_t speed);
void motorR_back(uint8_t speed);
void motorR_stop();
void motor_stop();
void motor_forward();
void motor_back();
void motor_left();
void motor_right();
void motor_turnleft();
void motor_turnright();
void motor_spinleft();
void motor_spinright();

/********************************
* for I/O Control
*********************************/
#define ANA11   34
#define ANA12   35
#define A34     34  //I, GPIO34, ADC1_CH6
#define A35     35  //I, GPIO35, ADC1_CH7

#define DIGI01  32
#define DIGI02  33
#define D32     32  //IO, GPIO32, ADC1_CH4, TOUCH9
#define D33     33  //IO, GPIO33, ADC1_CH5, TOUCH8

#define DIGI11  26
#define DIGI12  25
#define D26     26  //IO, GPIO25, DAC_1, ADC2_CH8
#define D25     25  //IO, GPIO26, DAC_2, ADC2_CH9

// I2C
#define I2CSDA  21
#define I2CSCL  22
#define D21     21  //IO, GPIO21
#define D22     22  //IO, GPIO22

// UART2
#define UARTRXD 16
#define UARTTXD 17
#define D16     16  //IO, GPIO16, U2RXD
#define D17     17  //IO, GPIO17, U2TXD

// --------------------------------------
// i2c_scanner
// --------------------------------------
void i2c_scan();

#endif
