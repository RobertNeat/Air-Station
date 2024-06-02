//custom fonts: https://youtu.be/4zYaIJxEYaM
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Fonts/Custom/Lemon_Milk_Font_20.h>

TFT_eSPI tft = TFT_eSPI();

String displayed_text = "Lemon Milk";

void setup() {    
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK, true),
  tft.setTextSize(1);
  tft.loadFont(lemonMilkFont20);
}

void loop() {
  tft.drawString(displayed_text,(tft.width()-tft.textWidth(displayed_text))/2,(tft.height()-tft.fontHeight())/2);
  delay(100);
}
