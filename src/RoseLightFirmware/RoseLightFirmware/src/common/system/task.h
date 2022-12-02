//-----------------------------------------------------------------------------
//  task.h
/// @file
///
/// @brief Header for the file task.cpp
///
/// $Date: 2022-08-26 09:53:26 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 166 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __TASK_H
 #define __TASK_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <Arduino.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------
/// Maximum length for a task name
#define TASK_NAME_LEN   16

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Class CTask
///
/// @brief Base task class
///
//-----------------------------------------------------------------------------
class CTask
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CTask();
   // Startup method for a task
   int Startup(uint32_t userData);
   // Sets the stack size for the task
   int SetStackSize (uint32_t stackSize);
   // Sets the task priority
   int SetPriority (uint8_t priority);
   // Sets the name for the task
   int SetName(const char * taskName);
   // Sleep method
   static void Sleep(int sleepTimeMs);
   // Give a notification from an isr-routine
   void NotifyGiveFromIsr(BaseType_t * pxHigherPriorityTaskWoken);
private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   /// Pure virtual entry method of the task
   virtual void TaskFct (void) = 0;
   // Task function
   friend long xinitialize (void *tmp);
   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Stack size of the task
   uint32_t m_StackSize;
   /// User data for the task
   uint32_t m_UserData;
   /// Priority of the task
   uint32_t m_Priority;
   /// Task name
   char     m_TaskName[TASK_NAME_LEN + 1];
   /// Flag: Task is runnung (true) or not (false)
   bool     m_fRunning;
   /// Task handle
   TaskHandle_t m_xHandle;
};

#endif //__TASK_H
