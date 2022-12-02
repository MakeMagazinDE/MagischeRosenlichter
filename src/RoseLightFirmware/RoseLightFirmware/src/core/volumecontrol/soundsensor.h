//-----------------------------------------------------------------------------
//  soundsensor.h
/// @file
///
/// @brief Header for the module soundsensor.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __SOUNDSENSOR_H
 #define __SOUNDSENSOR_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "task.h"

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
//  Class CSoundSensor
///
/// @brief Management class for the sound sensor
///
//-----------------------------------------------------------------------------
class CSoundSensor : public CTask
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CSoundSensor(void);
   // Elaborate method of the class
   void Elaborate (void);
   // Activates the sound sensor
   void ActivateSoundSensor(void);
   // Deactivates the sound sensor
   void DeactivateSoundSensor(void);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Task function of the management class
   void TaskFct (void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Semaphore to starting a new job for the task
   SemaphoreHandle_t m_JobSema;
   /// Flag: Sound sensor is active (true) or not (false)
   bool m_SoundSensorActive;
};

#endif //__SOUNDSENSOR_H
