//-----------------------------------------------------------------------------
//  effectfire.h
/// @file
///
/// @brief Header for the module effectfire.cpp
///
/// @see CLedEffectBase
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTFIRE_H
 #define __EFFECTFIRE_H

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
//  Class CEffectFire
///
/// @brief Class for manage the led-effect 'fire'
///
/// @see CLedEffectBase
///
//-----------------------------------------------------------------------------
class CEffectFire : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectFire();
   // Starts the effect
   virtual void Start(void);
   // Gets the name of the effect
   const char * GetName(void);
private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Starts the effect
   void StartEffect_Fire(void);
   // Sets the effect parameter
   void SetEffectParameter(void);
   // Draws the marquee comparsion
   void DrawMarqueeComparison(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Brightness for rose light devices type 1
   uint32_t m_BrighnessType1Pct;
   /// Brightness for rose light devices type 2
   uint32_t m_BrighnessType2Pct;
};

#endif //__EFFECTFIRE_H
