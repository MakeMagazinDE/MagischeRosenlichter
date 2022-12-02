//-----------------------------------------------------------------------------
//  task.cpp
/// @file
///
/// @brief Simple task class
///
/// $Date: 2022-08-26 09:53:26 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 166 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <Arduino.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "task.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Default stack size
#define DEFAULT_STACK_SIZE 8192
/// Default name for a task
#define DEFAULT_TASK_NAME "tDefName"
/// Default task priority
#define DEFAULT_TASK_PRIORITY 2

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------

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
//  Function xinitialize 
///
/// @brief Task function
///
/// This is the entry point for the task.
///
/// @param[in] *tmp - Task parameter
///
/// @return Unused value
///
//-----------------------------------------------------------------------------
long xinitialize (void *tmp)
{
   CTask *thisTask;

   thisTask = (CTask *) tmp;

   // Call Task function
   thisTask->TaskFct();

   return 0;
}

//-----------------------------------------------------------------------------
//  Method CTask
///
/// @brief Constructor of the class CTask
///
/// @see Startup()
///
//-----------------------------------------------------------------------------
CTask::CTask()
   : m_StackSize(DEFAULT_STACK_SIZE)
   , m_UserData(0)
   , m_Priority(DEFAULT_TASK_PRIORITY)
   , m_TaskName(DEFAULT_TASK_NAME)
   , m_fRunning(false)
{
}

//-----------------------------------------------------------------------------
//  Method Startup
///
/// @brief Startup method for a task
///
/// This method creates the task instance.
///
/// @param[in] userData - User data for the task
///
/// @return Creation status, 0 in normal case
///
/// @see CTask()
///
//-----------------------------------------------------------------------------
int CTask::Startup(uint32_t userData)
{
  if (m_fRunning == true)
      return -2;

   m_UserData = userData;

   m_fRunning = true;

   m_xHandle = NULL;

   if (pdPASS != xTaskCreatePinnedToCore(
         (TaskFunction_t) xinitialize
      ,  m_TaskName 
      ,  m_StackSize
      ,  (void *) this
      ,  m_Priority
      ,  &m_xHandle 
      ,  ARDUINO_RUNNING_CORE))
      return -1;

   return 0;
}

//-----------------------------------------------------------------------------
//  Method SetStackSize
///
/// @brief Sets the stack size for the task
///
/// @param[in] stackSize - Task stack size in bytes
///
/// @return Status, 0 in normal case
///
/// @see Startup()
///
//-----------------------------------------------------------------------------
int CTask::SetStackSize (uint32_t stackSize)
{
   if (stackSize < 1)
      return -1;

   m_StackSize = stackSize;

   return 0;
}

//-----------------------------------------------------------------------------
//  Method SetPriority
///
/// @brief Sets the task priority
///
/// @param[in] priority - Task priority
///
/// @return Status, 0 in normal case
///
//-----------------------------------------------------------------------------
int CTask::SetPriority (uint8_t priority)
{
   m_Priority = priority;

   return 0;
}

//-----------------------------------------------------------------------------
//  Method SetName
///
/// @brief Sets the name for the task
///
/// @param[in] taskName - Task name
///
/// @return Status, 0 in normal case
///
/// @see SetPriority()
///
//-----------------------------------------------------------------------------
int CTask::SetName(const char * taskName)
{
   strcpy(m_TaskName, taskName);

   return 0;
}

//-----------------------------------------------------------------------------
//  Method Sleep
///
/// @brief Sleep method
///
/// @param[in] sleepTimeMs - Sleep time in ms
///
//-----------------------------------------------------------------------------
void CTask::Sleep(int sleepTimeMs)
{
   vTaskDelay(sleepTimeMs);
}

//-----------------------------------------------------------------------------
//  Method NotifyGiveFromIsr
///
/// @brief Give a notification from an isr-routine
///
/// @param[in] pxHigherPriorityTaskWoken - Flag: Higher priority task woken
///
//-----------------------------------------------------------------------------
void CTask::NotifyGiveFromIsr(BaseType_t * pxHigherPriorityTaskWoken)
{
   vTaskNotifyGiveFromISR(m_xHandle, pxHigherPriorityTaskWoken);
}

