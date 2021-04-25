void update_display(int counter) {
   display.clearDisplay();
   
   display.setCursor(0,0);
   display.print("   -LORA SENDER-");
   
   display.setCursor(0,20);
   display.print("Sent packet:");
   
   display.setCursor(80,20);
   display.print(counter);

   display.display();   
 
}

void display_splash() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("  -LORA SENDER-");
  display.display();
}

void blink_led() {
  digitalWrite(LED, HIGH);
  delay(20);
  digitalWrite(LED, LOW);
}
