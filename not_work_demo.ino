// README
// Add libs:
// RadioHead
//   Remember to uncomment #define RH_ENABLE_ENCRYPTION_MODULE in RadioHead.h
// Crypto
// CryptoLw

// Compile for esp32 dev module

// Right now it is printing out a bunch of zeros, if it worked correctly it would print out seemingly random hex numbers.
// Should have no complie errors
// Probably can't replicate the issue unless you have two lora radios, but hopefully you can spot the issues none the less

// Thanks!!!

#include <Arduino.h>

#include "VehicleTelemetry.h"

const uint8_t LORA_CS_PIN = 22;
const uint8_t LORA_INT_PIN = 17;
const uint8_t LORA_RST_PIN = 16;

const uint8_t TELEM_ENCRYPT_KEY[] = {0x6B, 0x58, 0x70, 0x32, 0x73, 0x35, 0x76, 0x38, 0x79, 0x2F, 0x42, 0x3F, 0x45, 0x28, 0x48, 0x2B};

VehicleTelemetry vehicleTelem(LORA_CS_PIN, LORA_INT_PIN, LORA_RST_PIN);

uint8_t serializedTelem[VehicleTelemetry::MAX_LEN];

unsigned long lastVarPrint;

void setup() {
  Serial.begin(115200);

  if (!vehicleTelem.initRadio()) {
    Serial.println("\nFailed to initialize vehicle telemetry radio");
  }
  vehicleTelem.setEncryptionKey(TELEM_ENCRYPT_KEY, sizeof(TELEM_ENCRYPT_KEY));
}

void printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}

void loop() {
  vehicleTelem.receiveTelemetry(serializedTelem, sizeof(serializedTelem));

  if (millis() > lastVarPrint + 1000) {
    lastVarPrint = millis();
    
    for(int i = 0; i < sizeof(serializedTelem); i++){
      printHex(serializedTelem[i]);
    }

    Serial.println();
    Serial.println();
  }
}
