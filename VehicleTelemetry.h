#ifndef VEHICLETELEMETRY_H
#define VEHICLETELEMETRY_H

#include <Arduino.h>

#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>

// Remember to uncomment #define RH_ENABLE_ENCRYPTION_MODULE in RadioHead.h

// Define YIELD for RadioHead library, for some reason it is not defined on the ESP32 platform, which causes RadioHead to hang
#ifdef YIELD
#undef YIELD
#define YIELD yield()
#endif 

class VehicleTelemetry {
  public:
    constexpr static size_t MAX_LEN = 235; // Gotten from _driver.maxMessageLength() minus a few bytes because RadioHead doesn't seem to work if exactly max length

    VehicleTelemetry(const uint8_t loraCsPin, const uint8_t loraIntPin, const uint8_t loraRstPin);

    void setEncryptionKey(const uint8_t key[], const int len);

    bool initRadio();
    
    bool sendTelemetry(const uint8_t telem[], const uint8_t len);
    bool receiveTelemetry(uint8_t telem[], uint8_t len);
  private:
    constexpr static float LORA_FREQ = 915.0; // in MHz
    constexpr static uint8_t LORA_POWER = 23; // in dBm

    RH_RF95 * _lora;
    uint8_t _loraRstPin;

    Speck * _loraCypher;
    RHEncryptedDriver * _loraDriver;

    void resetRadio();
};

#endif
