#include "duke32.h"

#define usesoftAP

//#define useOLED
#define useLED

#ifndef usesoftAP
  #define useOTA
#endif

//#define DEBUG

/********************************
* for Wifi Connection
*********************************/
#include "WebPage.h"
char OTAHostPrefix[]= "ESP32OTA-";
char WiFiAPPrefix[] = "ESP32-";

char WiFiAPname[256];
char OTAHostname[256];

#ifdef usesoftAP
  const char passwd[] = "esp32";
#endif

WiFiServer server(80);

String header;

int pos1 = 0;
int pos2 = 0;

/********************************
* for LED Control
*********************************/
#ifdef useLED
  // https://github.com/Makuna/NeoPixelBus
  #include <NeoPixelBus.h>
  
  const uint16_t PixelCount = 64;

  // three element pixels, in different order and speeds
  //NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod> strip(PixelCount, NEO_PIN);
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> ledStrip(PixelCount, NEO_PIN);

  String ledValueR = String(0);
  String ledValueG = String(0);
  String ledValueB = String(0);

  uint8_t colorR = ledValueR.toInt() / 2;
  uint8_t colorG = ledValueG.toInt() / 2;
  uint8_t colorB = ledValueB.toInt() / 2;
#endif

/********************************
* for Servo Control
*********************************/
String servoValueL = String(90);
String servoValueR = String(90);

uint8_t servoNo;    // Set 0 or 1
uint16_t angle;     // min=0, max=180
uint16_t servoL = servoValueL.toInt();
uint16_t servoR = servoValueR.toInt();

#ifdef useOLED
  // https://github.com/ThingPulse/esp8266-oled-ssd1306
  #include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
  SSD1306  display(0x3c, I2CSDA, I2CSCL);
#endif

/********************************
* for Motor Control
*********************************/
const uint8_t motorInterval = 30;  //[ms]

/********************************
* for I2C scan
*********************************/
#ifdef DEBUG
  #define PUSH_SHORT 100  //count of remove chattering
  uint8_t count_low = 0;
  uint8_t scanPin = 0;
#endif

void setup() {
  Serial.begin(115200);

  #ifndef useOLED
    Wire.begin(I2CSDA,I2CSCL);  //Wire.begin(SDA, SCL)
  #endif

  #ifdef useOLED
    // init OLED SSD1306
    display.init();
    display.clear();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "OLED Setup done");
    display.display();
  #endif

  #ifdef useLED
    // reset all LEDs to initial color
    ledStrip.Begin();
    RgbColor ledColor(colorR, colorG, colorB);
    setLED(ledColor);
  #endif

  // reset all Servo to initial Position
  servoInit();
  setServo(0, servoL);
  setServo(1, servoR);

  // reset all Motor to initial Setting
  Motor_INIT();
  delay(50);
  setMotorSpeed(MotorL, 0);
  setMotorSpeed(MotorR, 0);

//  motor_stop();

  #ifdef DEBUG
    //for I2C scan
      pinMode(scanPin, INPUT);
  #endif

  // Wifi wnd OTA setup
  uint64_t Chipid = GetChipid();

  sprintf(WiFiAPname, "%s%04X", WiFiAPPrefix, (uint16_t)Chipid);

  #ifdef useOLED
    display.clear();
    display.drawString(0, 0, (String)WiFiAPname);
    display.drawString(0, 10, "192.168.4.1");
    #ifdef usesoftAP
      display.drawString(0, 20, "softAP mode");
    #endif
    display.display();
  #endif

  #ifdef usesoftAP
    WiFi.softAP(WiFiAPname, passwd);
    delay(100);
    IPAddress ip(192,168,4,1);
    IPAddress subnet(255,255,255,0);
    WiFi.softAPConfig(ip, ip, subnet);
    Serial.print("AP name: ");
    Serial.println(WiFiAPname);
  #else
    WiFiMgrSetup(WiFiAPname);

    #ifdef useOLED
      display.clear();
      display.drawString(0, 0, "connected(^^)");
      display.drawString(0, 10, (String)WiFi.SSID());
      IPAddress myIP = WiFi.localIP();
      display.drawString(0, 20,(String)myIP[0] + "." + (String)myIP[1] + "." + (String)myIP[2] + "." + (String)myIP[3]);
      display.display();
    #endif
  #endif

  delay(1000);

  #ifdef useOTA
    sprintf(OTAHostname, "%s%04X", OTAHostPrefix, (uint16_t)Chipid);
  
    OTASetup(OTAHostname);
  #endif

  Serial.print("IP address: ");
  #ifdef usesoftAP
    Serial.println(ip);
  #else
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp32")) {
      Serial.println("MDNS responder started");
    }
  #endif

  Serial.println("Ready");

  server.begin();

}

