#include "BLE_mananger.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include "fan_control.h"

namespace _10klab {
namespace BLE {
BLEServer *pServer = NULL;
BLECharacteristic *details_characteristic = NULL; // data to sen
BLECharacteristic *command_characteristic = NULL; // fan speed

#define SERVICE_UUID "5702dfea-d5a7-41d5-97e5-42cf1db4e8cd"
#define DETAILS_CHARACTERISTIC_UUID "9d5e5b57-35b4-43cc-a31f-2be12b99a874"
#define COMMAND_CHARACTERISTIC_UUID "48fdcfd7-7335-43bc-ade8-e7a0e0cae785"

String readDeviceName();

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { Serial.println("Connected"); };

  void onDisconnect(BLEServer *pServer) {
    Serial.println("Disconnected");
    pServer->getAdvertising()->start();
  }
};

class CharacteristicsCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {

    String fan_speed_char = pCharacteristic->getValue().c_str();
    int fan_speed = fan_speed_char.toInt();
    Serial.println("fan speed= " + String(fan_speed));

    _10klab::fan::controlFanSpeed(fan_speed);


    String command_value = "";
    if (pCharacteristic == command_characteristic) {
      command_value = pCharacteristic->getValue().c_str();
      command_characteristic->setValue(
          const_cast<char *>(command_value.c_str()));
      command_characteristic->notify();
    }
  }
};

void initializeBLEService() {
  String device_name = readDeviceName();
  // Create the BLE Device
  BLEDevice::init(device_name.c_str());
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  delay(100);

  // Create a BLE Characteristic
  details_characteristic = pService->createCharacteristic(
      DETAILS_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                                       BLECharacteristic::PROPERTY_WRITE |
                                       BLECharacteristic::PROPERTY_NOTIFY |
                                       BLECharacteristic::PROPERTY_INDICATE);

  command_characteristic = pService->createCharacteristic(
      COMMAND_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                                       BLECharacteristic::PROPERTY_WRITE |
                                       BLECharacteristic::PROPERTY_NOTIFY |
                                       BLECharacteristic::PROPERTY_INDICATE);

  // Start the BLE service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

  details_characteristic->setValue("Message one");
  details_characteristic->setCallbacks(new CharacteristicsCallbacks());

  command_characteristic->setValue("0");
  command_characteristic->setCallbacks(new CharacteristicsCallbacks());

  Serial.println("Waiting for a client connection to notify...");
}

void sendMessage(String message) {
  details_characteristic->setValue(message.c_str());
  details_characteristic->notify();
}

String readDeviceName() {
  unsigned char read_mac[6] = {0};
  char mac_char[18] = {0};
  esp_read_mac(read_mac, ESP_MAC_WIFI_STA);

  sprintf(mac_char, "%02X %02X %02X %02X %02X %02X", read_mac[0], read_mac[1],
          read_mac[2], read_mac[3], read_mac[4], read_mac[5]);

  String mac_address(mac_char);
  String device_name = "Apolapp ";
  String complete_name = device_name + mac_address;

  Serial.println(complete_name);
  return complete_name;
}

byte calculateChecksum(String data) {
  byte checksum = 0;

  for (size_t i = 0; i < data.length(); i++) {
    checksum ^= data.charAt(i);
  }

  return checksum;
}

void compactAndSendData(float dog_sensor_temp, float fan_sensor_temp,
                   float battery_level) {
  String compacted_data = "";
  compacted_data = String(dog_sensor_temp) + "$" + String(fan_sensor_temp) +
                   "$" + String(battery_level) + "#";

  sendMessage(compacted_data);
}

} // namespace BLE
} // namespace _10klab