//-----------------------------------------------------------------------------
//  effectsnowglobe.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Snow globe'
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "effectsnowglobe.h"
#include "ledeffectmgr.h"
#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Snow globe'
#define EFFECT_SNOWGLOBE_NAME                         "Snow globe"
/// Parameter id/index for brightness device type 1 for effect 'Snow globe'
#define EFFECT_SNOWGLOBE_BRIGHTNESS_TYPE1             0
/// Parameter id/index for brightness device type 2 for effect 'Snow globe'
#define EFFECT_SNOWGLOBE_BRIGHTNESS_TYPE2             1
/// Parameter id/index for color snow flake for effect 'Snow globe'
#define EFFECT_SNOWGLOBE_COLOR_SNOWFLAKE              2
/// Parameter id/index for maximum number of snow flakes type 1 for effect 'Snow globe'
#define EFFECT_SNOWGLOBE_NUMBER_OF_SNOWFLAKES_TYPE1   3
/// Parameter id/index for maximum number of snow flakes type 2 for effect 'Snow globe'
#define EFFECT_SNOWGLOBE_NUMBER_OF_SNOWFLAKES_TYPE2   4

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
//  Method CEffectSnowGlobe
///
/// @brief Constructor of the class CEffectSnowGlobe
///
//-----------------------------------------------------------------------------
CEffectSnowGlobe::CEffectSnowGlobe()
   : CLedEffectBase((uint16_t)LedEffectId::SnowGlobe)
{
}

