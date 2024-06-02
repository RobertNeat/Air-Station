/*
  To do next:
  - dodać do kodu nadpisywanie elementów tylko kiedy wartości się zmieniają z poprzednich (nadpisywać większy obszar ciemnymi kwadratami)
    ^żeby nie było efektu mrugani całego ekranu
  - dodać zczytywanie kilku odczytów, żeby nie skakały tak bardzo przez niedokładne odczyty
    ^na przykład 5 pomiarów pewnie by zwiększyło stałość odczytów
  - i nie muszą się tak często odświerzać, może być do 5 sekund na przykład
  - dodać napis nad wykresem łukowym z napisem "Humidity 20%" tak, żeby był czarny napis od początku wykresu
  - dodać napis nad wykresem łukowym z napisem "CO2 20%" tak, żeby był czarny napis nad początkiem wykresu
  - zrobić refactoring kodu (pogrupować w obiekty/struktury, zrobić funkcje aktualizujące częsci dashboardu)

  - dodać rotary encoder i dodać przechodzenie po menu (strony wyjaśniejące poszczególne odczyty, wyjaśnienia z tabel scrollowalne)

*/

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
#include <Fonts/Custom/Lemon_Milk_Font_10.h>
#include <Fonts/Custom/Lemon_Milk_Font_20.h>
#include <Fonts/Custom/Lemon_Milk_Font_30.h>
#include <Fonts/Custom/Lemon_Milk_Font_40.h>

TFT_eSPI tft = TFT_eSPI();
AirSensor airSensor;

String temp_text = "";
String aqi_text = "";
String tvoc_text = "";
float humidity_value = 0.0;
float co2_value = 0.0;

int humidity_arc = 0;
int co2_arc = 0;
uint16_t co2_arc_color = 0x8eff;

void setup() {
  Serial.begin(115200);
    
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  //tft.setTextSize(1);
  //tft.setTextPadding(30);
  
  Serial.println("#-----/ENS160 (Indoor Air Quality) + AHT21 (temperature, humidity)/-----#");
  if (!airSensor.begin()) {
      Serial.println("Failed to initialize sensors!");
      while (1) delay(10);
  }

}

void loop() {
  airSensor.update();

    //1. GET READOUTS
      //airSensor.getReadout();//for debugging
      temp_text = airSensor.getTemperature();
      temp_text += " °C";
      aqi_text = airSensor.getAQI();
      aqi_text += " AQI";
      tvoc_text = airSensor.getTVOC();
      tvoc_text+= " TVOC";
      humidity_value = airSensor.getHumidity();
      co2_value = airSensor.getCO2();

    //2. PRINT TEXT DATA
      tft.loadFont(lemonMilkFont20);
      tft.setTextColor(0x8c71, TFT_BLACK, true);
      tft.drawString(tvoc_text,(tft.width()-tft.textWidth(tvoc_text))/2,((tft.height()-tft.fontHeight())/2)-50);

      tft.loadFont(lemonMilkFont40);
      tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
      tft.drawString(temp_text,(tft.width()-tft.textWidth(temp_text))/2,(tft.height()-tft.fontHeight())/2);

      tft.loadFont(lemonMilkFont30);
      tft.setTextColor(0xce59, TFT_BLACK, true);
      tft.drawString(aqi_text,(tft.width()-tft.textWidth(aqi_text))/2,((tft.height()-tft.fontHeight())/2)+50);

    //3. DRAW ARCS
      //humidity (0-100 value to 45-315) <-- outer ring
      humidity_arc = ((27.0/10.0)*humidity_value)+45.0;
      tft.drawSmoothArc(tft.width()/2, tft.height()/2, 120, 110, 45, humidity_arc, 0x04bf, TFT_BLACK, true);
      
      //co2 value <-- inner ring
      co2_arc = (((co2_value-400.0)*27.0)/80)+45;
          //Serial.print("CO2:");
          //Serial.println(co2_arc);
            if(co2_value<=600){co2_arc_color = 0x8eff;} //change color for specific tresholds (excellent - target)
            else if(co2_value<=800){co2_arc_color = 0x9772;}// (good - sufficient ventillation recommended)
            else if(co2_value<=1000){co2_arc_color = 0xfea0;}// (moderate - increased ventillation recommended)
            else if (co2_value<=1200){co2_arc_color = 0xfd20;}// (poor - intensified ventillation recommended)
            else{co2_arc_color = 0xfa8a;}// (unhealthy - use only if unavoidable)
      tft.drawSmoothArc(tft.width()/2, tft.height()/2, 100, 90, 45, co2_arc, co2_arc_color, TFT_BLACK, true);//315

    delay(1000);
    tft.fillScreen(TFT_BLACK);  //tempopary fix for updating text and arcs (maybe there is a function of I will use the bigger black shape)
}

/*
  Wzór na przeskalowanie wartości X z zakresu [a,b] na zakres [c,d] jest następujący:
  Y=[{(X−a)⋅(d−c)}/(b−a)]+c
*/

/*
Temperatura - zapisana na środku
Wilgotność (od 0 do 100%) << na niebieskim półokręgu
AQI (od 1 do 5) str. 11

TVOC - wartość na środku (podlega pod AQI)
eCO2 (od 400 do 1500; 5 kategorii) << zmiany kolorów napisów według dokumentacji str. 10

Zapis na środku:
	- temperatura (biały napis),
	- AQI (kolor napisu w zależności od wartości)
	- TVOC (mniejszy szary napis)

Zapis na skalce:
	- wilgotność (jasny niebieski),
	- CO2 (skala przechodząca między kolorami w zależności od wartości)
*/

