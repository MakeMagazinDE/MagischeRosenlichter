//-----------------------------------------------------------------------------
//  ledeffectbase.h
/// @file
///
/// @brief Header for the module ledeffectbase.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __LEDEFFECTBASE_H
 #define __LEDEFFECTBASE_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "iledeffect.h"
#include "ledeffectdefs.h"

/// Fast LED internal 
#define FASTLED_INTERNAL

#include "FastLED.h"

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
//  Class CLedEffectBase
///
/// @brief LED-effect base class
///
//-----------------------------------------------------------------------------
class CLedEffectBase : public ILedEffect
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CLedEffectBase(uint16_t EffectId);
   // Sets the effect parameter and starts the effect
   virtual void Start(void);
   // Stops the led effect
   virtual void Stop(StopEffectSequence stopEffectSequence);
   // Gets the LED-effect name
   virtual const char * GetName(void);
   // Gets the LED-effect id
   virtual uint16_t GetEffectId(void);
   // Gets the flag: Led-effect is running (true) or not (false)
   virtual bool IsRunning(void);
   // Sets the LED-hardware
   virtual void SetLedHardware(CRGB * pLeds, uint32_t numOfLeds);
   // Set color to effect
   virtual void SetColorToEffect(uint32_t color);

protected:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Sets the led color for all LEDs
   void SetColor(CRGB color, uint8_t brightness);
   // Set a new color value for the LED-ring(s)
   void SetColor(CRGB color);
   // Processes the stop procedure of a LED-effect
   void ProcessStopSequence(void);
   // Adds a color from table
   void AddColor2FromTable(uint32_t* pColorTable, uint32_t sizeOfColorTable, uint16_t startIndexColorTable, uint16_t startIndexTarget, uint16_t numberOfValues, float stepsize);
   // Shows the colors in the LED-ring(s)
   void ShowColors(uint8_t brightness);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Flag: Effect is running (true) or not (false)
   bool m_IsRunning;

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   /// Constructor
   CLedEffectBase() {}
   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect id
   uint16_t m_EffectId;
   /// Pointer to the LED-hardware
   CRGB * m_pLeds;
   /// Number of LEDs
   uint32_t m_NumOfLeds;
   /// Sequence for the stop process of an LED-effect
   StopEffectSequence m_StopEffectSequence;
};

#endif //__LEDEFFECTBASE_H
