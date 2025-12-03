#include <Arduino.h>
#include "LGFX_SPRESENSE_ILI9341.hpp"

LGFX lcd;

void setup() {
  Serial.begin(115200);
  while (!Serial) {;}

  Serial.println("=== BOS Camera Display Test ===");

  Serial.println("lcd.init()...");
  lcd.init();
  Serial.println("lcd.init() done");

  lcd.setRotation(1);  // Landscape mode
  Serial.println("fillScreen(BLACK)...");
  lcd.fillScreen(TFT_BLACK);
  Serial.println("fillScreen done");

  lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  lcd.setTextSize(2);
  lcd.setCursor(10, 10);
  lcd.print("BOS Camera");
  
  lcd.setCursor(10, 40);
  lcd.setTextSize(1);
  lcd.print("Display OK");
  
  Serial.println("Display test complete");
}

void loop() {
  // Do nothing
  delay(1000);
}
