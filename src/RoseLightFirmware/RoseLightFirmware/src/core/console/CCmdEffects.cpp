//-----------------------------------------------------------------------------
//  CCmdEffects.cpp
/// @file
///
/// @brief Contains the CLI-functions for LED-effects
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
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
#include "CCmdEffects.h"
#include "ledeffectmgr.h"

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
/// For access to the LED-effect manager instance
extern CLedEffectMgr LedEffectMgr;

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
/// @brief Command function for 'Start LED-effect'-command
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
static BaseType_t StartEffectCommandFct( char *pcWriteBuffer,
                                         size_t xWriteBufferLen,
                                         const char *pcCommandString )
{
   BaseType_t lParameterStringLength, xReturn;
   int8_t *pcParameter;
   static int EffectNumber;

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
            EffectNumber = atoi(((char *)pcParameter));
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;

         pcWriteBuffer[0] = '\0';
      }
      else
      {
         sprintf(pcWriteBuffer, "Effect activated!\r\n");

         LedEffectMgr.StartEffect((LedEffectId)EffectNumber);
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'setName'-command
const CLI_Command_Definition_t xStartEffect =
{
    "startEffect",
    "startEffect: Starts an led effect\r\n",
    StartEffectCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function StopEffectCommandFct
///
/// @brief Command function for 'Spot LED-effect'-command
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
static BaseType_t StopEffectCommandFct( char *pcWriteBuffer,
                                        size_t xWriteBufferLen,
                                        const char *pcCommandString )
{
   BaseType_t lParameterStringLength, xReturn;
   int8_t *pcParameter;
   static int StopEffectSequenceVal;

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
            StopEffectSequenceVal = atoi(((char *)pcParameter));
         }

         /* There might be more parameters to return after this one, so again
         set xReturn to pdTRUE. */
         xReturn = pdTRUE;
         lParameterNumber++;

         pcWriteBuffer[0] = '\0';
      }
      else
      {
         sprintf(pcWriteBuffer, "Effect deactivated!\r\n");
         LedEffectMgr.StopEffect((StopEffectSequence)StopEffectSequenceVal);
         xReturn = pdFALSE;
      }
   }

   return xReturn;
}

/// Command structure for the 'stopEffect'-command
const CLI_Command_Definition_t xStopEffect =
{
    "stopEffect",
    "stopEffect: Stops an ruuning led effect\r\n",
    StopEffectCommandFct,
    1
};

//-----------------------------------------------------------------------------
//  Function CCmd_RegCmd_Effects
///
/// @brief Registers all available cli-function for the 'LED-effects'
///
//-----------------------------------------------------------------------------
void CCmd_RegCmd_Effects(void)
{
   FreeRTOS_CLIRegisterCommand( &xStartEffect );
   FreeRTOS_CLIRegisterCommand( &xStopEffect );
}