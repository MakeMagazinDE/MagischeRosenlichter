//-----------------------------------------------------------------------------
//  effectheart.h
/// @file
///
/// @brief Header for the module effectheart.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTHEART_H
 #define __EFFECTHEART_H

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
//  Class CEffectHeart
///
/// @brief Management class for the LED-effect 'Heart'
///
//-----------------------------------------------------------------------------
class CEffectHeart : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectHeart();
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
   void StartEffect_Heart(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect brightness type 1
   uint32_t m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t m_BrighnessType2Pct;
   /// Heart color
   uint32_t m_ColorHeart;
   /// Background color
   uint32_t m_ColorBackground;
};

#endif //__EFFECTHEART_H
