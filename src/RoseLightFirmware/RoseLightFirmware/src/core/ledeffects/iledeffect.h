//-----------------------------------------------------------------------------
//  iledeffect.h
/// @file
///
/// @brief Contains the interface for all LED-effects
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __ILEDEFFECT_H
 #define __ILEDEFFECT_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
/// Fast led internal
#define FASTLED_INTERNAL

#include "FastLED.h"

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "ledeffectdefs.h"

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
//  Class ILedEffect
///
/// @brief Interface class for all LED-effects
///
//-----------------------------------------------------------------------------
class ILedEffect
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   /// Sets the effect parameter and starts the effect
   virtual void Start(void) = 0;
   /// Stops the led effect
   virtual void Stop(StopEffectSequence stopEffectSequence) = 0;
   /// Gets the LED-effect name
   virtual const char * GetName(void) = 0;
   /// Gets the LED-effect id
   virtual uint16_t GetEffectId(void) = 0;
   /// Gets the flag: Led-effect is running (true) or not (false)
   virtual bool IsRunning(void) = 0;
   /// Sets the LED-hardware
   virtual void SetLedHardware(CRGB * pLeds, uint32_t numOfLeds);
};

#endif //__ILEDEFFECT_H
