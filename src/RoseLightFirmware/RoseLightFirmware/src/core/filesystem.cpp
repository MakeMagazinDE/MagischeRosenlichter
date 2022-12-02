
#include "spiffs.h"

#include "filesystem.h"

void listFilesInDir(File dir, int numTabs);

void FileSys_Init(void)
{
  if(!SPIFFS.begin())
  { 
    Serial.println("An Error has occurred while mounting SPIFFS");  
  }
}

void FileSys_Format(void)
{
    bool formatted = SPIFFS.format();
 
    if ( formatted )
    {
        Serial.println("SPIFFS formatted successfully");
    } 
    else 
    {
        Serial.println("Error formatting");
    }
}

void FileSys_PrintInfo(void)
{
// Get all information of SPIFFS
 
    unsigned int totalBytes = SPIFFS.totalBytes();
    unsigned int usedBytes = SPIFFS.usedBytes();
 
    Serial.println("===== File system info =====");
 
    Serial.print("Total space:      ");
    Serial.print(totalBytes);
    Serial.println("byte");
 
    Serial.print("Total space used: ");
    Serial.print(usedBytes);
    Serial.println("byte");
 
    Serial.println();
 
    // Open dir folder
    File dir = SPIFFS.open("/");
    // List file at root
    listFilesInDir(dir, 3);
}

File FileSys_OpenFile(const char* path, const char* mode)
{
    File file = SPIFFS.open(path, mode);
  
    if(!file)
    {
        // File not found 
        Serial.println("Failed to open counter file");
        return file;
    }

    file.println("01.01.1980  12:00:00;Log cleared or deleted"); 

    return file;
}

void FileSys_CloseFile(File * pFile)
{
    if (NULL != pFile)
        pFile->close();
}

void listFilesInDir(File dir, int numTabs) 
{
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files in the folder
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      listFilesInDir(entry, numTabs + 1);
    } else {
      // display zise for file, nothing for directory
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}