#include <Arduino.h>
#include "LGFX_SPRESENSE_ILI9341.hpp"

LGFX lcd;

void setup() {
  Serial.begin(115200);
  while (!Serial) {;}

  Serial.println("ILI9341 Test Start (SPI4)");

  Serial.println("lcd.init()...");
  lcd.init();                        // ← ここで落ちたらハードフォルト確定ポイント
  Serial.println("lcd.init() done");

  lcd.setRotation(0);
  Serial.println("fillScreen(BLACK)...");
  lcd.fillScreen(TFT_BLACK);
  Serial.println("fillScreen done");

  lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  lcd.setTextSize(2);
  lcd.setCursor(10, 10);
  lcd.print("Hello Spresense");
  Serial.println("text draw done");
}

void loop() {
  // 何もしない
}
