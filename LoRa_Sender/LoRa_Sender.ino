/*********
  Original code from Rui Santos  https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/

  - Get sensor data
  - Create packet + Unique ID + Checkum
  * Send packet over LoRa

  ? Flag to optionaly disable Display
  ? How to save power?

                        --> 2021 - SA3ARK, SA3ARQ, SM3HHI, SM3MGU <--
*********/

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

//LED pin [TTGO32 LoRa OLED]
#define LED 2

//LED pin [TTGO32 T-Beam]
//#define LED 14

/////////////////////////////////////////////////////////////////////////////////
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

int counter = 0; //packet counter

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  //Led Pin Output
  pinMode(LED, OUTPUT);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display_splash();
  Serial.println("LoRa Sender");

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
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  digitalWrite(LED, HIGH); // Light LED when TX
  LoRa.beginPacket();
  LoRa.print("Number: ");
  LoRa.print(counter);
  LoRa.endPacket();
  digitalWrite(LED, LOW);

  update_display(counter);
  
  counter++;
  
  delay(10000);
}
