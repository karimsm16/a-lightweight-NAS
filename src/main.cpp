#include <Arduino.h>
#include <SPI.h>

#include "Storage/SDManager.h"
#include "Network/WiFiManager.h"
#include "Services/FileService.h"
#include "Web/HttpServer.h"

// ============================
// Configuration
// ============================

constexpr uint8_t SD_CS_PIN = 5;

const char* WIFI_SSID     = "Nova";
const char* WIFI_PASSWORD = "sj111111j";
const char* HOSTNAME      = "esp32nas";

// ============================
// Project Modules
// ============================

SDManager sd;
WiFiManager wifi;
FileService files(sd);
HttpServer http(sd, files, wifi);

void setup()
{
    Serial.begin(115200);

    SPI.begin();

    // Initialize SD card
    if (!sd.begin(SD_CS_PIN))
    {
        Serial.println("SD initialization failed.");
    }

    sd.printCardInfo();

    // Connect to Wi-Fi
    if (!wifi.begin(WIFI_SSID, WIFI_PASSWORD, HOSTNAME))
    {
        Serial.println("WiFi initialization failed.");
    }
    // Start the HTTP server
     if (!http.begin())
     {
       Serial.println("HTTP server failed to start.");
      } 
   
}

void loop()
{
    wifi.update();
    http.handleClient();
}