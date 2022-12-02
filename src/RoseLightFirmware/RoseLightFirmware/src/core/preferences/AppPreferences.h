//-----------------------------------------------------------------------------
//  AppPreferences.h
/// @file
///
/// @brief Header for the module AppPreferences.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __APPPREFERENCES_H
 #define __APPPREFERENCES_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include "Preferences.h"

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Class AppPreferences
///
/// @brief Class to manage the access to the application preferences
///
//-----------------------------------------------------------------------------
class AppPreferences
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Begin method
   static void begin(void);
   // Gets the preference "type"
   static uint8_t GetType(void);
   // Gets the preference "ssid"
   static String GetSSID(void);
   // Gets the preference "password"
   static String GetPassword(void);
   // Gets the preference "name"
   static String GetName(void);
   // Gets the preference "lasthost"
   static String GetLastHost(void);
   // Gets the preference "flags"
   static uint8_t GetFlags(void);

   // Sets the preference "type"
   static void SetType(uint8_t type);
   // Sets the preference "ssid"
   static void SetSSID(char * pSSID);
   // Sets the preference "password"
   static void SetPassword(char * pPassword);
   // Sets the preference "name"
   static void SetName(char * pName);
   // Sets the preference "lasthost"
   static void SetLastHost(const char * pLastHost);
   // Sets the preference "flags"
   static void SetFlags(uint8_t type);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   AppPreferences();
};

#endif //__APPPREFERENCES_H
