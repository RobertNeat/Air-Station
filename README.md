# Air Station

Air station project is the air quality sensor paired with display that aims to:

- monitor the air properties in real time, display it on dashboard,
- provide the explanation pages for user to learn about air quality properties (in future),
- acquire data to show daily graphs (in future),
- provide local web page to read through the readings, dashboards and all using computer or phone (in future)

## At now completed:

1. Show in dashboard current readings (dashboard need little polishing but the main objective is completed)

## Next to complete:

1. average readouts, tune the cooldown, dashboard tweaks, code refactoring

## Connection diagram (create graphic diagram):

```
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
```
