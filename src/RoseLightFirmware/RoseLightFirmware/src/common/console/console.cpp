//-----------------------------------------------------------------------------
//  console.c
/// @file
///
/// @brief Module with console function for the system
///
/// This module contains all common console function of the application.
///
/// $Date: 2022-08-26 09:53:26 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 166 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <Arduino.h>
//#include <string.h>
//#include <stdarg.h>
//#include <stdio.h>

//#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
//#include "os.h"
//#include "app_config.h"

//#include "stm32_hal.h"

#include "console.h"

//-----------------------------------------------------------------------------
// Private definitions, macros and constants
//-----------------------------------------------------------------------------
/// Maximum length of the input string
#define MAX_INPUT_LENGTH       50
/// Maximum length of the output string
#define MAX_OUTPUT_LENGTH     600

/// Maximum size for a trace (INFO, WARNING, ERROR)
#define MAX_DEBUG_MSG_LENGTH  200

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
#ifdef HGSDISPLAY
/// Handle for access to UART2
//extern UART_HandleTypeDef huart2;
//#elif defined HGSPLUS
/// Handle for access to UART1
//extern UART_HandleTypeDef huart1;
#else
/// Handle for access to UART3
//extern UART_HandleTypeDef huart3;
#endif

//-----------------------------------------------------------------------------
// Global member for the module
//-----------------------------------------------------------------------------
/// Welcome message of the console
static const char * const WelcomeMessage =
   "\r\nFreeRTOS command server.\r\nType Help to view a list of registered commands.\r\n\r\n";

/// Pointer to the current used UART handle (controlling structure)
//UART_HandleTypeDef * phuart = NULL;
/// Task id of the console task
static uint32_t ConsoleTaskID = 0;

/// String for saving the last processed console command (can be repeated by pressing key ^ [0x1b])
static char pcLastInputString[MAX_INPUT_LENGTH];
/// Flag: Error detect on uart for console communication
uint8_t IsConsoleUartRecError = 0;

//-----------------------------------------------------------------------------
// Prototypes of private functions
//-----------------------------------------------------------------------------
// Outputs a string at the console
void ConsoleOut(uint8_t *pData, uint16_t Size);
// Outputs a string as info string at the console
void ConsoleOut_DbgMsgInfo(const char * pString, ...);
// Outputs a string as warning string at the console
void ConsoleOut_DbgMsgWarning(const char * pString, ...);
// Outputs a string as error string at the console
void ConsoleOut_DbgMsgError(const char * pString, ...);
// Outputs the text "Info: " at the console
static void ConsoleOut_InfoString(void);
// Outputs the text "Warning: " at the console
static void ConsoleOut_WarningString(void);
// Outputs the text "Error: " at the console
static void ConsoleOut_ErrorString(void);
// Outputs the text "\r\n" at the console
static void ConsoleOut_InfoCRLF(void);
// Displays the welcome message
static void DisplayWelcomeMessage();

//-----------------------------------------------------------------------------
// Implementations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Function RegisterCommands
///
/// @brief Register function for all the console commands
///
/// This is the register function for all the console commands. The function
/// should be overwrite in the file(s) for the application specified console
/// commands.
///
//-----------------------------------------------------------------------------
__attribute__((weak))
void RegisterCommands(void)
{
  /* NOTE : This function should not be modified, when the register function is needed,
            the RegisterCommands function could be implemented in the user file
   */
}

//-----------------------------------------------------------------------------
//  Function  DisplayWelcomeMessage
///
/// @brief Displays the welcome message
///
/// This function displays the welcome message.
///
//-----------------------------------------------------------------------------
static void DisplayWelcomeMessage()
{
   Serial.println(WelcomeMessage);

   (void)vTaskDelay(10);
}

