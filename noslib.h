#ifndef NOS_ARDUINO_H
#define NOS_ARDUINO_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ChaChaPoly.h>

#define KEY_SIZE 32
#define NONCE_SIZE 12
#define TAG_SIZE 16

class NOS {
public:
    NOS(const char* id, const uint16_t port, const char* key, const char* mqttServer, int mqttPort);
    bool begin();
    void loop();
    void sendPacket(const char* dstAddress, int dstPort, const char* payload);
    void onMessage(void (*callback)(const char* srcAddress, int srcPort, const char* payload));

private:
    const char* key;
    uint8_t tag[16];

    static NOS* instance;
    WiFiClient espClient;
    PubSubClient mqttClient;
    String id;
    uint16_t port;

    const char* mqttServer;
    int mqttPort;
    void (*messageCallback)(const char*, int, const char*);
    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    void encryptData(const uint8_t *input, uint8_t *output, const uint8_t *nonce, size_t length);
    bool decryptData(const uint8_t *input, uint8_t *output, const uint8_t *nonce, uint8_t *tagBytes, size_t length);

    // bool connectWiFi(const char* ssid, const char* password);
    void connectMQTT();
};

#endif
