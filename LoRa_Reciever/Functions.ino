void update_display(String LoRaData, int rssi, int snr) {
   display.clearDisplay();
   
   display.setCursor(0,0);
   display.print("  -LORA RECEIVER-");
   
   display.setCursor(0,20);
   display.print("Received packet:");
   
   display.setCursor(0,30);
   display.print(LoRaData);

   display.setCursor(0,40);
   display.print("RSSI:");
   display.setCursor(30,40);
   display.print(rssi);

   display.setCursor(60,40);
   display.print("SNR:");
   display.setCursor(90,40);
   display.print(snr);
   
   display.display();   
 
}

void display_splash() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA RECEIVER ");
  display.display();
}

void blink_led() {
  digitalWrite(LED, HIGH);
  delay(20);
  digitalWrite(LED, LOW);
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("IP:   ");
  Serial.println(WiFi.localIP());

  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
}
