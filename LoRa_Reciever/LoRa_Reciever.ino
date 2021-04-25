/*********
  Original code from Rui Santos  https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/

  * Connect to WiFi
    - Reconnect if connection lost https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/#3
    - Dissconnect to save power? Probably not on the RX
 
  - Connect to MQTT
    ? https://github.com/marvinroger/async-mqtt-client
    ? https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
 
  * Listen for LoRa packet
    ! BUG! Stops recieves packets
    
  - Decode packet, ID and Checksum
  - Send sensor data from packet to MQTT

                        --> 2021 - SA3ARK, SA3ARQ, SM3HHI, SM3MGU <--
*********/

#include "secret.h"
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

//Libraries for WiFi
#include <WiFi.h>

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//LoRa Tansciever [TTGO32 LoRa OLED] [TTGO T-Beam]
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
#define BAND 866E6

//OLED pins [TTGO32 LoRa OLED]
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16

//OLED pins [TTGO32 T-Beam]
//#define OLED_SDA 22
//#define OLED_SCL 21
//#define OLED_RST 16

#define SCREEN_WIDTH 128 // Width in pixels
#define SCREEN_HEIGHT 64 // Height in pixels

// [TTGO32 LoRa OLED]
#define LED 2 //LED pin
#define BUTTON 0 //PRG button 


//LED pin [TTGO32 T-Beam]
//#define LED 14

/////////////////////////////////////////////////////////////////////////////////
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData;
int buttonState = 0;  // variable for reading button state

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);

  //Led Pin Output
  pinMode(LED, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(BUTTON, INPUT);
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Don't proceed, loop forever
  }

  display_splash();
  Serial.println("LoRa Receiver");

  initWiFi();
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  // LoRa.setSpreadingFactor(8);           // ranges from 6-12,default 7 see API docs
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();  
}


void loop() {
  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    Serial.print("Received packet ");
    blink_led();

    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
    }

    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");    
    Serial.print(rssi);

    //print SNR of packet
    int snr = LoRa.packetSnr();
    Serial.print(" with SNR ");    
    Serial.println(snr);

    // Dsiplay information
    update_display(LoRaData, rssi, snr);
  }

  buttonState = digitalRead(BUTTON);
  if (buttonState == LOW ) {
    Serial.println("PRG Button Pressed");
    delay(500);  // Ugly, but just for test.
  }

}
