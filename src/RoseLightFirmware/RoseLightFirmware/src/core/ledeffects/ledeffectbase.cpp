//-----------------------------------------------------------------------------
//  ledeffectbase.cpp
/// @file
///
/// @brief Contains the implementation of the led effect base class
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
#include <Arduino.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "ledeffectbase.h"
#include "ledeffectmgr.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------

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
//  Method CLedEffectBase
///
/// @brief Constructor of the class CLedEffectBase
///
//-----------------------------------------------------------------------------
CLedEffectBase::CLedEffectBase(uint16_t EffectId)
   : m_IsRunning(false)
   , m_pLeds(nullptr)
   , m_NumOfLeds(0)
   , m_StopEffectSequence(StopEffectSequence::Unknown)
{
   LedEffectMgr.RegisterLedEffect(this);

   m_EffectId = EffectId;
}

//-----------------------------------------------------------------------------
//  Method Start
///
/// @brief Sets the effect parameter and starts the effect
///
//-----------------------------------------------------------------------------
void CLedEffectBase::Start(void)
{
}

//-----------------------------------------------------------------------------
//  Method Stop
///
/// @brief Stops the led effect
///
/// @param[in] stopEffectSequence - Stop sequence
///
//-----------------------------------------------------------------------------
void CLedEffectBase::Stop(StopEffectSequence stopEffectSequence)
{
   m_StopEffectSequence = stopEffectSequence;

   m_IsRunning = false;
}

//-----------------------------------------------------------------------------
//  Method SetLedHardware
///
/// @brief Sets the LED-hardware
///
/// @param[in] pLeds - LED-array
///
/// @param[in] numOfLeds - Number of LEDs
///
//-----------------------------------------------------------------------------
void CLedEffectBase::SetLedHardware(CRGB * pLeds, uint32_t numOfLeds)
{
   m_pLeds = pLeds;
   m_NumOfLeds = numOfLeds;
}

//-----------------------------------------------------------------------------
//  Method IsRunning
///
/// @brief Gets the flag: Led-effect is running (true) or not (false)
///
/// @return Flag: Led-effect is running (true) or not (false)
///
//-----------------------------------------------------------------------------
bool CLedEffectBase::IsRunning(void)
{
   return m_IsRunning;
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the LED-effect name
///
/// @return LED-effect name
///
//-----------------------------------------------------------------------------
const char * CLedEffectBase::GetName(void)
{
   return "No name";
}

//-----------------------------------------------------------------------------
//  Method GetEffectId
///
/// @brief Gets the LED-effect id
///
/// @return LED-effect id
///
//-----------------------------------------------------------------------------
uint16_t CLedEffectBase::GetEffectId(void)
{
   return m_EffectId;
}

//-----------------------------------------------------------------------------
//  Method SetColor
///
/// @brief Sets the led color for all LEDs
///
/// @param[in] color - Color value
///
/// @param[in] brightness - Brightness value
///
//-----------------------------------------------------------------------------
void CLedEffectBase::SetColor(CRGB color, uint8_t brightness)
{
   int iTmp;

   for (iTmp = 0; iTmp < m_NumOfLeds; iTmp++)
   {
      m_pLeds[iTmp] = color;
   }

   FastLED.setBrightness(brightness); 
   FastLED.show();
}

//-----------------------------------------------------------------------------
//  Method AddColor2FromTable
///
/// @brief Adds a color from table
///
/// @param[in] pColorTable - Pointer to color table
///
/// @param[in] sizeOfColorTable - Size of the color table
///
/// @param[in] startIndexColorTable - Start index in the color table
///
/// @param[in] startIndexTarget - Start index on the target
///
/// @param[in] numberOfValues - Number of values
///
/// @param[in] stepsize - Step size
///
//-----------------------------------------------------------------------------
void CLedEffectBase::AddColor2FromTable(uint32_t * pColorTable, uint32_t sizeOfColorTable, uint16_t startIndexColorTable, uint16_t startIndexTarget, uint16_t numberOfValues, float stepsize)
{
   uint16_t iValue = 0;
   float indexColorTable;   
   uint16_t indexTarget;

   indexColorTable = startIndexColorTable;
   indexTarget = startIndexTarget;

//   Serial.printf("Stepsize: %f\r\n", stepsize);

   while (iValue < numberOfValues)
   {
      m_pLeds[indexTarget] = pColorTable[(uint16_t)indexColorTable];

      indexTarget++;

      indexColorTable += stepsize;

      if (indexColorTable >= sizeOfColorTable)
      {
         indexColorTable -= sizeOfColorTable;
      }

      iValue++;
   }

  // Serial.printf("Ring index: %f\n\r", indexColorTable);
}

//-----------------------------------------------------------------------------
//  Method ShowColors
///
/// @brief Shows the colors in the LED-ring(s)
///
/// @param[in] brightness - Brightness value
///
//-----------------------------------------------------------------------------
void CLedEffectBase::ShowColors(uint8_t brightness)
{
   FastLED.setBrightness(brightness); 
   FastLED.show();
}

//-----------------------------------------------------------------------------
//  Method SetColor
///
/// @brief Set a new color value for the LED-ring(s)
///
/// @param[in] color - Color value
///
//-----------------------------------------------------------------------------
void CLedEffectBase::SetColor(CRGB color)
{
   int iTmp;

   for (iTmp = 0; iTmp < m_NumOfLeds; iTmp++)
   {
      m_pLeds[iTmp] = color;
   }

   FastLED.show();
}

//-----------------------------------------------------------------------------
//  Method ProcessStopSequence
///
/// @brief Processes the stop procedure of a LED-effect
///
//-----------------------------------------------------------------------------
void CLedEffectBase::ProcessStopSequence(void)
{
   switch (m_StopEffectSequence)
   {
      case StopEffectSequence::Unknown:
      default:
         break;

      case StopEffectSequence::HardSwitchOff:
         SetColor(CRGB::Black);
         break;

      case StopEffectSequence::SoftSwitchOff:
         uint8_t currentBrightness = FastLED.getBrightness();
   
         for (int j = currentBrightness; j >= 0; j--)
         {
            FastLED.setBrightness(j);
            FastLED.show();
            vTaskDelay(50);
         }
         break;
   }
}

//-----------------------------------------------------------------------------
//  Method SetColorToEffect
///
/// @brief Set color to effect
///
/// @param[in] color - Color value
///
//-----------------------------------------------------------------------------
void CLedEffectBase::SetColorToEffect(uint32_t color)
{
}
