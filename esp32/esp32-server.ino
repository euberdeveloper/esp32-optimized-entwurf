#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
int i = 0;

class SphinxServerCallbacks: BLEServerCallbacks {
  public:
  void onConnect(BLEServer* pServer) {
    Serial.println("Server received connection");
    i++;

    if (i == 1) {
      // TODO: put out of "disabled mode"
    }
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Server received disconnection");
    i--;

    if (i == 0) {
      // TODO: put in "disabled mode"
    }
  }

  BLEServerCallbacks* getBossInstance() {
        return this;
   }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize the BLE environment
  BLEDevice::init("Sphinx BLE Server");
  Serial.println("SPHINX");
  // Create server
  BLEServer *pServer = BLEDevice::createServer();
  Serial.println("Initialized BLE Server");
  // Set server callbacks
  BLEServerCallbacks *cbs = (new SphinxServerCallbacks())->getBossInstance();
  pServer->setCallbacks(cbs);
  // Create service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  Serial.println("Initialized BLE Service");
  // Create characteristic
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  Serial.println("Initialized Characteristics");
  // Set characteristic value
  pCharacteristic->setValue("Hello world Ciao mondo Hallo Welt");
  // Start BLE service
  pService->start();
  Serial.println("Started BLE service");

  // Advertise BLE device
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Started BLE advertisement");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  pCharacteristic->setValue(std::to_string(i++));
  pCharacteristic->notify();
  Serial.println("Udated");
}