//-----------------------------------------------------------------------------
//  Function  console_task_fct
///
/// @brief Task function of the console-task
///
/// @param pvParameters (@c [in] void const *) - Unused
///
/// This is the task function of the console task.
///
//-----------------------------------------------------------------------------
void console_task_fct( void *pvParameters )
{
   String cmdData;
   String tmpData;
   // int8_t cInputIndex = 0;
   BaseType_t xMoreDataToFollow;
// cRxedChar
   /* The input and output buffers are declared static to keep them off the stack. */
   static char pcOutputString[MAX_OUTPUT_LENGTH];
//   static char pcInputString[MAX_INPUT_LENGTH];

//#ifdef HGSDISPLAY
//   phuart = &huart2;
//#elif defined HGSPLUS
//   phuart = &huart1;
//#else
//   phuart = &huart3;
//#endif

   ConsoleTaskID = (uint32_t) pvParameters;

   RegisterCommands();

   DisplayWelcomeMessage();

#ifdef HGSPLUS
   (void)OS_SemaphoreSetCountToZero(SEMA_ID_CONSOLE_OUT);
   (void)OS_SemaphoreGiveFromISR (SEMA_ID_CONSOLE_OUT);
#endif

   for(;;)
   {
      //String cmdString = Serial.readString();

      vTaskDelay(10);

      for(;;)
      {
         if(Serial.available() > 0)
         {
            //Serial.println("New command received!");

            tmpData = Serial.readString();

            ConsoleOut((uint8_t *)tmpData.c_str(), tmpData.length());

            cmdData += tmpData;

            //Serial.print(cmdData);
            //Serial.println("");

            if (cmdData.charAt(cmdData.length()-1) == 0x0A)
            {
               cmdData.remove((cmdData.length()-2), 2);

               /* A newline character was received, so the input command string is
               complete and can be processed.  Transmit a line separator, just to
               make the output easier to read. */
               ConsoleOut ((uint8_t *)"\r\n", (uint16_t)strlen( "\r\n" ));

               /* The command interpreter is called repeatedly until it returns
               pdFALSE.  See the "Implementing a command" documentation for an
               explanation of why this is. */
               do
               {
                     /* Send the command string to the command interpreter.  Any
                     output generated by the command interpreter will be placed in the
                     pcOutputString buffer. */
                     xMoreDataToFollow = FreeRTOS_CLIProcessCommand( cmdData.c_str(),   /* The command string.*/
                                                                     pcOutputString,  /* The output buffer. */
                                                                     MAX_OUTPUT_LENGTH/* The size of the output buffer. */);

                     /* Write the output generated by the command interpreter to the
                     console. */
                     ConsoleOut ((uint8_t *)pcOutputString, (uint16_t)strlen( (const char *)pcOutputString ));
               } while( xMoreDataToFollow != pdFALSE );

               ConsoleOut ((uint8_t *)"\r\n", (uint16_t)strlen( "\r\n" ));

               strcpy(pcLastInputString, cmdData.c_str());

               /* All the strings generated by the input command have been sent.
               Processing of the command is complete.  Clear the input string ready
               to receive the next command. */
               // cInputIndex = 0;
               //memset( pcInputString, 0x00, MAX_INPUT_LENGTH );

               cmdData = "";
            }
            // else if (cRxedChar == 0x1b)
            // {
            //    if (strlen(pcLastInputString) > 0)
            //    {
            //       strcpy(pcInputString, pcLastInputString);

            //       cInputIndex = strlen( pcInputString );

            //       cRxedChar = 0;

            //       ConsoleOut ((uint8_t *)"\r\n", (uint16_t)strlen( "\r\n" ));

            //       ConsoleOut ((uint8_t *)pcInputString, cInputIndex);
            //    }
            // }
            // else
            // {
            //    /* The if() clause performs the processing after a newline character
            //    is received.  This else clause performs the processing if any other
            //    character is received. */
            //    if( cRxedChar == '\n' || cRxedChar == '[')
            //    {
            //       /* Ignore carriage returns. */
            //    }
            //    else if( cRxedChar == '\b' )
            //    {
            //       /* Backspace was pressed.  Erase the last character in the input
            //       buffer - if there are any. */
            //       if( cInputIndex > 0 )
            //       {
            //          cInputIndex--;
            //          pcInputString[ cInputIndex ] = '\0';
            //       }
            //    }
            //    else
            //    {
            //       /* A character was entered.  It was not a new line, backspace
            //       or carriage return, so it is accepted as part of the input and
            //       placed into the input buffer.  When a \n is entered the complete
            //       string will be passed to the command interpreter. */
            //       if( cInputIndex < MAX_INPUT_LENGTH )
            //       {
            //          pcInputString[ cInputIndex ] = cRxedChar;
            //          cInputIndex++;
            //       }
            //    }
            // }
            else
            {
               break;
            }      
         }
      }
      
   }
}

//-----------------------------------------------------------------------------
//  Function ConsoleOut
///
/// @brief Outputs a string at the console
///
/// @param[in] pData - Pointer to the string
///
/// @param[in] Size - Size of the string
///
//-----------------------------------------------------------------------------
void ConsoleOut(uint8_t * pData, uint16_t Size)
{
   Serial.printf("%s", pData);
}

//-----------------------------------------------------------------------------
//  Function  ConsoleOut_DbgMsgInfo
///
/// @brief Outputs a string as info string at the console
///
/// This function outputs a string as info string at the console. This string
/// has the form: "Info: <contents>\r\n"
///
/// @param[in] pString - Pointer to the string
///
/// @param[in] ... - Argument list
///
//-----------------------------------------------------------------------------
void ConsoleOut_DbgMsgInfo(const char * pString, ...)
{
//#ifdef HGSPLUS
//   (void)OS_SemaphoreTake (SEMA_ID_CONSOLE_OUT, WAIT_MAX);
//#else
//   (void)OS_MutexTake (ConsoleTaskID, WAIT_MAX);
//#endif
   ConsoleOut_InfoString();

   static char dmsg_buffer[MAX_DEBUG_MSG_LENGTH];

   va_list args;
   va_start(args, pString);
   vsprintf(dmsg_buffer, pString, args);
   va_end(args);

//   if (phuart != NULL)
//   {
//      (void)HAL_UART_Transmit(phuart, (uint8_t *) dmsg_buffer, (uint16_t)strlen(dmsg_buffer), 1000);
//   }

   ConsoleOut_InfoCRLF();

//#ifdef HGSPLUS
//   (void)OS_SemaphoreGiveFromISR (SEMA_ID_CONSOLE_OUT);
//#else
//   (void)OS_MutexGive (ConsoleTaskID);
//#endif
}

