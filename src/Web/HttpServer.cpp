/******************************************************************************
 * File        : HttpServer.cpp
 * Project     : ESP32 NAS
 * Author      : Abde nacer
 *
 * Description:
 * ---------------------------------------------------------------------------
 * Implements the HttpServer class.
 ******************************************************************************/
#include "HttpServer.h"

// constructor : stores refrences o the project's managers , the HTTPServer does not create new managers, it simply uses the ones in main.cpp
HttpServer::HttpServer(
    SDManager& sdManager,
    FileService& fileService,
    WiFiManager& wifiManager)
    : sd(sdManager),
      files(fileService),
      wifiManager(wifiManager),
      server(80),
      running(false)
{
};
 
// begin : starts the HTTPServer .
bool HttpServer:: begin()
{
        Serial.println("=================================");
    Serial.println("Starting HTTP server...");
    Serial.println("=================================");

    registerRoutes();

    Serial.println("Routes registered");

    server.begin();

    Serial.println("server.begin() called");

    running = true;

    Serial.println("HTTP server is running");

    return true;

}
 

/******************************************************************************
 * handleClient()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Processes incoming HTTP requests.
 *
 * This function should be called continuously inside loop().
 ******************************************************************************/

 void HttpServer::handleClient()
 {
    if (!running )
    {
      return ;
    }
    server.handleClient ();

 }
    
/******************************************************************************
 * stop()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Stops the HTTP server.
 ******************************************************************************/
 
 void HttpServer::stop()
 {
   server.stop();
   running = false ;

   Serial.println("Http Server stopped .");


 }

 /******************************************************************************
 * isRunning()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns whether the server is currently running.
 ******************************************************************************/
 bool HttpServer::isRunning()const 
 {
   return running ; 
 }
  
 /******************************************************************************
 * registerRoutes()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Registers all HTTP routes handled by the server.
 *
 * Every URL requested by the browser is associated with a callback function.
 *
 * Example:
 *
 *      GET /
 *      GET /status
 *      GET /files
 *
 ******************************************************************************/
void HttpServer::registerRoutes()
{
// home page 
 server.on("/",HTTP_GET,[this]()
 {
  handleHome();
 });
 
 // serve CSS

 server.on("/style.css", HTTP_GET, [this]()
{
    File file = sd.open("/style.css", FILE_READ);

    if (!file)
    {
        server.send(404, "text/plain", "style.css not found");
        return;
    }

    server.streamFile(file, "text/css");
    file.close();
});

// Serve JavaScript

server.on("/script.js", HTTP_GET, [this]()
{
    File file = sd.open("/script.js", FILE_READ);

    if (!file)
    {
        server.send(404, "text/plain", "script.js not found");
        return;
    }

    server.streamFile(file, "application/javascript");
    file.close();
});

server.on("/api/rename", HTTP_POST, [this]()
{
    handleRename();
});

server.on("/api/mkdir", HTTP_POST, [this]()
{
    handleCreateFolder();
});

// server status 
  server.on("/status",HTTP_GET,[this]()
 {
    handleStatus();
 });

 server.on("/api/system", HTTP_GET, [this]()
{
    handleSystemInfo();
});
    /*
     * Placeholder for future routes
     */
    server.on("/files", HTTP_GET, [this]()
    {
        server.send(
            200,
            "text/plain",
            "File browser coming soon.");
    });

    server.on("/api/network", HTTP_GET, [this]()
{
    String json = "{";

    json += "\"status\":\"";
    json += (wifiManager.isConnected() ? "Connected" : "Disconnected");
    json += "\",";

    json += "\"ssid\":\"";
    json += wifiManager.ssid();
    json += "\",";

    json += "\"ip\":\"";
    json += wifiManager.ipAddress();
    json += "\",";

    json += "\"signal\":";
    json += String(wifiManager.signalStrength());
    json += ",";

    json += "\"mac\":\"";
    json += wifiManager.macAddress();
    json += "\",";

    json += "\"gateway\":\"";
    json += wifiManager.gatewayIP();
    json += "\"";

    json += "}";

    server.send(200, "application/json", json);
});

    server.onNotFound([this]()
    {
        server.send(
            404,
            "text/plain",
            "404 - Page Not Found");
    });
    server.on("/api/storage", HTTP_GET, [this]()
    {
        handleStorageInfo();
    });

    server.on("/api/files", HTTP_GET,[this]()
    {
        handleFileList();
    });

    server.on ("/download",HTTP_GET,[this]()
    {
      handleDownload() ;

    });

    server.on("/api/delete",HTTP_GET,[this]()
    {
      handleDelete();
    });

    server.on(
    "/api/upload",
    HTTP_POST,
    [this]()
    {
        server.send(200, "text/plain", "OK");
    },
    [this]()
    {
        handleUpload();
    }
);

}
void HttpServer::handleHome()
{
    Serial.println("GET /");
    File file = sd.open("/index.html", FILE_READ);
    Serial.println(sd.exists("/index.html"));
    if (!file)
    {
        server.send(404, "text/plain", "index.html not found");
        return;
    }

    server.streamFile(file, "text/html");
    file.close();
}


