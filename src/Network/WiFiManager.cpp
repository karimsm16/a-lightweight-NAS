#include "WiFiManager.h"
WiFiManager::WiFiManager()
{
    connected = false;
  this->hostName = "";
}
/******************************************************************************
 * begin()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Connects the ESP32 to a Wi-Fi network and starts the mDNS service.
 *
 * Parameters:
 * ---------------------------------------------------------------------------
 * ssid
 *      Wi-Fi network name.
 *
 * password
 *      Wi-Fi password.
 *
 * hostname
 *      Desired mDNS hostname.
 *
 * Returns:
 * ---------------------------------------------------------------------------
 * true
 *      Wi-Fi connected successfully and mDNS started.
 *
 * false
 *      Wi-Fi connection failed.
 *
 ******************************************************************************/
bool WiFiManager::begin(
    const char* networkSSID,
    const char* networkPassword,
    const char* hostname)
{ 
 Serial.println("=================================");
 Serial.println("connecting to wifi ....");
 Serial.println("=================================");

  // save the hostname for later use 

 this->hostName = hostname;

 // put the ESP32 into Station mode.
 // the ESP32 will connect to an existing WI_FI network

 WiFi.mode(WIFI_STA);

 // Start connecting 

 WiFi.begin(networkSSID,networkPassword);

 // wait until connected 

 uint32_t startTime = millis() ;

  while(WiFi.status()!=WL_CONNECTED)
 {
  Serial.print(".");
  delay(500);

  

  // TImeout after 15 seconds 
    if (millis()-startTime > 15000)
   {
     Serial.println("ERROR : connection timeout .");

     connected = false ;
     return false ;

    }


 }
 // connection successful . 
  connected = true ; 
  Serial.println ("SUCCESS : Connected to Wi_Fi");
  Serial.println();

  //Display the assigned IP address 
  Serial.print ("IP adress : ");
  Serial.println(WiFi.localIP());
  
  // Start mDNS service :
    if (!MDNS.begin (hostname))
  {
    Serial.println("ERROR:Failed to start mDNS");
    return false ;
  }
 Serial.print (" Hostname : ");
 Serial.print(this->hostName);
 Serial.println(".local");

 Serial.println();
 Serial.print("Open browser and go to :");
 Serial.print ("http://");
 Serial.print(hostName);
 Serial.println(".local");

 Serial.println("=================================");

 return true ;



} 
/******************************************************************************
 * update()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Checks the Wi-Fi connection state.
 *
 * This function should be called continuously inside loop().
 ******************************************************************************/

void WiFiManager::update()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        connected = true;
    }
    else
    {
        connected = false;
    }
}


/******************************************************************************
 * isConnected()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns true if the ESP32 is connected to Wi-Fi.
 ******************************************************************************/

bool WiFiManager::isConnected() const
{
    return connected;
}


/******************************************************************************
 * ipAddress()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns the current IP address assigned to the ESP32.
 ******************************************************************************/

String WiFiManager::ipAddress() const
{
    return WiFi.localIP().toString();
}

String WiFiManager::ssid()const
{

    return WiFi.SSID();
}


/******************************************************************************
 * macAddress()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns the ESP32 MAC address.
 ******************************************************************************/

String WiFiManager::macAddress() const
{
    return WiFi.macAddress();
}


/******************************************************************************
 * signalStrength()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns the Wi-Fi signal strength.
 *
 * Unit:
 *      dBm
 ******************************************************************************/

int32_t WiFiManager::signalStrength() const
{
    return WiFi.RSSI();
}


/******************************************************************************
 * hostname()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns the configured mDNS hostname.
 ******************************************************************************/

String WiFiManager::hostname() const
{
    return hostName;
}

String WiFiManager::gatewayIP()const
{
    return WiFi.gatewayIP().toString();
}