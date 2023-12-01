#include <M5StickCPlus.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <BLEUUID.h>

// Define the Service and Characteristic UUIDs
#define SERVICE_UUID        "e9c03cba-8d77-11ee-b9d1-0242ac120002"
#define CHARACTERISTIC_UUID "e9c03fee-8d77-11ee-b9d1-0242ac120002"

BLEClient* pClient = NULL;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.println(F("BLE Receiver"));

    // Initialize BLE client
    BLEDevice::init("Receiver");
}

void connectToServer() {
    // Create BLE address from the MAC address of the server
    BLEAddress serverAddress("E8:9F:6D:09:19:C4"); // Replace with the address of your sending M5StickCPlus

    // Create BLE client
    pClient = BLEDevice::createClient();
    pClient->connect(serverAddress);

    // Obtain the service and characteristic
    BLERemoteService* pRemoteService = pClient->getService(BLEUUID(SERVICE_UUID));
    BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(BLEUUID(CHARACTERISTIC_UUID));

    // Read the initial value
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("Received Value: ");
    Serial.println(String(value.c_str()));
}

void loop() {
    if (!pClient) {
        connectToServer();
    } else {
        
        delay(1000);
    }
}
