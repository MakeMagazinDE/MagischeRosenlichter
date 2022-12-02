//-----------------------------------------------------------------------------
//  CCmdPreferences.cpp
/// @file
///
/// @brief Contains the CLI-functions for preferences
///
/// $Date: 2022-08-26 10:58:29 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 167 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <Arduino.h>
#include "FreeRTOS_CLI.h"

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "CCmdPreferences.h"

#include "AppPreferences.h"

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

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Function ShowPrefCommandFct
///
/// @brief Command function for 'Show preferences'-command
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
static BaseType_t ShowPrefCommandFct( char *pcWriteBuffer,
                                      size_t xWriteBufferLen,
                                      const char *pcCommandString )
{
   UNUSED(pcCommandString);

   pcWriteBuffer[0] = '\0';
   xWriteBufferLen = 0;

   Serial.printf("Device type:   %i\r\n", AppPreferences::GetType());
   Serial.printf("Device name:   %s\r\n", AppPreferences::GetName().c_str());
   Serial.printf("WLAN SSID:     %s\r\n", AppPreferences::GetSSID().c_str());
   Serial.printf("WLAN password: %s\r\n", AppPreferences::GetPassword().c_str());
   Serial.printf("Last host:     %s\r\n", AppPreferences::GetLastHost().c_str());
   Serial.printf("Flags:         0x%02x\r\n", AppPreferences::GetFlags());

   return pdFALSE;
}

/// Command structure for the 'showPref'-command
const CLI_Command_Definition_t xShowPrefCommand =
{
    "showPref",
    "showPref: Shows the app preferences\r\n",
    ShowPrefCommandFct,
    0
};

//-----------------------------------------------------------------------------
//  Function SetTypeCommandFct
///
/// @brief Command function for 'Set device type'-command
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
static BaseType_t SetTypeCommandFct( char *pcWriteBuffer,
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
            AppPreferences::SetType(atoi((char *)pcParameter));
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;

         pcWriteBuffer[0] = '\0';
      }
      else
      {
         sprintf(pcWriteBuffer, "Preference changed!\r\n");
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'setType'-command
const CLI_Command_Definition_t xSetTypeCommand =
{
    "setType",
    "setType: Sets the type preference\r\n",
    SetTypeCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function SetSsidCommandFct
///
/// @brief Command function for 'Set ssid'-command
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
static BaseType_t SetSsidCommandFct( char *pcWriteBuffer,
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
            AppPreferences::SetSSID((char *)pcParameter);
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;

         pcWriteBuffer[0] = '\0';
      }
      else
      {
         sprintf(pcWriteBuffer, "Preference changed!\r\n");
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'setSsid'-command
const CLI_Command_Definition_t xSetSsidCommand =
{
    "setSsid",
    "setSsid: Sets the SSID preference\r\n",
    SetSsidCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function SetPwCommandFct
///
/// @brief Command function for 'Show wlan password'-command
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
static BaseType_t SetPwCommandFct( char *pcWriteBuffer,
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
            AppPreferences::SetPassword((char *)pcParameter);
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;

         pcWriteBuffer[0] = '\0';
      }
      else
      {
         sprintf(pcWriteBuffer, "Preference changed!\r\n");
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'setPw'-command
const CLI_Command_Definition_t xSetPwCommand =
{
    "setPw",
    "setPw: Sets the WLAN password\r\n",
    SetPwCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function SetNameCommandFct
///
/// @brief Command function for 'Show device name'-command
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
static BaseType_t SetNameCommandFct( char *pcWriteBuffer,
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
            AppPreferences::SetName((char *)pcParameter);
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;

         pcWriteBuffer[0] = '\0';
      }
      else
      {
         sprintf(pcWriteBuffer, "Preference changed!\r\n");
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'setName'-command
const CLI_Command_Definition_t xSetNameCommand =
{
    "setName",
    "setName: Sets the device name\r\n",
    SetNameCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function SetMasterCommandFct
///
/// @brief Command function for 'Set master flag'-command
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
static BaseType_t SetMasterCommandFct( char *pcWriteBuffer,
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
         int tmp;

         if (1 == lParameterNumber)
         {
            tmp = atoi((char *)pcParameter);

            uint8_t flags = AppPreferences::GetFlags();

            if (0 == tmp)
            {
               flags &= 0xFE;
            }
            else
            {
               flags |= 0x01;
            }

            AppPreferences::SetFlags(flags);
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;

         pcWriteBuffer[0] = '\0';
      }
      else
      {
         sprintf(pcWriteBuffer, "Preference changed!\r\n");
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'setName'-command
const CLI_Command_Definition_t xSetMasterCommand =
{
    "setMaster",
    "setMaster: Sets the master flag\r\n",
    SetMasterCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function CCmd_RegCmd_Preferences
///
/// @brief Registers all available cli-function for the 'Preferences'
///
//-----------------------------------------------------------------------------
void CCmd_RegCmd_Preferences(void)
{
   FreeRTOS_CLIRegisterCommand( &xShowPrefCommand );
   FreeRTOS_CLIRegisterCommand( &xSetTypeCommand );
   FreeRTOS_CLIRegisterCommand( &xSetSsidCommand );
   FreeRTOS_CLIRegisterCommand( &xSetPwCommand );
   FreeRTOS_CLIRegisterCommand( &xSetNameCommand );
   FreeRTOS_CLIRegisterCommand( &xSetMasterCommand );
}

