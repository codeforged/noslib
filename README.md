Berikut adalah contoh pengiriman data 4 sensor dari nilai2 sensor yang diperoleh dari nilai random:


```C
#include <Arduino.h>
#include <noslib.h>

#define WIFI_SSID "<YoutWiFiSSID>"
#define WIFI_PASSWORD "YourWiFiPassword"
#define MQTT_SERVER "192.168.0.105"
#define MQTT_PORT 1883

char key[KEY_SIZE] = {<Your 32 byte Chacha20 Key>};
NOS nos("ESP32MultiSensorDec", 100, key, MQTT_SERVER, MQTT_PORT);

const char *sensorIds[] = {"01", "02", "03", "04"};
int sensorVals[4] = {0, 0, 0, 0}; // Initial values for sensors
const int sensorCount = 4;
uint32_t lastSent = 0;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  nos.begin();
}

void loop()
{
  nos.loop();
  if (millis() - lastSent >= 5000)
  {
    for (int i = 0; i < sensorCount; i++)
    {
      int delta = random(-10, 11);
      sensorVals[i] = constrain(sensorVals[i] + delta, 1, 100);
    }
    char data[64];
    sprintf(data, "%s=%d;%s=%d;%s=%d;%s=%d",
            sensorIds[0], sensorVals[0],
            sensorIds[1], sensorVals[1],
            sensorIds[2], sensorVals[2],
            sensorIds[3], sensorVals[3]);
    nos.sendPacket("espiot", 1000, data);
    Serial.println(data);
    lastSent = millis();
  }
}
```