//-----------------------------------------------------------------------------
//  Function  ConsoleOut_DbgMsgWarning
///
/// @brief Outputs a string as warning string at the console
///
/// This function outputs a string as warning string at the console. This string
/// has the form: "Warning: <contents>\r\n"
///
/// @param[in] pString - Pointer to the string
///
/// @param[in] ... - Argument list
///
//-----------------------------------------------------------------------------
void ConsoleOut_DbgMsgWarning(const char * pString, ...)
{
//#ifdef HGSPLUS
//   (void)OS_SemaphoreTake (SEMA_ID_CONSOLE_OUT, WAIT_MAX);
//#else
//   (void)OS_MutexTake (ConsoleTaskID, WAIT_MAX);
//#endif

   ConsoleOut_WarningString();

   static char dmsg_buffer[MAX_DEBUG_MSG_LENGTH];

   va_list args;
   va_start(args, pString);
   vsprintf(dmsg_buffer, pString, args);
   va_end(args);

//   if (phuart != NULL)
//   {
//      (void)HAL_UART_Transmit(phuart, (uint8_t *) dmsg_buffer, (uint16_t)strlen(dmsg_buffer), 1000);
//   }

   ConsoleOut_InfoCRLF();

//#ifdef HGSPLUS
//   (void)OS_SemaphoreGiveFromISR (SEMA_ID_CONSOLE_OUT);
//#else
//   (void)OS_MutexGive (ConsoleTaskID);
//#endif
}

//-----------------------------------------------------------------------------
//  Function  ConsoleOut_DbgMsgError
///
/// @brief Outputs a string as error string at the console
///
/// This function outputs a string as error string at the console. This string
/// has the form: "Error: <contents>\r\n"
///
/// @param[in] pString - Pointer to the string
///
/// @param[in] ... - Argument list
///
//-----------------------------------------------------------------------------
void ConsoleOut_DbgMsgError(const char * pString, ...)
{
//#ifdef HGSPLUS
//   (void)OS_SemaphoreTake (SEMA_ID_CONSOLE_OUT, WAIT_MAX);
//#else
//   (void)OS_MutexTake (ConsoleTaskID, WAIT_MAX);
//#endif
   ConsoleOut_ErrorString();

   static char dmsg_buffer[MAX_DEBUG_MSG_LENGTH];

   va_list args;
   va_start(args, pString);
   vsprintf(dmsg_buffer, pString, args);
   va_end(args);

//   if (phuart != NULL)
//   {
//      (void)HAL_UART_Transmit(phuart, (uint8_t *) dmsg_buffer, (uint16_t)strlen(dmsg_buffer), 1000);
//   }

   ConsoleOut_InfoCRLF();

//#ifdef HGSPLUS
//   (void)OS_SemaphoreGiveFromISR (SEMA_ID_CONSOLE_OUT);
//#else
//   (void)OS_MutexGive (ConsoleTaskID);
//#endif
}

//-----------------------------------------------------------------------------
//  Function  ConsoleOut_InfoString
///
/// @brief Outputs the text "Info: " at the console
///
/// @see ConsoleOut_DbgMsgInfo()
///
//-----------------------------------------------------------------------------
static void ConsoleOut_InfoString(void)
{
//   if (phuart != NULL)
//   {
//      (void)HAL_UART_Transmit(phuart, (uint8_t *) "Info: ", (uint16_t)strlen("Info: "), 1000);
//   }
}

//-----------------------------------------------------------------------------
//  Function  ConsoleOut_WarningString
///
/// @brief Outputs the text "Warning: " at the console
///
/// @see ConsoleOut_DbgMsgWarning()
///
//-----------------------------------------------------------------------------
static void ConsoleOut_WarningString(void)
{
//   if (phuart != NULL)
//   {
//      (void)HAL_UART_Transmit(phuart, (uint8_t *) "Warning: ", (uint16_t)strlen("Warning: "), 1000);
//   }
}

//-----------------------------------------------------------------------------
//  Function  ConsoleOut_ErrorString
///
/// @brief Outputs the text "Error: " at the console
///
/// @see ConsoleOut_DbgMsgError()
///
//-----------------------------------------------------------------------------
static void ConsoleOut_ErrorString(void)
{
//   if (phuart != NULL)
//   {
//      (void)HAL_UART_Transmit(phuart, (uint8_t *) "Error: ", (uint16_t)strlen("Error: "), 1000);
//   }
}

//-----------------------------------------------------------------------------
//  Function  ConsoleOut_InfoCRLF
///
/// @brief Outputs the text "\r\n" at the console
///
/// @see ConsoleOut_DbgMsgWarning()
///
//-----------------------------------------------------------------------------
static void ConsoleOut_InfoCRLF(void)
{
//   if (phuart != NULL)
//   {
//      (void)HAL_UART_Transmit(phuart, (uint8_t *) "\r\n", (uint16_t)strlen("\r\n"), 1000);
//   }
}
