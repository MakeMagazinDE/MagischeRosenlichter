//-----------------------------------------------------------------------------
//  effectrainbowbase.cpp
/// @file
///
/// @brief Contains the base class implementation for rainbow LED-effects
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <stdint.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "effectrainbowbase.h"
#include "ledeffectmgr.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Parameter id/index for 'Is master flag' for a rainbow effect
#define EFFECT_RAINBOW_BASE_ISMASTERFLAG     0
/// Parameter id/index for base frequency master for a rainbow effect
#define EFFECT_RAINBOW_BASE_FREQUENCY_MASTER 1
/// Parameter id/index for base frequency slave for a rainbow effect
#define EFFECT_RAINBOW_BASE_FREQUENCY_SLAVE  2
/// Parameter id/index for brightness device type 1 for rainbow effect
#define EFFECT_RAINBOW_BASE_BRIGHTNESS_TYPE1 3
/// Parameter id/index for brightness device type 2 for rainbow effect
#define EFFECT_RAINBOW_BASE_BRIGHTNESS_TYPE2 4

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// Instance of the CLedEffectMgr class
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
//  Method CEffectRainbowBase
///
/// @brief Constructor of the class CEffectRainbowBase
///
//-----------------------------------------------------------------------------
CEffectRainbowBase::CEffectRainbowBase(uint16_t EffectId)
   : CLedEffectBase(EffectId)
{  
   BuildColorTable();
}

//-----------------------------------------------------------------------------
//  Method BuildColorTable
///
/// @brief Builds the color table for the rainbow
///
//-----------------------------------------------------------------------------
void CEffectRainbowBase::BuildColorTable(void)
{
   float stepSize = (256 / (((float)SIZEOFCOLORTABLE) / 4));

   int32_t SegmentSize = SIZEOFCOLORTABLE / 4;

   float Green = 0.0f;
   float Red = 255.0f;
   float Blue = 0.0f; 

   for (int i = 0; i < SIZEOFCOLORTABLE; i++)
   {
    if (i > 0 && i <= (1 * SegmentSize))
    {
      Green += stepSize;

      if (Green > 0xFF)
        Green = 0xFF;
    }
    else if (i > (1 * SegmentSize) && i <= (2 * SegmentSize))
    {
      Red -= stepSize;
      Blue += stepSize;

      if (Blue > 0xFF)
        Blue = 0xFF;

      if (Red < 0x00)
        Red = 0x00;
    }
    else if (i > (2 * SegmentSize) && i <= (3 * SegmentSize))
    {
      Green -= stepSize;

      if (Green < 0x00)
        Green = 0x00;
    }
    else if (i > (3 * SegmentSize) && i < (4 * SegmentSize))
    {
      Red += stepSize;
      Blue -= stepSize;

      if (Red > 0xFF)
        Red = 0xFF;

      if (Blue < 0x00)
        Blue = 0x00;
    }

    RingColors[i] = 0;
    RingColors[i] = (uint8_t)Green << 16 | (uint8_t)Red << 8 | (uint8_t)Blue;
  }
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
//-----------------------------------------------------------------------------
void CEffectRainbowBase::SetEffectParameter(void)
{
   m_IsMaster = LedEffectMgr.GetEffectParameter(EFFECT_RAINBOW_BASE_ISMASTERFLAG);
   m_FrequencyMaster = (LedEffectFequency) LedEffectMgr.GetEffectParameter(EFFECT_RAINBOW_BASE_FREQUENCY_MASTER);
   m_FrequencySlave = (LedEffectFequency) LedEffectMgr.GetEffectParameter(EFFECT_RAINBOW_BASE_FREQUENCY_SLAVE);

   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_RAINBOW_BASE_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_RAINBOW_BASE_BRIGHTNESS_TYPE2);

   Serial.printf("Is master         %i\r\n",m_IsMaster);
   Serial.printf("Frequency type 1  %i\r\n",m_FrequencySlave);
   Serial.printf("Frequency type 2  %i\r\n",m_FrequencyMaster);

   Serial.printf("Brightness type 1 %i %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2 %i %%\r\n",m_BrighnessType2Pct);
}
