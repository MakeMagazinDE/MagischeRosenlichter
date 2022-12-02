//-----------------------------------------------------------------------------
//  hardware.cpp
/// @file
///
/// @brief Contains the implementations for the clas CHardware
///
/// This Class provides the hardware information
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

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "hardware.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Type detection pin bit 0
#define TYPE_DETECT_PIN_BIT_0 18
/// Type detection pin bit 1
#define TYPE_DETECT_PIN_BIT_1 19
/// Hardware version bit 0
#define HW_VERSION_PIN_BIT_0  4

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
//  Method CHardware
///
/// @brief Constructor of the class CHardware
///
//-----------------------------------------------------------------------------
CHardware::CHardware(void)
{
   pinMode(TYPE_DETECT_PIN_BIT_0, INPUT_PULLDOWN);
   pinMode(TYPE_DETECT_PIN_BIT_1, INPUT_PULLDOWN);

   pinMode(HW_VERSION_PIN_BIT_0,  INPUT_PULLDOWN);
}

//-----------------------------------------------------------------------------
//  Method GetHardwareVersion
///
/// @brief Gets the hardware version of the device
///
/// @return Version of the device
///
//-----------------------------------------------------------------------------
uint8_t CHardware::GetHardwareVersion(void)
{
   uint8_t retVal = 1;

   if (1 == digitalRead(HW_VERSION_PIN_BIT_0))
   {
      retVal = 2;
   }

   return retVal;
}

//-----------------------------------------------------------------------------
//  Method GetType
///
/// @brief Gets the type of the device
///
/// @return Type of the device
///
/// @see GetHardwareVersion()
///
//-----------------------------------------------------------------------------
RoseLightType CHardware::GetType(void)
{
   RoseLightType retVal = RoseLightType::Unknown;

   if (   1 == digitalRead(TYPE_DETECT_PIN_BIT_0)
       && 0 == digitalRead(TYPE_DETECT_PIN_BIT_1))
   {
      retVal = RoseLightType::Type1;
   }
   else if (   0 == digitalRead(TYPE_DETECT_PIN_BIT_0)
            && 1 == digitalRead(TYPE_DETECT_PIN_BIT_1))
   {
      retVal = RoseLightType::Type2;
   }
   else if (   1 == digitalRead(TYPE_DETECT_PIN_BIT_0)
            && 1 == digitalRead(TYPE_DETECT_PIN_BIT_1))
   {
      retVal = RoseLightType::Type3;
   }

   return retVal;
}