//-----------------------------------------------------------------------------
//  Method Start
///
/// @brief Sets the effect parameter and starts the effect
///
/// This method sets the effect parameter and starts the effect.
///
/// @see SetEffectParameter()
///
//-----------------------------------------------------------------------------
void CEffectSnowGlobe::Start(void)
{
   SetEffectParameter();

   StartEffect_SnowGlobe();
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectSnowGlobe::GetName(void)
{
   return EFFECT_SNOWGLOBE_NAME;
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectSnowGlobe::SetEffectParameter(void)
{
   uint32_t numOfSnowFlakesType1;
   uint32_t numOfSnowFlakesType2;

   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_SNOWGLOBE_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_SNOWGLOBE_BRIGHTNESS_TYPE2);
   m_ColorBase = LedEffectMgr.GetEffectParameter(EFFECT_SNOWGLOBE_COLOR_SNOWFLAKE);
   numOfSnowFlakesType1 = LedEffectMgr.GetEffectParameter(EFFECT_SNOWGLOBE_NUMBER_OF_SNOWFLAKES_TYPE1);
   numOfSnowFlakesType2 = LedEffectMgr.GetEffectParameter(EFFECT_SNOWGLOBE_NUMBER_OF_SNOWFLAKES_TYPE2);

   Serial.printf("Brightness type 1             %i %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2             %i %%\r\n",m_BrighnessType2Pct);
   Serial.printf("Color base                    0x%08x\r\n",m_ColorBase);
   Serial.printf("Number of snow flakes type 1  %i\r\n",numOfSnowFlakesType1);
   Serial.printf("Number of snow flakes type 2  %i\r\n",numOfSnowFlakesType2);

   if (2 == AppPreferences::GetType())
   {
      m_NumOfSnowFlakes = numOfSnowFlakesType2;
   }
   else
   {
      m_NumOfSnowFlakes = numOfSnowFlakesType1;
   }
}

//-----------------------------------------------------------------------------
//  Method StartEffect_SnowGlobe
///
/// @brief Starts the LED-effect
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectSnowGlobe::StartEffect_SnowGlobe(void)
{
   TickType_t xLastWakeTime;
   const TickType_t xFrequency = 50;
   uint32_t effectBrightness;
   uint32_t colors[241];
   uint8_t brightness;
   uint8_t brightnessPosN;
   uint8_t brightnessPosN1;
   uint32_t colorBase;

   InitSnowFlakes();

   if (2 == AppPreferences::GetType())
   {
      effectBrightness = m_BrighnessType2Pct;
   }
   else
   {
      effectBrightness = m_BrighnessType1Pct;
   }

   colorBase = CalculateColorBase(effectBrightness);

   m_IsRunning = true;

   xLastWakeTime = xTaskGetTickCount();

   while(m_IsRunning)
   {
      vTaskDelayUntil( &xLastWakeTime, xFrequency );

      CalculateSnowFlakes();

      for (int i = 0; i < 241; i++)
      {
         colors[i] = colorBase;
      }

      for (int i = 0; i < m_NumOfSnowFlakes; i++)
      {
         if (m_SnowFlakes[i].currentLifetime != 0)
         {
            brightness = (m_SnowFlakes[i].currentBrightness * 255) / 100;

            brightnessPosN = (uint8_t)((float)(m_SnowFlakes[i].currentPosition - (uint8_t)m_SnowFlakes[i].currentPosition) * brightness);

            brightnessPosN1 = (uint8_t)((1.0f - (float)(m_SnowFlakes[i].currentPosition - (uint8_t)m_SnowFlakes[i].currentPosition)) * brightness);

            if (((uint8_t)(m_SnowFlakes[i].currentPosition)-1) > 240)
               continue;

            colors[(uint8_t)(m_SnowFlakes[i].currentPosition)-1] = 0xFF000000 | brightnessPosN1 << 16 | brightnessPosN1 << 8 | brightnessPosN1;

            if (2 == AppPreferences::GetType())
            {
               if ((uint8_t)(m_SnowFlakes[i].currentPosition) >= 241)
               {
                  colors[0] = 0xFF000000 | brightnessPosN << 16 | brightnessPosN << 8 | brightnessPosN;
               }
               else
               {
                  colors[(uint8_t)(m_SnowFlakes[i].currentPosition)] = 0xFF000000 | brightnessPosN << 16 | brightnessPosN << 8 | brightnessPosN;
               }

               AddColor2FromTable(&colors[0], 241, (uint16_t)0, 0, 241, 1.0f);
            }
            else
            {
               if ((uint8_t)(m_SnowFlakes[i].currentPosition) >= 57)
               {
                  colors[0] = 0xFF000000 | brightnessPosN << 16 | brightnessPosN << 8 | brightnessPosN;
               }
               else
               {
                  colors[(uint8_t)(m_SnowFlakes[i].currentPosition)] = 0xFF000000 | brightnessPosN << 16 | brightnessPosN << 8 | brightnessPosN;
               }

               AddColor2FromTable(&colors[0], 57, (uint16_t)0, 0, 57, 1.0f);
            }
         }
      }

      ShowColors(100);
   }

   ProcessStopSequence();
}

//-----------------------------------------------------------------------------
//  Method InitSnowFlakes
///
/// @brief Initialize the snow flake effect data
///
//-----------------------------------------------------------------------------
void CEffectSnowGlobe::InitSnowFlakes(void)
{
   for (int i = 0; i < 80; i++)
   {
      m_SnowFlakes[i].positionAtStart = 0;
      m_SnowFlakes[i].brightnessAtStart = 0;
      m_SnowFlakes[i].brightnessAtEndOfLife = 0;
      m_SnowFlakes[i].lifetimeAtStart = 0;
      m_SnowFlakes[i].currentLifetime = 0;
      m_SnowFlakes[i].brightnessStepSize = 0;
      m_SnowFlakes[i].currentBrightness = 0;
      m_SnowFlakes[i].currentPosition = 0.0f;
      m_SnowFlakes[i].positionStepSize = 0.0f;
   }
}

//-----------------------------------------------------------------------------
//  Method CalculateColorBase
///
/// @brief Calculates the color base for the effect
///
/// @param[in] effectBrightness - Effect brighness
///
/// @return Color base value
///
//-----------------------------------------------------------------------------
uint32_t CEffectSnowGlobe::CalculateColorBase(uint8_t effectBrightness)
{
   uint8_t red;
   uint8_t green;
   uint8_t blue;
   uint32_t retVal;

   Serial.printf("Calculate color base for brightness %i %% ...\n\n", effectBrightness);

   red = (m_ColorBase >> 16) & 0xFF;
   green = (m_ColorBase >> 8) & 0xFF;
   blue = (m_ColorBase & 0x000000FF);

   red = (uint8_t)(red * ((float)effectBrightness/100));
   green = (uint8_t)(green * ((float)effectBrightness/100));
   blue = (uint8_t)(blue * ((float)effectBrightness/100));

   retVal = 0xFF000000;
   retVal |= red << 16;
   retVal |= green << 8;
   retVal |= blue;

   Serial.printf("Calculated base color: 0x%08x / 0x%08x\r\n", m_ColorBase, retVal);

   return retVal;
}

//-----------------------------------------------------------------------------
//  Method CalculateSnowFlakes
///
/// @brief Calculates the snow flakes
///
//-----------------------------------------------------------------------------
void CEffectSnowGlobe::CalculateSnowFlakes(void)
{
   for (int i = 0; i < m_NumOfSnowFlakes; i++)
   {
      if (m_SnowFlakes[i].currentLifetime > 0)
      {
         m_SnowFlakes[i].currentLifetime--;

         if (m_SnowFlakes[i].currentLifetime > 0)
         {
            if (m_SnowFlakes[i].currentBrightness > m_SnowFlakes[i].brightnessStepSize)
               m_SnowFlakes[i].currentBrightness -= m_SnowFlakes[i].brightnessStepSize;
            else
            {
               m_SnowFlakes[i].currentBrightness = 0;
               m_SnowFlakes[i].currentLifetime = 0;
            }

            m_SnowFlakes[i].currentPosition += m_SnowFlakes[i].positionStepSize;

            if (2 == AppPreferences::GetType())
            {
               if ((uint8_t)m_SnowFlakes[i].currentPosition > 241)
                  m_SnowFlakes[i].currentPosition = 1.0f;
            }
            else
            {
               if ((uint8_t)m_SnowFlakes[i].currentPosition > 57)
                  m_SnowFlakes[i].currentPosition = 1.0f;
            }
         }
         else
         {
            m_SnowFlakes[i].currentBrightness = 0;
          //  Serial.println("Snow flake end of life!");
         }
      }      
   }

   for (int i = 0; i < m_NumOfSnowFlakes; i++)
   {
      if (m_SnowFlakes[i].currentLifetime == 0)
      {
      //   Serial.println("Snow flake is born!");

         m_SnowFlakes[i].brightnessAtStart = RandomNumberGenerator(90, 100);;
         m_SnowFlakes[i].brightnessAtEndOfLife = 0;
         m_SnowFlakes[i].lifetimeAtStart = RandomNumberGenerator(120, 200);
         m_SnowFlakes[i].currentLifetime = m_SnowFlakes[i].lifetimeAtStart;
         m_SnowFlakes[i].currentBrightness = m_SnowFlakes[i].brightnessAtStart;
         m_SnowFlakes[i].positionAtStart = RandomStartPosition();
         m_SnowFlakes[i].currentPosition = m_SnowFlakes[i].positionAtStart;
         m_SnowFlakes[i].positionStepSize = RandomPositionStepsize();

        // Serial.printf("Pos: %f\r\n", m_SnowFlakes[i].positionStepSize);

         m_SnowFlakes[i].brightnessStepSize = (int8_t)((float)(m_SnowFlakes[i].brightnessAtStart - m_SnowFlakes[i].brightnessAtEndOfLife) / m_SnowFlakes[i].lifetimeAtStart);
         m_SnowFlakes[i].brightnessStepSize++;

         break;
      }      
   }
}

//-----------------------------------------------------------------------------
//  Method RandomPositionStepsize
///
/// @brief Get the stepsize for moving the snow flake
///
/// @return Stepsize for moving the snow flake
///
//-----------------------------------------------------------------------------
float CEffectSnowGlobe::RandomPositionStepsize(void)
{
   return (float)(RandomNumberGenerator(0,2000) - 1000) / 20000;
}

//-----------------------------------------------------------------------------
//  Method RandomStartPosition
///
/// @brief Get a new start position for a snow flake
///
/// @return Snow flake start position
///
//-----------------------------------------------------------------------------
uint8_t CEffectSnowGlobe::RandomStartPosition(void)
{
   uint8_t tmpPosition;
   bool fPosInvalid;

   for(;;)
   {
      if (2 == AppPreferences::GetType())
      {
         tmpPosition = RandomNumberGenerator(1, 241);
      }
      else
      {
         tmpPosition = RandomNumberGenerator(1, 57);
      }

      fPosInvalid = false;

      for (int i = 0; i < m_NumOfSnowFlakes; i++)
      {
         if (m_SnowFlakes[i].positionAtStart == tmpPosition)
         {
            fPosInvalid = true;
            break;
         }
      }

      if (!fPosInvalid)
         return tmpPosition;
   }

   return 1;
}

//-----------------------------------------------------------------------------
//  Method RandomNumberGenerator
///
/// @brief Generates a random number
///
/// @param[in] nMin - Minimum value
///
/// @param[in] nMax - Maximum value
///
/// @return Random number
///
//-----------------------------------------------------------------------------
int CEffectSnowGlobe::RandomNumberGenerator(const int nMin, const int nMax)
{
   return (rand()%(nMax-nMin) + nMin);
}