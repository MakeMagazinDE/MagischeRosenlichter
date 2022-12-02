//-----------------------------------------------------------------------------
//  effectChangingColor.h
/// @file
///
/// @brief Header for the module effectChangingColor.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTCHANGINGCOLOR_H
 #define __EFFECTCHANGINGCOLOR_H

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
//  Class CEffectChangingColor
///
/// @brief Management class for the LED-effect 'Changing color'
///
//-----------------------------------------------------------------------------
class CEffectChangingColor : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor of the class CEffectChangingColor
   CEffectChangingColor();
   // Sets the effect parameter and starts the effect
   virtual void Start(void);
   // Gets the name of the effect
   const char * GetName(void);
   // Sets the color to the effect
   void SetColorToEffect(uint32_t color);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Sets the effect parameter
   void SetEffectParameter(void);
   // Starts the LED-effect
   void StartEffect_ChangingColor(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect brightness type 1
   uint32_t m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t m_BrighnessType2Pct;
   /// Effect color value 1
   uint32_t m_Color1;
   /// Effect color value 2
   uint32_t m_Color2;
   /// Color view time
   uint32_t m_ColorViewTime;
   /// Flag: Is master controlled (true) or not (false)
   uint32_t m_IsMasterControlled;
};

#endif //__EFFECTCHANGINGCOLOR_H
