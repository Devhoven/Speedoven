#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <stdlib.h>

namespace KomootBLE
{
    // Name of the bluetooth device
    #define DEVICE_NAME "Speedoven"

    const BLEUUID PrimaryServiceUUID("71C1E128-D92F-4FA8-A2B2-0F171DB3436C");
    const BLEUUID CharacteristicUUID("503DD605-9BCB-4F6E-B235-270A57483026");

    class KomootAdvertisedDeviceCallbacksHandler : public BLEAdvertisedDeviceCallbacks
    {
        // The onResult-method is called when another BLE-device is picked up by the esp
        void onResult(BLEAdvertisedDevice advertisedDevice) override;
    };

    void MessageCallback(BLERemoteCharacteristic* remoteCharacteristics, uint8_t* data, size_t length, bool isNotification);

    struct KomootData 
    {
        uint32_t Identifier;
        uint8_t Direction;
        uint32_t Distance;
        char* StreetName;
    };

    enum ConnectionState
    {
        SCANNING,
        WAITING_FOR_CONNECTION,
        CONNECTED
    };

    BLEScan *Scan;
    BLEAdvertisedDevice *Device;
    BLEClient *Client;
    BLERemoteService *RemoteService;
    BLERemoteCharacteristic *RemoteCharacteristic;

    ConnectionState CurrentState;

    KomootData* CurrentData;

    void Init()
    {
        CurrentState = SCANNING;

        BLEDevice::init(DEVICE_NAME);
        BLEDevice::setMTU(127);
        BLEDevice::startAdvertising();

        Scan = BLEDevice::getScan();
        Scan->setAdvertisedDeviceCallbacks(new KomootAdvertisedDeviceCallbacksHandler());
        // Active scan uses more power, but gets results faster
        Scan->setActiveScan(true);
        Scan->setInterval(1000);
        Scan->setWindow(999);
    }

    void StartScan()
    {
        Scan->start(5, false);
        Scan->stop();
    }

    void EstablishConnection()
    {
        Client = BLEDevice::createClient();
        if (!Client->connect(Device))
            return;

        RemoteService = Client->getService(PrimaryServiceUUID);
        if (RemoteService == nullptr)
            return;

        RemoteCharacteristic = RemoteService->getCharacteristic(CharacteristicUUID);
        if (RemoteCharacteristic == nullptr)
            return;

        if (!RemoteCharacteristic->canNotify())
            return;

        // If everything has gone right, a callback can be set up which gets called when a message from comoot is received
        RemoteCharacteristic->registerForNotify(MessageCallback);
        
        CurrentState = CONNECTED;
    }

    void Update()
    {
        switch (CurrentState)
        {
            case SCANNING:
                StartScan();
                break;
            case WAITING_FOR_CONNECTION:
                EstablishConnection();
                break;
        }
    }

    void MessageCallback(BLERemoteCharacteristic* remoteCharacteristics, uint8_t* data, size_t length, bool isNotification)
    {
        CurrentData = new KomootData();
        memcpy(&CurrentData->Identifier, data,     4);
        memcpy(&CurrentData->Direction,  data + 4, 1);
        memcpy(&CurrentData->Distance,   data + 5, 4);
        CurrentData->StreetName = (char*)(data + 9);
        
        Serial.print("ID: ");
        Serial.print(CurrentData->Identifier);
        Serial.print(" Dir: ");
        Serial.print(CurrentData->Direction);
        Serial.print(" Distance: ");
        Serial.print(CurrentData->Distance);
        Serial.print(" Street name:  ");
        Serial.println(CurrentData->StreetName);
    }

    void KomootAdvertisedDeviceCallbacksHandler::onResult(BLEAdvertisedDevice advertisedDevice) 
    {
        // Checking if the advertisedDevice has the right service
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(PrimaryServiceUUID))
        {
            Device = new BLEAdvertisedDevice(advertisedDevice);
            CurrentState = WAITING_FOR_CONNECTION;
        }
    }
}