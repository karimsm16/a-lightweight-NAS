#ifndef SD_Manager_H
#define SD_Manager_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SDManager 
{
public:

//constructor
SDManager() ;

// SD inialize the SD card 
bool begin (uint8_t csPin);

// returns true if the SD card is mounted 
bool isMounted() const ;

// Storage information 
uint64_t totalBytes() const ;
uint64_t usedBytes() const ;
uint64_t freeBytes() const ;

// card information 

String cardType() const ;

//  file operation 
bool exists(const String& path) const ;
File open(const String& path , const char*mode ) ;
bool remove(const String& path );
bool rename(const String& oldPath , const String& newPath) ;
bool createFolder(const String& path);

// print SD information to the serial monitor 

void printCardInfo() const ;

private:

bool mounted ;


} ;
#endif