void HttpServer::handleStatus()
{
    String response;

    response += "ESP32 NAS Status\n";
    response += "-----------------\n";

    response += "WiFi: ";
    response += wifiManager.isConnected() ? "Connected\n" : "Disconnected\n";

    response += "IP: ";
    response += wifiManager.ipAddress();
    response += "\n";

    response += "SD Mounted: ";
    response += sd.isMounted() ? "YES" : "NO";

    server.send(
        200,
        "text/plain",
        response);
}
void HttpServer::handleSystemInfo()
{
    String json = "{";

    json += "\"wifi\":";
    json += wifiManager.isConnected() ? "true" : "false";

    json += ",\"ip\":\"";
    json += wifiManager.ipAddress();
    json += "\"";

    json += ",\"sdMounted\":";
    json += sd.isMounted() ? "true" : "false";

    json += ",\"firmware\":\"1.0\"";

    json += "}";

    server.send(200, "application/json", json);
}
 
void HttpServer::handleStorageInfo()
{
  String json ="{";
 json += "\"total\":";
 json += String(sd.totalBytes());

 json += ",\"used\":";
 json += String(sd.usedBytes());

 json += ",\"free\":";
 json += String(sd.freeBytes());

   json += "}" ;

  server.send(200,"application/json",json);
}
void HttpServer::handleFileList()
{
    String path = "/" ;

    // If the browser requests another folder,
   // use it instead of the root folder.

    if (server.hasArg("path"))
    {
        path = server.arg("path");
    }

    std::vector<FileInfo> filesList = files.listFiles(path);

    String json = "[";

    for (size_t i = 0; i < filesList.size(); i++)
    {
        json += "{";

        json += "\"name\":\"";
        json += filesList[i].name;
        json += "\",";

        json += "\"type\":\"";
        json += filesList[i].isDirectory ? "folder" : "file";
        json += "\",";

        json += "\"size\":";
        json += String(filesList[i].size);

        json += "}";

        if (i < filesList.size() - 1)
        {
            json += ",";
        }
    }

    json += "]";

    server.send(200, "application/json", json);
}
 void HttpServer::handleDownload()
 {
   if (!server.hasArg("file"))
   {
    server.send(400,"text/plain","missing file parameter");
     return ;
   }

   String path = server.arg("file");

   File file = sd.open(path,FILE_READ);

   if (!file)
   {
    server.send(404,"text/plain","File not found");
    return ;
   }

 server.sendHeader(
    "Content-Disposition",
    "attachment; filename=\"" + path.substring(path.lastIndexOf('/') + 1) + "\""
);

server.streamFile(file,"application/octet-stream");
 file.close();

 }

 void HttpServer::handleDelete()
 {

  if(!server.hasArg("file"))
  {
    server.send(400,"text/plain","Missing file parameter");
    return ;

  }

  String path = server.arg("file");
  
 if(files.deleteFile(path) || files.deleteFolder(path))
 {
  server.send(200,"text/plain","File deleted");
 }
 else 
 {
   server.send(404,"text/plain","Delete failed");
 }
 }

 void HttpServer::handleUpload()
 {
   HTTPUpload& upload = server.upload();

   static File file ;

   switch(upload.status)
   {
    case UPLOAD_FILE_START :
    {
      String filename = "/" + upload.filename ;
      file =sd.open(filename,FILE_WRITE);
      break ;
    }
    case UPLOAD_FILE_WRITE:
    {
      if (file)
      {
        file.write(upload.buf,upload.currentSize);
      }
      break ;
    }
    case UPLOAD_FILE_END:
    {
      if (file)
      {
        file.close();
      }
      break ;
    }
    default :
    {
      break ;
    }
   }


 }

 /******************************************************************************
 * handleRename()
 *
 * Renames a file or folder.
 ******************************************************************************/
void HttpServer::handleRename()
{
    if (!server.hasArg("old") || !server.hasArg("new"))
    {
        server.send(400, "text/plain", "Missing parameters");
        return;
    }

    String oldPath = server.arg("old");
    String newPath = server.arg("new");

    if (files.rename(oldPath, newPath))
    {
        server.send(200, "text/plain", "Rename successful");
    }
    else
    {
        server.send(500, "text/plain", "Rename failed");
    }
}

/******************************************************************************
 * handleCreateFolder()
 *
 * Creates a new folder.
 ******************************************************************************/
void HttpServer::handleCreateFolder()
{
    if (!server.hasArg("path"))
    {
        server.send(400, "text/plain", "Missing path");
        return;
    }

    String path = server.arg("path");

    if (files.createFolder(path))
    {
        server.send(200, "text/plain", "Folder created");
    }
    else
    {
        server.send(500, "text/plain", "Failed to create folder");
    }
}