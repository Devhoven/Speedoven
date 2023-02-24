#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <stdlib.h>
#include <mutex>

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

    static std::mutex DataMutex;
    static uint8_t Direction;
    static uint32_t Distance;
    // The ble messages are maximum 22 bytes long and the first 9 bytes is reserved
    // for the other data (ID, Direction, Distance) 
    // Thus 13 bytes are left for the street name
    static char* StreetName = new char[13];

    void Init()
    {
        CurrentState = SCANNING;

        StreetName[0] = '\0';

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
    }

    bool EstablishConnection()
    {
        Client = BLEDevice::createClient();
        if (!Client->connect(Device))
        {
            Serial.println("No connection :(");
            return false;
        }

        RemoteService = Client->getService(PrimaryServiceUUID);
        if (RemoteService == nullptr)
        {
            Serial.println("No remote service :(");
            return false;
        }

        RemoteCharacteristic = RemoteService->getCharacteristic(CharacteristicUUID);
        if (RemoteCharacteristic == nullptr)
        {
            Serial.println("No remote char :(");
            return false;
        }

        if (!RemoteCharacteristic->canNotify())
        {
            Serial.println("No remote notify :(");
            return false;
        }

        // If everything has gone right, a callback can be set up which gets called when a message from comoot is received
        RemoteCharacteristic->registerForNotify(MessageCallback);
        
        Serial.println("--KOMOOT-- Connected to Komoot-device");
        
        CurrentState = CONNECTED;
        return true;
    }

    void CheckConnection() 
    {
        if (!Client->isConnected())
        {
            Serial.println("--KOMOOT-- Lost connection");
            CurrentState = SCANNING;
        }
    }

    void Update()
    {
        switch (CurrentState)
        {
            case SCANNING:
                StartScan();
                break;
            case WAITING_FOR_CONNECTION:
                if (!EstablishConnection())
                    CurrentState = SCANNING;
                break;
            case CONNECTED:
                CheckConnection();
                break;
        }
    }

    void MessageCallback(BLERemoteCharacteristic* remoteCharacteristics, uint8_t* data, size_t length, bool isNotification)
    {
        DataMutex.lock();
        memcpy(&Direction, data + 4, 1);
        memcpy(&Distance,  data + 5, 4);
        strcpy(StreetName, (char*)(data + 9));
        
        Serial.print(" Dir: ");
        Serial.print(Direction);
        Serial.print(" Distance: ");
        Serial.print(Distance);
        Serial.print(" Street name: ");
        Serial.println(StreetName);
        DataMutex.unlock();
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