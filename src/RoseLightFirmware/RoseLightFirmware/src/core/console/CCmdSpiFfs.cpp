//-----------------------------------------------------------------------------
//  CCmdSpiFfs.cpp
/// @file
///
/// @brief Contains the CLI-functions for spi file system
///
/// $Date: 2022-08-26 12:08:51 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 169 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <Arduino.h>
#include "FreeRTOS_CLI.h"
#include "spiffs.h"

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "CCmdSpiFfs.h"
#include "filesystem.h"

#include "defs.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global members of the module
//-----------------------------------------------------------------------------
/// Test file for cli-test function
File testFile;

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Function InitFsCommandFct
///
/// @brief Command function for 'Init file system'-command
///
/// @param[out] *pcWriteBuffer - Buffer for command answer string
///
/// @param[in] xWriteBufferLen - Maximum size of the buffer for command answer string
///
/// @param[in] *pcCommandString - CLI-command string
///
/// @return Processing flag: pdFALSE - Command processed completely, pdTrue - 
///         Command not completely processed, function must be called another one
///
/// @see CCmd_RegCmd_Preferences()
///
//-----------------------------------------------------------------------------
static BaseType_t InitFsCommandFct( char *pcWriteBuffer,
                                    size_t xWriteBufferLen,
                                    const char *pcCommandString )
{
   UNUSED(pcCommandString);

   pcWriteBuffer[0] = '\0';

   FileSys_Init();

   return pdFALSE;
}

/// Command structure for the 'initFs'-command
const CLI_Command_Definition_t xInitFsCommand =
{
    "initFs",
    "initFs: Initializes the filesystem\r\n",
    InitFsCommandFct,
    0
};

//-----------------------------------------------------------------------------
//  Function FormatFsCommandFct
///
/// @brief Command function for 'Format file system'-command
///
/// @param[out] *pcWriteBuffer - Buffer for command answer string
///
/// @param[in] xWriteBufferLen - Maximum size of the buffer for command answer string
///
/// @param[in] *pcCommandString - CLI-command string
///
/// @return Processing flag: pdFALSE - Command processed completely, pdTrue - 
///         Command not completely processed, function must be called another one
///
/// @see CCmd_RegCmd_Preferences()
///
//-----------------------------------------------------------------------------
static BaseType_t FormatFsCommandFct( char *pcWriteBuffer,
                                      size_t xWriteBufferLen,
                                      const char *pcCommandString )
{
   UNUSED(pcCommandString);

   pcWriteBuffer[0] = '\0';

   FileSys_Format();

   return pdFALSE;
}

/// Command structure for the 'formatFs'-command
const CLI_Command_Definition_t xFormatFsCommand =
{
    "formatFs",
    "formatFs: Formats the filesystem\r\n",
    FormatFsCommandFct,
    0
};

//-----------------------------------------------------------------------------
//  Function InfoFsCommandFct
///
/// @brief Command function for 'Information about file system'-command
///
/// @param[out] *pcWriteBuffer - Buffer for command answer string
///
/// @param[in] xWriteBufferLen - Maximum size of the buffer for command answer string
///
/// @param[in] *pcCommandString - CLI-command string
///
/// @return Processing flag: pdFALSE - Command processed completely, pdTrue - 
///         Command not completely processed, function must be called another one
///
/// @see CCmd_RegCmd_Preferences()
///
//-----------------------------------------------------------------------------
static BaseType_t InfoFsCommandFct( char *pcWriteBuffer,
                                    size_t xWriteBufferLen,
                                    const char *pcCommandString )
{
   UNUSED(pcCommandString);

   pcWriteBuffer[0] = '\0';

   FileSys_PrintInfo();

   return pdFALSE;
}

/// Command structure for the 'infoFs'-command
const CLI_Command_Definition_t xInfoFsCommand =
{
    "infoFs",
    "infoFs: Prints filesystem information\r\n",
    InfoFsCommandFct,
    0
};

