#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1            D6
#define Relay2            D5
#define Relay3            D2
#define Relay4            D1

// Add wifi access point credentiaals
#define WLAN_SSID   "Dheke"
#define WLAN_PASS   "9860251117"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883    
#define AIO_USERNAME  "sssanjaya"
#define AIO_KEY       "3b6c77cdf7ee42c38c0de08ff9c68b92"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Lamp1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lamp1"); // FeedName in my case i called it Lamp
Adafruit_MQTT_Subscribe Lamp2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lamp2");
Adafruit_MQTT_Subscribe Lamp3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lamp3");
Adafruit_MQTT_Subscribe Lamp4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lamp4");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Lamp1);
  mqtt.subscribe(&Lamp3);
  mqtt.subscribe(&Lamp2);
  mqtt.subscribe(&Lamp4);
}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Lamp1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Lamp1.lastread);
      int Lamp1_State = atoi((char *)Lamp1.lastread);
      digitalWrite(Relay1, Lamp1_State);
      
    }
    if (subscription == &Lamp2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Lamp2.lastread);
      int Lamp2_State = atoi((char *)Lamp2.lastread);
      digitalWrite(Relay2, Lamp2_State);
    }
    if (subscription == &Lamp3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Lamp3.lastread);
      int Lamp3_State = atoi((char *)Lamp3.lastread);
      digitalWrite(Relay3, Lamp3_State);
    }
    if (subscription == &Lamp4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Lamp4.lastread);
      int Lamp4_State = atoi((char *)Lamp4.lastread);
      digitalWrite(Relay4, Lamp4_State);
      
    }
  }

}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
