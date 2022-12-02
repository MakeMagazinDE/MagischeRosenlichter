//-----------------------------------------------------------------------------
//  effectsnowflake.h
/// @file
///
/// @brief Header for the module effectsnowflake.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTSNOWFLAKE_H
 #define __EFFECTSNOWFLAKE_H

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

//-----------------------------------------------------------------------------
//  Class CEffectSnowFlake
///
/// @brief Management class for the LED-effect 'Snow flake'
///
//-----------------------------------------------------------------------------
class CEffectSnowFlake : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectSnowFlake();
   // Sets the effect parameter and starts the effect
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
   void StartEffect_SnowFlake(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect brightness type 1
   uint32_t m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t m_BrighnessType2Pct;
   /// Color for the snow flake
   uint32_t m_ColorSnowFlake;
   /// Color for the star
   uint32_t m_ColorStar;
   /// Background color
   uint32_t m_ColorBackground;
};

#endif //__EFFECTSNOWFLAKE_H