//-----------------------------------------------------------------------------
//  Function FileOpenCommandFct
///
/// @brief Command function for 'Open file'-command
///
/// @param[out] *pcWriteBuffer - Buffer for command answer string
///
/// @param[in] xWriteBufferLen - Maximum size of the buffer for command answer string
///
/// @param[in] *pcCommandString - CLI-command string
///
/// @return Processing flag: pdFALSE - Command processed completely, pdTrue - 
///         Command not completely processed, function must be called another one
///
/// @see CCmd_RegCmd_Preferences()
///
//-----------------------------------------------------------------------------
static BaseType_t FileOpenCommandFct( char *pcWriteBuffer,
                                      size_t xWriteBufferLen,
                                      const char *pcCommandString )
{
   BaseType_t lParameterStringLength, xReturn;
   int8_t *pcParameter;

   /* Note that the use of the static parameter means this function is not reentrant. */
   static BaseType_t lParameterNumber = 0;

   if( lParameterNumber == 0 )
   {
      /* lParameterNumber is 0, so this is the first time the function has been
      called since the command was entered. No output at this point. */
      pcWriteBuffer[ 0 ] = 0x00;

      /* Next time the function is called the first parameter will be echoed
      back. */
      lParameterNumber = 1L;

      /* There is more data to be returned as no parameters have been echoed
      back yet, so set xReturn to pdPASS so the function will be called again. */
      xReturn = pdPASS;
   }
   else
   {
      /* lParameter is not 0, so holds the number of the parameter that should
      be returned.  Obtain the complete parameter string. */
      pcParameter = ( int8_t * ) FreeRTOS_CLIGetParameter
                                 (
                                     /* The command string itself. */
                                     pcCommandString,
                                     /* Return the next parameter. */
                                     lParameterNumber,
                                     /* Store the parameter string length. */
                                     &lParameterStringLength
                                 );

      if( pcParameter != NULL )
      {
         if (1 == lParameterNumber)
         {
            testFile = FileSys_OpenFile((char *)pcParameter, "w");

            if (!testFile)
            {
               sprintf(pcWriteBuffer, "Can't create/open file %s!\r\n", (char *)pcParameter);
            }
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;
      }
      else
      {
         pcWriteBuffer[0] = '\0';
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'fileOpen'-command
const CLI_Command_Definition_t xFileOpenCommand =
{
    "fileOpen",
    "fileOpen: Open a file\r\n",
    FileOpenCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function FileCloseCommandFct
///
/// @brief Command function for 'File close'-command
///
/// @param[out] *pcWriteBuffer - Buffer for command answer string
///
/// @param[in] xWriteBufferLen - Maximum size of the buffer for command answer string
///
/// @param[in] *pcCommandString - CLI-command string
///
/// @return Processing flag: pdFALSE - Command processed completely, pdTrue - 
///         Command not completely processed, function must be called another one
///
/// @see CCmd_RegCmd_Preferences()
///
//-----------------------------------------------------------------------------
static BaseType_t FileCloseCommandFct( char *pcWriteBuffer,
                                       size_t xWriteBufferLen,
                                       const char *pcCommandString )
{
   UNUSED(pcCommandString);

   pcWriteBuffer[0] = '\0';

   FileSys_CloseFile(&testFile);

   return pdFALSE;
}

/// Command structure for the 'fileClose'-command
const CLI_Command_Definition_t xFileCloseCommand =
{
    "fileClose",
    "fileClose: Closes a file\r\n",
    FileCloseCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function CCmd_RegCmd_SpiFfs
///
/// @brief Registers all available cli-function for the 'Spi file system'
///
//-----------------------------------------------------------------------------
void CCmd_RegCmd_SpiFfs(void)
{
   FreeRTOS_CLIRegisterCommand( &xInitFsCommand );
   FreeRTOS_CLIRegisterCommand( &xFormatFsCommand );
   FreeRTOS_CLIRegisterCommand( &xInfoFsCommand );
   FreeRTOS_CLIRegisterCommand( &xFileOpenCommand );
   FreeRTOS_CLIRegisterCommand( &xFileCloseCommand );
}