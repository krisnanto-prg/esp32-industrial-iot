/**
 * @file main.cpp
 * @brief smart industrial environment monitor - wifi & MQTT connectivity feature
 * @version 1.1.0
 * @date 2026-09-09
 * @author krisnanto-prg
 */

 #include <Arduino.h>
 #include <WiFi.h>
 #include <PubSubClient.h>
 #include <DHT.h>

 //===configuration constants===
 #define DHTPIN 4
 #define DHTTYPE DHT11

 //===network credentials===
 const char* ssid = "HR01a-60180F"; /**replace with your wifi ssid */
 const char* password = "2b0cdc1dd2"; /**replace with your wifi paaword */

 //===MQTT broker configuration(using public broker for testing)===
 const char* mqtt_server = "test.mosquitto.org"; /**broker change from "://hivemq.com"; */
 const int mqtt_port = 1883;

 //previous code
 //const char* mqtt_topic_publish = "industrial/factory/sensor1";

 const char* mqtt_topic_publish = "industrial/factory/krisnanto/sensor1";

 //===global objects===
 DHT dht(DHTPIN, DHTTYPE);
 WiFiClient espClient;
 PubSubClient client(espClient);

 //===timing variables for non-blicking execution===
 unsigned long previousMillisSensor = 0;
 unsigned long previousMillisMqtt = 0;
 const long sensorInterval = 2000; /**< read sensor every 2 second */
 const long mqttReconnectInterval = 5000; /**try reconnecting MQTT every 5 second if disconnected */

/**
 * @brief setup wifi connection
 */
 void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print(F("[WIFI] Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  //note: in real production, this should also be non-blocking,
  //but for initial boot up setup, a blocking loop is acceptable.
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(F("."));
  }

 Serial.println();
 Serial.println(F("[WIFI] WiFi connected SUCCESS"));
 Serial.print(F("[WIFI] IP address: "));
 Serial.println(WiFi.localIP());
}

/**
 *@brief non-blocking MQTT reconnection logic 
 */
 void reconnect_mqtt(){
    unsigned long currentMillis = millis();

    //only attempt connection if WiFi is connected and  MQTT is down
    if (WiFi.status() == WL_CONNECTED && !client.connected()){
      if(currentMillis - previousMillisMqtt >= mqttReconnectInterval ){
        previousMillisMqtt = currentMillis;

        Serial.print(F("[MQTT] Attempting connection..."));
        //create a unique client ID based on eSP32 Mac address
        String clientId = "ESP32CLient-" + String(random(0, 1000));

        if(client.connect(clientId.c_str())){
          Serial.println(F(" CONNECTED"));
        }else{
          Serial.print(F(" FAILED, rc="));
          Serial.print(client.state());
          Serial.println(F(" | will try again in 5 second"));
        }
      }
    }
 }

 void setup(){
  Serial.begin(115200);
  Serial.println(F("\n====================================="));
  Serial.println(F("Industrial IoT monitor: Network setup"));
  Serial.println(F("======================================="));

  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
 }

 void loop(){
  //keep the MQTT client internal state alive and process incoming messages
  if (!client.connected()){
    reconnect_mqtt();
  }else{
    client.loop();
  }
  
  unsigned long currentMillis = millis();

  //non-blocking timer for sensor reading & data publishing
  if(currentMillis - previousMillisSensor >= sensorInterval){
    previousMillisSensor = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if(isnan(humidity) || isnan(temperature)){
      Serial.println(F("[ERROR] failed to read from DHT sensor!"));
      return;
    }

    //print to local serial monitor
    Serial.print(F("[DATA] Temp: "));
    Serial.print(temperature);
    Serial.print(F(" ℃ | Hum: "));
    Serial.print(humidity);
    Serial.println(F(" %")); 

    //if connected to MQTT, publish the data in JSON format (standard industry)
    if(client.connected()){
      String payload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
      
      if(client.publish(mqtt_topic_publish, payload.c_str())) {
        Serial.println(F("[MQTT] data published successfully."));
      }else{
        Serial.println(F("[MQTT] failed to publish data."));
      }
    }
  }
 }