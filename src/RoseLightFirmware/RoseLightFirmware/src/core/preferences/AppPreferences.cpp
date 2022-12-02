//-----------------------------------------------------------------------------
//  AppPreferences.cpp
/// @file
///
/// @brief Contains the implementation of the class AppPreferences
///
/// The manage the access to the preferences of the rose light firmware application
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "AppPreferences.h"

#include "defs.h"
#include "hardware.h"
#include "version.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// 32-Bit-Value for the software version V1.1.0
#define SW_VERSION_1_1_0   0x00010100
/// Hardware version 2
#define HW_VERSION_2       2

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// For access to the class Hardware
extern CHardware  Hardware;

//-----------------------------------------------------------------------------
// Prototypes of private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global members of the module
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------
/// For access to the preferences base class
static Preferences preferences;

//-----------------------------------------------------------------------------
//  Method begin
///
/// @brief Begin method
///
/// Note: Original comments from the original example of the preference class
/// Open Preferences with RLApp namespace. Each application module, library, etc
/// has to use a namespace name to prevent key name collisions. We will open storage in
/// RW-mode (second parameter has to be false).
/// Note: Namespace name is limited to 15 chars.
///
//-----------------------------------------------------------------------------
void AppPreferences::begin(void)
{
   preferences.begin("RLApp", false);
}

//-----------------------------------------------------------------------------
//  Method GetType
///
/// @brief Gets the preference "type"
///
/// This method gets the preference "type".
/// 
/// Note: For software version greater than V1.1.0 and hardware version greater
/// than 2, we detect the device type from hardware.
/// 
/// @return Preference "type"
///
/// @see SetType()
///
//-----------------------------------------------------------------------------
uint8_t AppPreferences::GetType(void)
{
   if (VERSION_UINT32 >= SW_VERSION_1_1_0 && Hardware.GetHardwareVersion() >= HW_VERSION_2)
   {
      return (uint8_t)Hardware.GetType();
   }

   return preferences.getUChar("type", 0);
}

//-----------------------------------------------------------------------------
//  Method SetType
///
/// @brief Sets the preference "type"
///
/// This method sets the preference "type".
/// 
/// Note: For software version greater than V1.1.0 and hardware version greater
/// than 2, we don't need to store the device type.
///
/// @param[in] type - New value for preference "type"
///
/// @see GetType()
///
//-----------------------------------------------------------------------------
void AppPreferences::SetType(uint8_t type)
{
   if (VERSION_UINT32 >= SW_VERSION_1_1_0 && Hardware.GetHardwareVersion() >= HW_VERSION_2)
      return;

   preferences.putUChar("type", type);
}

//-----------------------------------------------------------------------------
//  Method GetFlags
///
/// @brief Gets the preference "flags"
///
/// @return Preference "flags"
///
/// @see SetFlags()
///
//-----------------------------------------------------------------------------
uint8_t AppPreferences::GetFlags(void)
{
   return preferences.getUChar("flags", 0);
}

//-----------------------------------------------------------------------------
//  Method SetFlags
///
/// @brief Sets the preference "flags"
///
/// @param[in] type - New value for preference "flags"
///
/// @see GetFlags()
///
//-----------------------------------------------------------------------------
void AppPreferences::SetFlags(uint8_t type)
{
   preferences.putUChar("flags", type);
}

//-----------------------------------------------------------------------------
//  Method GetSSID
///
/// @brief Gets the preference "ssid"
///
/// @return Preference "ssid"
///
/// @see SetSSID()
///
//-----------------------------------------------------------------------------
String AppPreferences::GetSSID(void)
{
   return preferences.getString("ssid", "No SSID");
}

//-----------------------------------------------------------------------------
//  Method SetSSID
///
/// @brief Sets the preference "ssid"
///
/// @param[in] pSSID - New value for preference "ssid"
///
/// @see GetSSID()
///
//-----------------------------------------------------------------------------
void AppPreferences::SetSSID(char * pSSID)
{
   preferences.putString("ssid", pSSID);
}

//-----------------------------------------------------------------------------
//  Method GetPassword
///
/// @brief Gets the preference "password"
///
/// @return Preference "password"
///
/// @see SetPassword()
///
//-----------------------------------------------------------------------------
String AppPreferences::GetPassword(void)
{
   return preferences.getString("password", "No password");
}

//-----------------------------------------------------------------------------
//  Method SetPassword
///
/// @brief Sets the preference "password"
///
/// @param[in] pPassword - New value for preference "password"
///
/// @see GetPassword()
///
//-----------------------------------------------------------------------------
void AppPreferences::SetPassword(char * pPassword)
{
   preferences.putString("password", pPassword);
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the preference "name"
///
/// @return Preference "name"
///
/// @see SetName()
///
//-----------------------------------------------------------------------------
String AppPreferences::GetName(void)
{
   return preferences.getString("name", "No name");
}

//-----------------------------------------------------------------------------
//  Method SetName
///
/// @brief Sets the preference "name"
///
/// @param[in] pName - New value for preference "name"
///
/// @see GetName()
///
//-----------------------------------------------------------------------------
void AppPreferences::SetName(char * pName)
{
   preferences.putString("name", pName);
}

//-----------------------------------------------------------------------------
//  Method GetLastHost
///
/// @brief Gets the preference "lasthost"
///
/// @return Preference "lasthost"
///
/// @see SetLastHost()
///
//-----------------------------------------------------------------------------
String AppPreferences::GetLastHost(void)
{
   return preferences.getString("lasthost", "No host");
}

//-----------------------------------------------------------------------------
//  Method SetLastHost
///
/// @brief Sets the preference "lasthost"
///
/// @param[in] pLastHost - New value for preference "lasthost"
///
/// @see GetLastHost()
///
//-----------------------------------------------------------------------------
void AppPreferences::SetLastHost(const char * pLastHost)
{
   preferences.putString("lasthost", pLastHost);
}
