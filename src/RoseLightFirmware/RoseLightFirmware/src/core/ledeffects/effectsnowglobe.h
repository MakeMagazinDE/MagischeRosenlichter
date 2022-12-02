//-----------------------------------------------------------------------------
//  effectsnowglobe.h
/// @file
///
/// @brief Header for the module effectsnowglobe.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTSNOWGLOBE_H
 #define __EFFECTSNOWGLOBE_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "ledeffectbase.h"

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------
/// Processing snow flake behaviour data 
typedef struct tagSnowFlake 
{
   uint16_t positionAtStart;           ///< Snow flake position at start
   uint8_t  brightnessAtStart;         ///< Snow flake brightness at start
   uint8_t  brightnessAtEndOfLife;     ///< Snow flake brightness at end of life
   uint8_t  lifetimeAtStart;           ///< Snow flake lifetime at start
   uint8_t  currentLifetime;           ///< Current lifetime
   uint8_t  currentBrightness;         ///< Current brightness
   int8_t   brightnessStepSize;        ///< Current brightness step size
   float    currentPosition;           ///< Current position
   float    positionStepSize;          ///< Current position step size
} SnowFlake;

//-----------------------------------------------------------------------------
//  Class CEffectSnowGlobe
///
/// @brief Management class for the LED-effect 'Snow globe'
///
//-----------------------------------------------------------------------------
class CEffectSnowGlobe : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectSnowGlobe();
   // Gets the name of the effect
   virtual void Start(void);
   // Gets the name of the effect
   const char * GetName(void);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Sets the effect parameter
   void SetEffectParameter(void);
   // Starts the LED-effect
   void StartEffect_SnowGlobe(void);
   // Calculates the color base for the effect
   uint32_t CalculateColorBase(uint8_t effectBrightness);
   // Generates a random number
   int RandomNumberGenerator(const int nMin, const int nMax);
   // Initialize the snow flake effect data
   void InitSnowFlakes(void);
   // Calculates the snow flakes
   void CalculateSnowFlakes(void);
   // Get a new start position for a snow flake
   uint8_t RandomStartPosition(void);
   // Get the stepsize for moving the snow flake
   float RandomPositionStepsize(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect brightness type 1
   uint32_t m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t m_BrighnessType2Pct;
   /// Base color
   uint32_t m_ColorBase;
   /// Snow flakes data
   SnowFlake m_SnowFlakes[80];
   /// Number of snow flakes
   uint16_t m_NumOfSnowFlakes;
};

#endif //__EFFECTSNOWGLOBE_H
