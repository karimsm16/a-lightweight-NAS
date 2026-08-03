
/******************************************************************************
 * File        : WiFiManager.h
 * Project     : ESP32 NAS
 * Author      : Abde nacer
 *
 * Description:
 * ---------------------------------------------------------------------------
 * WiFiManager is responsible for all Wi-Fi related operations.
 *
 * Responsibilities:
 *  - Connect to a Wi-Fi network
 *  - Monitor the connection state
 *  - Automatically reconnect if needed
 *  - Start the mDNS service
 *  - Provide network information
 *
 * Note : This class DOES Not:
 *  - Handle HTTP requests
 *  - Manage SD cards
 *  - Transfer files
 *  - Build web pages
 *
 ******************************************************************************/
#ifndef WiFi_Manager_H
#define WiFi_Manager_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

class WiFiManager
{
public :

WiFiManager(); //  the constructor   
 

    /**********************************************************************
     * Connect to the Wi-Fi network.
     *
     * Parameters 
     * --------------------------------------------------
     * ssid
     *      Wi-Fi network name.
     *
     * password
     *      Wi-Fi password.
     *
     * hostname
     *      mDNS hostname.
     *
     * Returns:
     * --------------------------------------------------
     * true  -> Connected successfully.
     * false -> Connection failed.
     **********************************************************************/
 bool begin (const char* networkSSID , const char* networkPassword , const char* hostname );

   /**********************************************************************
     * Called repeatedly inside loop().
     *
     * Responsible for checking the Wi-Fi connection and reconnecting
     * automatically if necessary.
     **********************************************************************/

    void update();

        /**********************************************************************
     * Returns true if Wi-Fi is connected.
     **********************************************************************/
    bool isConnected()const;

        /**********************************************************************
     * Returns the current IP address.
     *
     * Example:
     *      192.168.1.50
     **********************************************************************/
    String ipAddress()const;

    /**********************************************************************
     * Returns the  MAC address.
     **********************************************************************/
    String macAddress()const ; 

       /**********************************************************************
     * Returns the Wi-Fi signal strength (RSSI).
     *
     * Example:
     *      -45 dBm
     **********************************************************************/

  int32_t signalStrength() const;

     /**********************************************************************
     * Returns the configured mDNS hostname.
     *
     * Example:
     *      esp32nas.local
     **********************************************************************/

    String hostname()const ;

    // Returns the connected Wi-Fi SSID
String ssid() const;


// Returns the gateway IP address
String gatewayIP() const;

    private :
     
    // current WI_FI connection state 

    bool connected ;

    // mDNS hostname 

    String hostName ;


};
#endif // WiFi_Manager_H