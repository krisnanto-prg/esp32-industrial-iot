/**
 * @file main.cpp
 * @brief Smart Industrial Environment Monitor - Sensor Reading Feature
 * @version 1.0.0
 * @date 2026-09-09
 * @author krisnanto-prg
 * 
 * @copyright Copyright (c) 2026
 */

 #include<Arduino.h>
 #include<DHT.h>

 //---configuration constants---
 #define DHTPIN 4 /**<GPIO pin connected to the DHT11 data pin */
 #define DHTTYPE DHT11

//---Global objects---
DHT dht(DHTPIN, DHTTYPE);

//---Timing variables for Non-blocking Execution ---
unsigned long previousMillis = 0;
const long interval = 2000; /**read sensor data every 2000ms */

/**
 * @brief System Initialization
 */
 void setup(){
  Serial.begin(115200);
  Serial.println(F("======================================="));
  Serial.println(F("Industrial IoT Monitor: Initializing..."));
  Serial.println(F("======================================="));

  dht.begin();

  Serial.println(F("DHT11 Sensor Initialozation: SUCCESS"));
 }

/**
 * @brief Main System Loop
 */
 void loop(){
  unsigned long currentMillis = millis();

  //non-blocking timer implementation (standard industry practice)
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;

    //reading temperature and humidity
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    //error handling: check if any reads failed and exit early(fail-safe)
    if(isnan(humidity) || isnan(temperature)){
      Serial.println(F("[ERROR] Failed to read from DHT sensor! check hardware wiring!!"));

      return;
    }

    //outputting clean and structured data log
    Serial.print(F("[DATA] Temperature: "));
    Serial.print(temperature);
    Serial.print(F(" ℃ | Humidity: "));
    Serial.print(humidity);
    Serial.println(F(" %"));
  } 
}