#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID "abcd1234-5678-90ab-cdef-1234567890ab"

const int ledPin = 2;
BLECharacteristic *pCharacteristic;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    Serial.print("Received: ");
    Serial.println(value.c_str());

    if (value.find("on") != std::string::npos) {
      Serial.println("LED ON");
      digitalWrite(ledPin, HIGH);
    } else if (value.find("off") != std::string::npos) {
      Serial.println("LED OFF");
      digitalWrite(ledPin, LOW);
    }
  }   
};


class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("BLE Client Connected");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("BLE Client Disconnected");
  }
};


void setup() {
  delay(10000); // Allow time for the serial monitor to open
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  BLEDevice::init("XIAO-LED");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->setCallbacks(new ServerCallbacks());
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  Serial.println("BLE LED Ready");
}

void loop() {
}
