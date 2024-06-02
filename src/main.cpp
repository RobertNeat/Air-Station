/***************************************************************************
  ENS160 + AHT21 sensor (standard reading mode)
  Connections from sensors to esp_32:
  VIN - to 3V3 esp_32 pin 
  3V3 - DON't CONNECT (weak power)
  GND - to GND esp_32 pin
  SCL - to GPIO_22 esp_32 pin
  SDA - to GPIO_21 esp_32 pin
****************************************************************************
  Connections (round display):
  GC9A01:            ESP32:
  RST (reset) ------- GPIO_27
  CS (chip select) -- GPIO_26
  DC (data/command) - GPIO_25
  SDA (data in) ----- GPIO_33
  SCL (clock) ------- GPIO_32
*****************************************************************************/

#include <Arduino.h>
#include <AirSensor.h>
#include <TFT_eSPI.h>
#include <Fonts/Custom/Lemon_Milk_Font_20.h>

TFT_eSPI tft = TFT_eSPI();
AirSensor airSensor;

String displayed_text = "";

void setup() {
  Serial.begin(115200);
    
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK, true),
  tft.setTextSize(1);
  tft.loadFont(lemonMilkFont20);
  tft.setTextPadding(30);
  
  Serial.println("#-----/ENS160 (Indoor Air Quality) + AHT21 (temperature, humidity)/-----#");
  if (!airSensor.begin()) {
      Serial.println("Failed to initialize sensors!");
      while (1) delay(10);
  }
}

void loop() {
    airSensor.update();

    /*
    Serial.println("### Sensor Readings ###");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" % rH");

    Serial.print("Air Quality Index (AQI): ");
    Serial.println(aqi);

    Serial.print("Total Volatile Organic Compounds (TVOC): ");
    Serial.print(tvoc);
    Serial.println(" ppb");

    Serial.print("Equivalent CO2 (eCO2): ");
    Serial.print(co2);
    Serial.println(" ppm");
    */

    airSensor.getReadout();
    displayed_text = airSensor.getTemperature();
    displayed_text += " °C";
    tft.drawString(displayed_text,(tft.width()-tft.textWidth(displayed_text))/2,(tft.height()-tft.fontHeight())/2);

    delay(1000);
}
