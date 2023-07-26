#include "BLE_mananger.h"
#include "fan_control.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>


namespace _10klab {
namespace BLE {
#define LED_ON_BOARD 21
BLEServer *pServer = NULL;
BLECharacteristic *details_characteristic = NULL; // data to sen
BLECharacteristic *command_characteristic = NULL; // fan speed

#define SERVICE_UUID "5702dfea-d5a7-41d5-97e5-42cf1db4e8cd"
#define DETAILS_CHARACTERISTIC_UUID "9d5e5b57-35b4-43cc-a31f-2be12b99a874"
#define COMMAND_CHARACTERISTIC_UUID "48fdcfd7-7335-43bc-ade8-e7a0e0cae785"

String readDeviceName();
void blinkLed(int blink_time);
int StorageFanSpeed(bool update_speed, int speed);

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { Serial.println("Connected"); };

  void onDisconnect(BLEServer *pServer) {
    Serial.println("Disconnected");
    pServer->getAdvertising()->start(); // Restart the advertising
  }
};

// Define a custom class CharacteristicsCallbacks that inherits from
// BLECharacteristicCallbacks
class CharacteristicsCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    // Read the value written to the characteristic
    String fan_speed_char = pCharacteristic->getValue().c_str();
    int fan_speed = fan_speed_char.toInt();
    StorageFanSpeed(true, fan_speed);
    Serial.println("fan speed= " + String(fan_speed));
    // Call the function to control the fan speed
    _10klab::fan::controlFanSpeed(fan_speed);

    String command_value = "";
    // Send the incoming data as answer
    if (pCharacteristic == command_characteristic) {
      command_value = pCharacteristic->getValue().c_str();
      command_characteristic->setValue(
          const_cast<char *>(command_value.c_str()));
      command_characteristic->notify();
    }

    blinkLed(100);
  }
};

int StorageFanSpeed(bool update_speed, int speed){
  static int fan_speed = 0;

  if(update_speed){
    fan_speed = speed;
    return fan_speed;
  }
  else
  {
    return fan_speed;
  }
}

void blinkLed(int blink_time) {

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_ON_BOARD, HIGH);
    delay(blink_time);
    digitalWrite(LED_ON_BOARD, LOW);
    delay(blink_time);
  }
}

void initializeBLEService() {

  pinMode(LED_ON_BOARD, OUTPUT);
  digitalWrite(LED_ON_BOARD, LOW);

  blinkLed(250);

  String device_name = readDeviceName(); // Read the device name
  // Create the BLE Device
  BLEDevice::init(device_name.c_str());
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  delay(100);

  // Create a BLE Characteristic for details
  details_characteristic = pService->createCharacteristic(
      DETAILS_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                                       BLECharacteristic::PROPERTY_WRITE |
                                       BLECharacteristic::PROPERTY_NOTIFY |
                                       BLECharacteristic::PROPERTY_INDICATE);
  // Create a BLE Characteristic for commands
  command_characteristic = pService->createCharacteristic(
      COMMAND_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                                       BLECharacteristic::PROPERTY_WRITE |
                                       BLECharacteristic::PROPERTY_NOTIFY |
                                       BLECharacteristic::PROPERTY_INDICATE);

  // Start the BLE service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

  // Set initial values and callbacks for the details characteristic
  details_characteristic->setValue("");
  details_characteristic->setCallbacks(new CharacteristicsCallbacks());

  // Set initial values and callbacks for the command characteristic
  command_characteristic->setValue("0");
  command_characteristic->setCallbacks(new CharacteristicsCallbacks());

  Serial.println("Waiting for a client connection to notify...");
}

bool verifyConnectionState(){
  if (pServer->getConnectedCount() > 0){
    return true;
  }
  else{
    return false;
  }
}

void sendMessage(String message) {
  details_characteristic->setValue(message.c_str());
  details_characteristic->notify();
  blinkLed(20);
}

String readDeviceName() {
  unsigned char read_mac[6] = {0}; // Create an array to store the MAC address
  char mac_char[18] = {
      0}; // Create a character array to store the MAC address as a string
  esp_read_mac(read_mac,
               ESP_MAC_WIFI_STA); // Read the MAC address of the device

  // Format the MAC address as a string
  sprintf(mac_char, "%02X %02X %02X %02X %02X %02X", read_mac[0], read_mac[1],
          read_mac[2], read_mac[3], read_mac[4], read_mac[5]);

  String mac_address(mac_char);    // Convert the MAC address to a string
  String device_name = "Apolapp "; // Set the device name prefix
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
        

  String compacted_data =
      ""; // Initialize an empty string to store the compacted data
    
  int fan_speed = StorageFanSpeed(false, false);

  // Compact the data by concatenating the values with delimiters
  //TODO: change the cast for the correct variable type
  compacted_data = String(int(dog_sensor_temp)) + "$" + String(int(fan_sensor_temp)) +
                   "$" + String(int(battery_level)) + "$" + String(int(fan_speed));

  sendMessage(compacted_data);
}

} // namespace BLE
} // namespace _10klab