#include <M5StickCPlus.h>
#include "M5_ENV.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic;

// Define your Service and Characteristic UUIDs
#define SERVICE_UUID        "e9c03cba-8d77-11ee-b9d1-0242ac120002"
#define CHARACTERISTIC_UUID "e9c03fee-8d77-11ee-b9d1-0242ac120002"

SHT3X sht30;
QMP6988 qmp6988;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

void setup() {
    M5.begin();             // Init M5StickCPlus. 
    M5.Lcd.setRotation(3);  // Rotate the screen. 
    Wire.begin(0, 26);

    qmp6988.init();
    sht30.init();
    M5.lcd.println(F("ENVIII Hat(SHT30 and QMP6988) test"));

    // Initialize BLE
    BLEDevice::init("ENVIII_BLE");
    pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
}

void loop() {
    pressure = qmp6988.calcPressure();
    if (sht30.get() == 0) {
        tmp = sht30.cTemp;
        hum = sht30.humidity;
    } else {
        tmp = 0, hum = 0;
    }

    // Update BLE characteristic with temperature and humidity data
    String data = String(tmp) + "," + String(hum);
    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();

    // Display data on the local screen
    M5.lcd.fillRect(0, 20, 100, 60, BLACK);
    M5.lcd.setCursor(0, 20);
    M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n",
                  tmp, hum, pressure);
    
    delay(2000);
}
