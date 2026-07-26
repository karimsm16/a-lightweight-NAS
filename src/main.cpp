#include "storage/SDManager.h"

constexpr uint8_t SD_CS_PIN = 5;   // Change to your actual CS pin

SDManager sd;

void setup()
{
  Serial.begin(115200);
  SPI.begin();  // Initialize SPI bus
    sd.begin(SD_CS_PIN);
    Serial.println("SD Card Information:");
    sd.printCardInfo();
}

void loop()
{
}