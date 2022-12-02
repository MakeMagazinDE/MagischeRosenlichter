//-----------------------------------------------------------------------------
//  hardware.h
/// @file
///
/// @brief Header for the module hardware.cpp
///
/// $Date: 2022-08-26 09:53:26 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 166 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __HARDWARE_H
 #define __HARDWARE_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <stdint.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------
/// Type of the rose light device
enum class RoseLightType
{
   Unknown = 0,         ///< Unknown device type
   Type1,               ///< Rose light device type 1
   Type2,               ///< Rose light device type 2
   Type3                ///< Rose light device type 3
};

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Class CHardware
///
/// @brief Class for access to the hardware information
///
//-----------------------------------------------------------------------------
class CHardware
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CHardware(void);
   // Gets the hardware version of the device
   uint8_t GetHardwareVersion(void);
   // Gets the type of the device
   RoseLightType GetType(void);
};

#endif //__HARDWARE_H
