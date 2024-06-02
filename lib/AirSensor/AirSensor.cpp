#include "AirSensor.h"

AirSensor::AirSensor() : ens160(ENS160_I2CADDR_1), temperature(0), humidity(0), aqi(0), tvoc(0), co2(0) {}

bool AirSensor::begin() {
    if (!aht.begin()) {
        Serial.println("Could not find AHT? Check wiring");
        return false;
    }

    if (!ens160.begin()) {
        Serial.println("Could not find ENS160? Check wiring");
        return false;
    }

    if (ens160.available()) {
        Serial.print("\tRev: "); Serial.print(ens160.getMajorRev());
        Serial.print("."); Serial.print(ens160.getMinorRev());
        Serial.print("."); Serial.println(ens160.getBuild());

        Serial.print("\tStandard mode ");
        Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
    } else {
        Serial.print("Connect following (not connected):");
        Serial.print("GND - to ESP32 GND");
        Serial.print("VIN - to ESP32 3V3");
        Serial.print("SCL - to ESP32 GPIO 22");
        Serial.print("SDA - to ESP32 GPIO 21");
        return false;
    }

    return true;
}

void AirSensor::update() {
    sensors_event_t humidity_event, temp_event;
    aht.getEvent(&humidity_event, &temp_event);
    temperature = temp_event.temperature;
    humidity = humidity_event.relative_humidity;

    if (ens160.available()) {
        ens160.set_envdata(temperature, humidity);
        ens160.measure(true);
        ens160.measureRaw(true);

        aqi = ens160.getAQI();
        tvoc = ens160.getTVOC();
        co2 = ens160.geteCO2();
    }
}

float AirSensor::getTemperature() const {
    return temperature;
}

float AirSensor::getHumidity() const {
    return humidity;
}

int AirSensor::getAQI() const {
    return aqi;
}

int AirSensor::getTVOC() const {
    return tvoc;
}

int AirSensor::getCO2() const {
    return co2;
}

void AirSensor::getReadout(){
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" % rH");
    
    Serial.print("AQI: ");
    Serial.println(aqi);
    
    Serial.print("TVOC: ");
    Serial.print(tvoc);
    Serial.println(" ppb");
    
    Serial.print("eCO2: ");
    Serial.print(co2);
    Serial.println(" ppm");
}

/*
Temperature: 28 degrees C
Humidity: 55% rH
AQI: 1
TVOC: 27ppb
eCO2: 409ppm

*/