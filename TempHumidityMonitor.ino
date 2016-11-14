#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#define WRITE_DELAY_FOR_THINGSPEAK  15500   // 15 seconds
#define MAIN_LOOP_DELAY             3600000 // 1 hour
#define DHTPIN                      5       // Pin which is connected to the DHT sensor.
#define DHTTYPE                     DHT22   // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

const char *ssid = "";
const char *password = "";

unsigned long myChannelNumber = 11111;
const char * myWriteAPIKey = "11111";

WiFiClient client;

void setup(void)
{
  setupHardware();
  connectToWifi();
  ThingSpeak.begin(client);
  dht.begin();
}

void loop(void) 
{
  // temperature in degrees celcius
  float t;
  // humidity in % 
  float h;
  
  // Get temperature event and print its value.
  sensors_event_t event;  
  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) 
  {
    Serial.println("Error reading temperature!");
  }
  else
  {
    t = event.temperature;
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  }
  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
  {
    Serial.println("Error reading humidity!");
  }
  else 
  {
    h = event.relative_humidity;
    delay(WRITE_DELAY_FOR_THINGSPEAK);
    ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  }

  Serial.print(t);
  Serial.print(" (C)\t");
  Serial.print(h);
  Serial.print(" (%)");
  Serial.println("");
  delay(MAIN_LOOP_DELAY);
}

void setupHardware()
{
  Serial.begin(115200);
}

void connectToWifi()
{
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
