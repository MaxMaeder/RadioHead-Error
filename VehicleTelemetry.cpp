#include "VehicleTelemetry.h"

VehicleTelemetry::VehicleTelemetry(const uint8_t loraCsPin, const uint8_t loraIntPin, const uint8_t loraRstPin) {
  _lora = new RH_RF95(loraCsPin, loraIntPin);
  _loraRstPin = loraRstPin;

  _loraCypher = new Speck();
  _loraDriver = new RHEncryptedDriver(*_lora, *_loraCypher);
}

void VehicleTelemetry::setEncryptionKey(const uint8_t key[], const int len) {
  _loraCypher->setKey(key, len);
}

bool VehicleTelemetry::initRadio() {
  pinMode(_loraRstPin, OUTPUT);
  digitalWrite(_loraRstPin, HIGH);

  resetRadio();
  
  if (!_lora->init() || !_lora->setFrequency(LORA_FREQ)) {
    return false;
  }

  _lora->setTxPower(LORA_POWER, false);

  return true;
}

bool VehicleTelemetry::receiveTelemetry(uint8_t telem[], uint8_t len) {
  if (!_loraDriver->recv(telem, &len)) {
    return false;
  } else {
    Serial.println("Recieved telem!");
  }

  return true;
}

void VehicleTelemetry::resetRadio() {
  digitalWrite(_loraRstPin, LOW);
  delay(10);
  digitalWrite(_loraRstPin, HIGH);
  delay(10);
}
