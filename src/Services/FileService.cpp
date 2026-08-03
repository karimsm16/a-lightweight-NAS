
 /*    
 * File        : FileService.cpp
 * Project     : ESP32 NAS
 * Author      : Abde nacer
 *
 * Description:
 * ---------------------------------------------------------------------------
 * Implements the FileService class.
 *
 * FileService provides high-level file management operations while relying on
 * SDManager for all communication with the SD card.
 */
#include "FileService.h"

/*
 * Constructor
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Stores a reference to the project's SDManager.
 *
 * We do NOT create another SDManager object.
 * We simply use the one that already exists.
*/
FileService::FileService(SDManager& sdManager)
    : sd(sdManager)
{}
bool FileService::exists(const String& path)
{
return sd.exists(path);
}
/*
  createFolder()
 
   Purpose:
  ---------------------------------------------------------------------------
  Creates a new folder.
*/
bool FileService::createFolder(const String& path )
{
    return sd.createFolder(path);
}
/*
deleteFile()
purpose :
deletes a file 
*/
bool FileService::deleteFile(const String& path) 
{
    return sd.remove(path);

}

/******************************************************************************
 * deleteFolder()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Deletes a folder.
 *
 * NOte:
 * ---------------------------------------------------------------------------
 * SD.remove() can also remove an empty directory on ESP32.
 *
 ******************************************************************************/
bool FileService::deleteFolder(const String& path)
{
    return sd.remove(path);
}

/******************************************************************************
 * rename()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Renames a file or folder.
 *
 ******************************************************************************/
bool FileService::rename(const String& oldPath ,const String& newPath)
{
return sd.rename(oldPath,newPath);
}


/******************************************************************************
 * fileSize()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns the size of a file in bytes.
 *
 * If the file does not exist, 0 is returned.
 *
 ******************************************************************************/
uint64_t FileService::fileSize(const String& path)
{
    // open the file in the read mode 
    File file = sd.open(path,FILE_READ);

    // if the opening failed 
    if (!file)
    {
        return 0 ;
    }

    // get the file size
    uint64_t size = file.size(); 

    // always close the file 
    file.close();

    return size ;
}

/******************************************************************************
 * copyFile()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Copies one file into another.
 *
 * Example:
 *
 *      copyFile("/photo.jpg", "/backup/photo.jpg");
 *
 ******************************************************************************/

 bool FileService::copyFile(const String& source ,const String& destination)
 {

    //open source file 
    File sourceFile =sd.open(source,FILE_READ);

    if (!sourceFile)
    {
      return false ;
    }

    // Open destination file 
    File destinationFile  = sd.open(destination,FILE_WRITE);
    if (!destinationFile)
    {
        sourceFile.close();
        return false;
    }
 // copy every byt
    while(sourceFile.available())
    {
        destinationFile.write(sourceFile.read());
    }
     sourceFile.close();
     destinationFile.close();
      return true ;
}
/******************************************************************************
 * moveFile()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Moves a file.
 *
 * Since SD cards do not provide a dedicated "move" operation,
 * moving is performed as:
 *
 *      Copy
 *      +
 *      Delete original
 *
 ******************************************************************************/

 bool FileService::moveFile(const String& source ,const String& destination )
 {
    // First copy the file 
    if (!copyFile(source,destination))
    {
       return false ;
    }

   // Delete the original 
   return deleteFile(source);

 }
 std::vector<FileInfo> FileService::listFiles(const String& path)
{
    std::vector<FileInfo> files;

File root = sd.open(path, FILE_READ);


    if(!root)
    {
        return files;
    } 

    if (!root.isDirectory())
{
    root.close();
    return files;
}


    File file = root.openNextFile();


    while(file)
    {
        FileInfo info;


      info.name = String(file.name());


        if(file.isDirectory())
        {
           info.isDirectory = true;
            info.size = 0;
        }
        else
        {
           info.isDirectory = false;
            info.size = file.size();
        }


        files.push_back(info);
        file.close();
        file = root.openNextFile();
    }

    root.close();
    return files;
}