void loop(){
  #ifdef DEBUG
    //Scan pin check
    if(digitalRead(scanPin) == LOW){  // remove chattering
      if(count_low <= PUSH_SHORT){
        count_low ++;
      }
    }else{
      count_low = 0;
    }

    if(count_low == PUSH_SHORT){
      i2c_scan();
    }
  #endif

  #ifdef useOTA
    ArduinoOTA.handle();
  #endif

  WiFiClient client = server.available();   // Listen for incoming clients

  int lineNum;
  String htmlCmd = "";

  if (client) {                             // If a new client connects,
    #ifdef DEBUG
      Serial.println("New Client.");          // print a message out in the serial port
    #endif
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()){             // loop while the client's connected
      if (client.available()){              // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        #ifdef DEBUG
          Serial.write(c);                    // print it out the serial monitor
        #endif
        header += c;
        if (c == '\n'){                     // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0){
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the web page of WebPage.h
            lineNum = 0;
            do{
              //Update LED value
              if(HTTP_Page[lineNum].indexOf("ledValueR")>= 0){
                htmlCmd = HTTP_Page[lineNum];
                #ifdef useLED
                  htmlCmd.replace("ledValueR", ledValueR);
                #endif
                client.println(htmlCmd);
                #ifdef DEBUG
                  Serial.println("Replaced: " + htmlCmd);
                #endif
              }else if(HTTP_Page[lineNum].indexOf("ledValueG")>= 0){
                htmlCmd = HTTP_Page[lineNum];
                #ifdef useLED
                  htmlCmd.replace("ledValueG", ledValueG);
                #endif
                client.println(htmlCmd);
                #ifdef DEBUG
                  Serial.println("Replaced: " + htmlCmd);
                #endif
              }else if(HTTP_Page[lineNum].indexOf("ledValueB")>= 0){
                htmlCmd = HTTP_Page[lineNum];
                #ifdef useLED
                  htmlCmd.replace("ledValueB", ledValueB);
                #endif
                client.println(htmlCmd);
                #ifdef DEBUG
                  Serial.println("Replaced: " + htmlCmd);
                #endif
              }else if(HTTP_Page[lineNum].indexOf("servoValueL")>= 0){
                htmlCmd = HTTP_Page[lineNum];
                htmlCmd.replace("servoValueL", servoValueL);
                client.println(htmlCmd);
                #ifdef DEBUG
                  Serial.println("Replaced: " + htmlCmd);
                #endif
              }else if(HTTP_Page[lineNum].indexOf("servoValueR")>= 0){
                htmlCmd = HTTP_Page[lineNum];
                htmlCmd.replace("servoValueR", servoValueR);
                client.println(htmlCmd);
                #ifdef DEBUG
                  Serial.println("Replaced: " + htmlCmd);
                #endif
              }else{
                client.println(HTTP_Page[lineNum]);
              }
              if(HTTP_Page[lineNum].indexOf("</html>")>= 0){
                break;
              }
              lineNum++;
            }while(true);

            // Get Motor value
            if(header.indexOf("GET /?MOTOR=STOP")>=0) {
              motor_stop();
            }
            if(header.indexOf("GET /?MOTOR=FWD")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_forward();
            }
            if(header.indexOf("GET /?MOTOR=BACK")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_back();
            }
            if(header.indexOf("GET /?MOTOR=LEFT")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_left();
            }
            if(header.indexOf("GET /?MOTOR=RIGHT")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_right();
            }
            if(header.indexOf("GET /?MOTOR=TURN_L")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_turnleft();
            }
            if(header.indexOf("GET /?MOTOR=TURN_R")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_turnright();
            }
            if(header.indexOf("GET /?MOTOR=SPIN_L")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_spinleft();
            }
            if(header.indexOf("GET /?MOTOR=SPIN_R")>=0) {
              motor_stop();
              delay(motorInterval);
              motor_spinright();
            }

            // Get Servo value
            if(header.indexOf("GET /?servoSlidL=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              servoValueL = header.substring(pos1+1, pos2);
              servoL = servoValueL.toInt();
              #ifdef DEBUG
                Serial.println("Servo Left: " + (String)servoL);
              #endif
              setServo(0, servoL);
            }

            if(header.indexOf("GET /?servoSlidR=")>=0) {
              servoValueR = 90;
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              servoValueR = header.substring(pos1+1, pos2);
              servoR = servoValueR.toInt();
              #ifdef DEBUG
                Serial.println("Servo Right: " + (String)servoR);
              #endif
              setServo(1, servoR);
            }

            if(header.indexOf("GET /?servoRESET=RESET")>=0) {
              servoValueL = "90";
              servoValueR = "90";
              servoL = servoValueL.toInt();
              servoR = servoValueR.toInt();
              setServo(0, servoL);
              setServo(1, servoR);
            }

            #ifdef useLED
              // Get LED value
              if(header.indexOf("GET /?valueR=")>=0) {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                ledValueR = header.substring(pos1+1, pos2);
  
                colorR = ledValueR.toInt()/ 2;
                #ifdef DEBUG
                  Serial.println("LED R: " + (String)colorR);
                #endif
                RgbColor ledColor(colorR, colorG, colorB);
                setLED(ledColor);
              }
  
              if(header.indexOf("GET /?valueG=")>=0) {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                ledValueG = header.substring(pos1+1, pos2);
  
                colorG = ledValueG.toInt() / 2;
                #ifdef DEBUG
                  Serial.println("LED G: " + (String)colorG);
                #endif
                RgbColor ledColor(colorR, colorG, colorB);
                setLED(ledColor);
              }
  
              if(header.indexOf("GET /?valueB=")>=0) {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                ledValueB = header.substring(pos1+1, pos2);
  
                colorB = ledValueB.toInt() / 2;
                #ifdef DEBUG
                  Serial.println("LED B: " + (String)colorB);
                #endif
                RgbColor ledColor(colorR, colorG, colorB);
                setLED(ledColor);
              }

              if(header.indexOf("GET /?ledRESET=OFF")>=0) {
                ledValueR = "0";
                ledValueG = "0";
                ledValueB = "0";
                colorR = ledValueR.toInt() / 2;
                colorG = ledValueG.toInt() / 2;
                colorB = ledValueB.toInt() / 2;
                RgbColor ledColor(colorR, colorG, colorB);
                setLED(ledColor);
              }
            #endif
            client.println();
            // Break out of the while loop
            break;
          }else{
            currentLine = "";
          }
        }else if(c != '\r') {
          currentLine += c;
        }
      }
    }
    // Clear the header variable
    header = "";
    client.stop();
    #ifdef DEBUG
      Serial.println("Client disconnected.");
      Serial.println("");
    #endif
  }
}

#ifdef useLED
/********************************
* for LED Control
*********************************/
void setLED(RgbColor npcolor){
    for(uint8_t i=0; i<PixelCount; i++){
      ledStrip.SetPixelColor(i, npcolor);
    }
    ledStrip.Show();
}
#endif
