#include <WiFi.h>

#include <PubSubClient.h>
#include "DHT.h"

float  TEMP;
String a = "";
float  HUMIDITY;
WiFiClient espClient;
PubSubClient client(espClient);

void reconnectmqttserver() {
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
String clientId = "ESP32Client-";
 clientId += String(random(0xffff), HEX);
if (client.connect(clientId.c_str())) {
Serial.println("connected");
  client.subscribe("Jacket");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
delay(5000);
}
}
}

DHT dht4(4,DHT11);

char msgmqtt[50];
void callback(char* topic, byte* payload, unsigned int length) {
  String MQTT_DATA = "";
  for (int i=0;i<length;i++) {
   MQTT_DATA += (char)payload[i];}
  if (MQTT_DATA == "ON") {
    if (a == "HOT") {
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      digitalWrite(14,LOW);
      digitalWrite(15,LOW);

    }
    if (a == "COOL") {
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
      digitalWrite(14,HIGH);
      digitalWrite(15,LOW);

    }

  }
  if (MQTT_DATA == "OFF") {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);

  }

}

void setup()
{
TEMP = 0;
HUMIDITY = 0;
Serial.begin(9600);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("Global","99999999");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  client.setServer("192.168.43.225", 1883);
  client.setCallback(callback);

pinMode(12, OUTPUT);
pinMode(13, OUTPUT);
pinMode(14, OUTPUT);
pinMode(15, OUTPUT);
}


void loop()
{

    if (!client.connected()) {
    reconnectmqttserver();
    }
    client.loop();
    TEMP = (dht4.readTemperature( ));
    snprintf (msgmqtt, 50, "%d ",TEMP);
    client.publish("TMP", msgmqtt);
    delay(10000);

}
