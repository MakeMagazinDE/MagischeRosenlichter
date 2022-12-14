//-----------------------------------------------------------------------------
//  wlan.h
/// @file
///
/// @brief Header for the module wlan.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __WLAN_H
 #define __WLAN_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

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
//  Class CWLan
///
/// @brief WLan connection management class
///
//-----------------------------------------------------------------------------
class CWLan
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CWLan();
   // Established a connection to the wlan
   void ConnectToWLan(void);
   // Gets the information that a WLan connection is exstablished
   bool IsConnected(void);

private:
   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// System is connected (true) or not (false)
   bool m_SystemConnected;   
};

#endif //__WLAN_H
