/******************************************************************************
 * File        : SDManager.cpp
 * Project     : ESP32 NAS
 * Author      : Abde nacer
 *
 * Description:
 * ---------------------------------------------------------------------------
 * This file implements the SDManager class.
 *
 * SDManager is the ONLY class responsible for communicating with the SD card.
 *
 * No other class should directly use SD.h.
 *
 * Advantages:
 *  - Easy maintenance.
 *  - Easy debugging.
 *  - Easy to replace the SD library in the future.
 *  - Clean software architecture.
 *
 ******************************************************************************/
#include "SDManager.h"

/******************************************************************************
 * Constructor
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Creates a new SDManager object.
 *
 * This constructor is executed automatically whenever we create:
 *
 *      SDManager sd;
 *
 * Since the ESP32 has just powered on, we assume the SD card is NOT mounted.
 *
 ******************************************************************************/
SDManager::SDManager() 
{

    mounted = false ;
}

/******************************************************************************
 * begin()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Initializes the SD card.
 *
 * Parameter:
 * ---------------------------------------------------------------------------
 * csPin
 *      Chip Select pin connected to the SD module.
 *
 * Returns:
 * ---------------------------------------------------------------------------
 * true
 *      Initialization successful.
 *
 * false
 *      Initialization failed.
 *
 ******************************************************************************/
bool SDManager::begin(uint8_t csPin)
{

    Serial.println();
    Serial.println("=================================");
    Serial.println ("intializing SD card ");
    Serial.println ("================================");

    mounted = SD.begin(csPin);

    if (!mounted)
    {
     Serial.println("ERROR: SD card initialization failed ");
    
    }
    else
     Serial.println(" SUCCESS : SD card initialized ") ;
    
return true ;

}

/******************************************************************************
 * isMounted()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns the current SD mounting status.
 *
 * Returns:
 * ---------------------------------------------------------------------------
 * true
 *      SD card is mounted.
 *
 * false
 *      SD card is unavailable.
 *
 ******************************************************************************/
bool SDManager::isMounted() const
{
return mounted ;
}


/******************************************************************************
 * totalBytes()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns the total storage capacity of the SD card.
 *
 * Example:
 *
 *      31,914,844,160 bytes
 *
 ******************************************************************************/
uint64_t SDManager::totalBytes() const 
{

    return SD.totalBytes();

}


/******************************************************************************
 * usedBytes()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Returns how many bytes are currently occupied.
 *
 ******************************************************************************/

uint64_t SDManager::usedBytes() const
{
    return SD.usedBytes() ;
}


/******************************************************************************
 * freeBytes()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Calculates the remaining free space.
 *
 * Formula:
 *
 *      Free Space = Total Space - Used Space
 *
 ******************************************************************************/

 uint64_t SDManager::freeBytes() const 
 {
    return totalBytes() - usedBytes() ;
 }

 /******************************************************************************
 * cardType()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Converts the SD library card type into a human-readable string.
 *
 * Possible values:
 *
 *      SDSC
 *      SDHC
 *      UNKNOWN
 *
 ******************************************************************************/
String SDManager::cardType() const
{
    uint8_t type = SD.cardType() ;
    switch(type)
    {

         case CARD_MMC :
         return "MMC" ;

         case CARD_SD :
         return "SDSC" ;

         case CARD_SDHC :
         return "SDHC /SDXC " ;

         default :
         return "UNKNOWN" ;

    }
}

/******************************************************************************
 * exists()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Checks whether a file or folder exists.
 *
 ******************************************************************************/

 bool SDManager::exists(const String& path )const 
 {
    return SD.exists(path );
 }
 
/******************************************************************************
 * open()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Opens a file.
 *
 * Examples:
 *
 *      "/photo.jpg"
 *      "/documents/report.pdf"
 *
 ******************************************************************************/
File SDManager :: open(const String& path ,const char* mode ) 
{
    return SD.open (path , mode);
}


/******************************************************************************
 * createFolder()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Creates a new directory (folder) on the SD card.
 *
 * Example:
 *
 *      createFolder("/Images");
 *
 * If the folder already exists, SD.mkdir() simply returns false.
 *
 ******************************************************************************/

 bool SDManager::createFolder(const String& path )
 {
    return SD.mkdir(path);
 }
 
 
/******************************************************************************
 * remove()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Deletes a file from the SD card.
 *
 * Example:
 *
 *      remove("/movie.mp4");
 *
 * Returns:
 * ---------------------------------------------------------------------------
 * true
 *      File deleted successfully.
 *
 * false
 *      File doesn't exist or deletion failed.
 *
 ******************************************************************************/
bool SDManager::remove( const String& path )
{
    return SD.remove (path ) ;
}

/******************************************************************************
 * rename()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Renames a file or a folder.
 *
 * Example:
 *
 *      rename("/old.txt", "/new.txt");
 *
 ******************************************************************************/

 bool SDManager::rename(const String& oldPath ,const String& newPath )
 {
    return SD.rename(oldPath,newPath);
 }

 /******************************************************************************
 * printCardInfo()
 *
 * Purpose:
 * ---------------------------------------------------------------------------
 * Prints useful information about the SD card to the Serial Monitor.
 *
 * This function is mainly used during development and debugging.
 *
 * Example output:
 *
 * ======================================
 * SD CARD INFORMATION
 * ======================================
 * Card Type : SDHC / SDXC
 * Total     : 31.24 GB
 * Used      : 4.51 GB
 * Free      : 26.73 GB
 * Mounted   : YES
 * ======================================
 *
 ******************************************************************************/
void SDManager:: printCardInfo() const
{
 Serial.println() ;
 Serial.println("===========================");
 Serial.println("SD card information");
 Serial.println("===========================");

 if (!mounted)
 { 
    Serial.println (" ERROR : SD card is not mounted . ");
    Serial.println("=========================================================");
    return ;
  }

 Serial.print("Card type :");
 Serial.println(cardType());



    double totalGB =
        (double)totalBytes() / (1024.0 * 1024.0 * 1024.0);

    double usedGB =
        (double)usedBytes() / (1024.0 * 1024.0 * 1024.0);

    double freeGB =
        (double)freeBytes() / (1024.0 * 1024.0 * 1024.0);

        Serial.print("TOTAL: ");
        Serial.print(totalGB,2);
        Serial.println("GB");

        Serial.print("USED  :");
        Serial.print(usedGB,2);
        Serial.println("GB");

        Serial.print("FREE :");
        Serial.print(freeGB,2);
        Serial.println("GB");
        
        Serial.println("Mounted :"); 

        if (mounted )
        Serial.println("YES");
        else
        Serial.println("NO");
        Serial.println("===============================================");
        

}
