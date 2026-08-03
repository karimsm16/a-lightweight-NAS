/******************************************************************************
 * File        : HttpServer.h
 * Project     : ESP32 NAS
 * Author      : Abde nacer
 *
 * Description:
 * ---------------------------------------------------------------------------
 * HttpServer is responsible for all HTTP communication with web clients.
 *
 * Responsibilities:
 *  - Start the HTTP server
 *  - Stop the HTTP server
 *  - Register HTTP routes
 *  - Handle incoming client requests
 *  - Provide access to the project's web interface
 *
 * This class DOES Not:
 *  - Access the SD card directly
 *  - Manage Wi-Fi connections
 *  - Perform file operations
 *  - Store project data
 *
 ******************************************************************************/

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Arduino.h>
#include <WebServer.h>

#include "../Storage/SDManager.h"
#include "../Services/FileService.h"
#include "../Network/WiFiManager.h"

class HttpServer
{
    public :
     
    // the constructor , receives refrences to the project's managers
    HttpServer(SDManager& sdMnager,FileService& fileService,WiFiManager& wifiManger);

/*
  Starts the HTTP server.
     *
     * Returns:
     * --------------------------------------------------
     * true  -> Server started successfully.
     * false -> Failed to start.
*/
bool begin ();

/*
  Handles incoming HTTP requests.
     *
     * Must be called repeatedly inside loop().
*/

void handleClient () ;

// stops the HTTP server 
void stop();

// returns TRUE if the server is running
bool isRunning() const ;

private :

// register all HTTP routes 
void registerRoutes();


// Route handlers
void handleHome();
void handleStatus();
void handleFileList();
void handleStorageInfo();
void handleSystemInfo();
void handleUpload();
void handleDownload();
void handleDelete();
void handleRename();
void handleCreateFolder();

// refrences to other project modules 
SDManager& sd ;
FileService& files ;
WiFiManager& wifiManager ;


// internal HTTP server 
WebServer server ;

// server state 
bool running ;

};
#endif 




 

