/******************************************************************************
 * File        : FileService.h
 * Project     : ESP32 NAS
 * Author      : Abde nacer
 *
 * Description:
 * ---------------------------------------------------------------------------
 * FileService provides high-level file management operations.
 *
 * It acts as the bridge between the web server and the SD card.
 *
 * Responsibilities:
 *  - List files and folders
 *  - Create folders
 *  - Delete files
 *  - Delete folders
 *  - Rename files and folders
 *  - Copy files
 *  - Move files
 *  - Retrieve file information
 *
 * This class DOES Not:
 *  - Communicate directly with the browser
 *  - Manage Wi-Fi
 *  - Handle HTTP requests
 *
 ******************************************************************************/
#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include<Arduino.h>
#include<FS.h>
#include "../Storage/SDManager.h"
#include<vector>


struct FileInfo
{
    String name;
    bool isDirectory;
    uint64_t size;
};


class FileService 
{


    public :
    // the constructor 
   FileService(SDManager& sdManager);
   std::vector<FileInfo> listFiles(const String& path);
    /**********************************************************************
     * Lists the contents of a folder.
     *
     * Returns:
     *      true  -> Folder listed successfully.
     *      false -> Folder not found.
     **********************************************************************/

     bool listDirectory(const String& path);
      
     // create new folder 
     bool createFolder(const String&);

     // delete file 
     bool deleteFile(const String& path);

     // delete an empty folder 
     bool deleteFolder(const String& path);

     // rename a file or folder 
     bool rename (const String& oldPath , const String& newPath);

     // copies a file 
        bool copyFile(const String& source , const String& destination);

     // moves a file 
     bool moveFile(const String& source , const String& destination );

     // returns the size of a file in Bytes 
     uint64_t fileSize(const String& path);

     // returns true if a file or folder exists 
     bool exists(const String& path);

    private :

     // Reference to the SD card manager 

     SDManager& sd;
};
#endif 
