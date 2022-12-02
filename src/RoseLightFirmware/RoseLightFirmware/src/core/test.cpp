#if 0
#include <Arduino.h>
#include "FastLED.h"
#include "test.h"


//#define NUM_OF_LEDS 57
#define NUM_OF_LEDS 101

#define LED_PIN 27

CRGB leds[NUM_OF_LEDS];

void ProcessStopSequence(void);

void SetColor(CRGB color);
void SetColor2(CRGB color, uint8_t brightness);
int GetIndex(int lower, int upper);
void SetColor3(CRGB color, int Index);
void SetColor4(int Index0, int Index1, int Index2, int Index3, int Index4) ;
typedef struct tagColorValue
{
   CRGB color;
   uint8_t brightness;
} ColorValue;

ColorValue cValue[] = { { CRGB::Red, 10 },
                        { CRGB::Red, 20 },
                        { CRGB::Red, 30 },
                        { CRGB::Red, 40 },
                        { CRGB::Red, 50 },
                        { CRGB::Red, 40 },
                        { CRGB::Red, 30 },
                        { CRGB::Red, 20 },
                        { CRGB::Red, 10 },
                        { CRGB::Red, 0 },
                        { CRGB::Blue, 10 },
                        { CRGB::Blue, 20 },
                        { CRGB::Blue, 30 },
                        { CRGB::Blue, 40 },
                        { CRGB::Blue, 50 },
                        { CRGB::Blue, 40 },
                        { CRGB::Blue, 30 },
                        { CRGB::Blue, 20 },
                        { CRGB::Blue, 10 },
                        { CRGB::Blue, 0 },
                        { CRGB::Green, 10 },
                        { CRGB::Green, 20 },
                        { CRGB::Green, 30 },
                        { CRGB::Green, 40 },
                        { CRGB::Green, 50 },
                        { CRGB::Green, 40 },
                        { CRGB::Green, 30 },
                        { CRGB::Green, 20 },
                        { CRGB::Green, 10 },
                        { CRGB::Green, 0 },
                        { CRGB::Yellow, 10 },
                        { CRGB::Yellow, 20 },
                        { CRGB::Yellow, 30 },
                        { CRGB::Yellow, 40 },
                        { CRGB::Yellow, 50 },
                        { CRGB::Yellow, 40 },
                        { CRGB::Yellow, 30 },
                        { CRGB::Yellow, 20 },
                        { CRGB::Yellow, 10 },
                        { CRGB::Yellow, 0 } };

typedef struct tagColorValue2
{
   CRGB color;
   int index;
   uint8_t onShow;
} ColorValue2;

ColorValue2 cValue2[5] = { { CRGB::Black, -1, 0 },
                           { CRGB::Black, -1, 0 },
                           { CRGB::Black, -1, 0 },
                           { CRGB::Black, -1, 0 },
                           { CRGB::Black, -1, 0 }};

void BuildPixelTable(void);

SemaphoreHandle_t jobSema;

int m_JobId;

bool m_JobIsRunning;

int m_StopEffectSequence;


void StartEffect_MultiColor(void);
void StartEffect_SnowGlobe1(void);
void StartEffect_SingleColor(void);

void StartJob(int jobId)
{
   m_JobId = jobId;

   xSemaphoreGive(jobSema);
}

void StopJob(int StopEffectSequence)
{
   m_StopEffectSequence = StopEffectSequence;

   m_JobIsRunning = false;
}

void test_task_fct( void *pvParameters )
{
   Serial.printf("Ansteuerung WS2812\r\n");

   FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_OF_LEDS);
   FastLED.setBrightness(50); 

   vSemaphoreCreateBinary(jobSema);

   for(;;)
   {
      xSemaphoreTake(jobSema, portMAX_DELAY);

      switch (m_JobId)
      {
         case EFFECT_MULTICOLOR:
            StartEffect_MultiColor();
            break;

         case EFFECT_SINGLECOLOR:
            StartEffect_SingleColor();
            break;

         case EFFECT_SNOW_GLOBE_1:
            StartEffect_SnowGlobe1();
            break;
      }
   }


/////////////////////////////////////

   //   int iStep = 0; //

 /*  for(;;)
   {
      SetColor(CRGB::Red);
      vTaskDelay(1000);
      SetColor(CRGB::Blue);
      vTaskDelay(1000);
      SetColor(CRGB::Green);
      vTaskDelay(1000);
      SetColor(CRGB::Yellow);
      vTaskDelay(1000);
      SetColor(CRGB::Black);
      vTaskDelay(1000);
   }*/

   /*for(;;)
   {
      SetColor2(cValue[iStep].color, cValue[iStep].brightness);
      vTaskDelay(100);

      iStep++;

      if (iStep == 40)
      {
         iStep = 0;
      }
   }*/

/*   for(;;)
   {
      switch (iStep)
      {
         case 0: color = CRGB::Red; break;
         case 1: color = CRGB::Blue; break;
         case 2: color = CRGB::Green; break;
         case 3: color = CRGB::Magenta; break;
      }

      int i;

      for (i = 0; i < 50; i++)
      {
         SetColor2(color, i);
         vTaskDelay(50);
      }

      for (i = 50; i > 0; i--)
      {
         SetColor2(color, i);
         vTaskDelay(50);
      }

      iStep++;

      if (iStep == 4)
      {
         iStep = 0;
      }
   } */

/*
// Use current time as 
    // seed for random generator
    srand(time(0));

   for (;;)
   {
      SetColor3(CRGB::White, GetIndex(0,45));
      vTaskDelay(100);
   }*/

// Use current time as 
    // seed for random generator

// Pulsierend

 /*   srand(time(0));

    for (;;)
   {
      BuildPixelTable();
      SetColor4(cValue2[0].index, cValue2[1].index, cValue2[2].index, cValue2[3].index, cValue2[4].index);
      vTaskDelay(100);
   }
   */
}

void StartEffect_SnowGlobe1(void)
{
   srand(time(0));

   FastLED.setBrightness(50);

   while(m_JobIsRunning)
   {
      BuildPixelTable();
      SetColor4(cValue2[0].index, cValue2[1].index, cValue2[2].index, cValue2[3].index, cValue2[4].index);
      vTaskDelay(100);
   }

   ProcessStopSequence();
}

void StartEffect_SingleColor(void)
{
   SetColor2(CRGB::White, 100);

/*   CRGB color;
   int iStep = 0; 
   int i;

   m_JobIsRunning = true;

   while(m_JobIsRunning)
   {
      switch (iStep)
      {
         case 0: color = CRGB::Red; break;
         case 1: color = CRGB::Blue; break;
         case 2: color = CRGB::Green; break;
         case 3: color = CRGB::Magenta; break;
      }

      for (i = 0; i < 50; i++)
      {
         if (!m_JobIsRunning)
            break;

         SetColor2(color, i);
         vTaskDelay(50);
      }

      for (i = 50; i > 0; i--)
      {
         if (!m_JobIsRunning)
            break;

         SetColor2(color, i);
         vTaskDelay(50);
      }

      iStep++;

      if (iStep == 4)
      {
         iStep = 0;
      }
   }

   ProcessStopSequence();*/
}

void StartEffect_MultiColor(void)
{
   CRGB color;
   int iStep = 0; 
   int i;

   m_JobIsRunning = true;

   while(m_JobIsRunning)
   {
      switch (iStep)
      {
         case 0: color = CRGB::Red; break;
         case 1: color = CRGB::Blue; break;
         case 2: color = CRGB::Green; break;
         case 3: color = CRGB::Magenta; break;
      }

      for (i = 0; i < 50; i++)
      {
         if (!m_JobIsRunning)
            break;

         SetColor2(color, i);
         vTaskDelay(50);
      }

      for (i = 50; i > 0; i--)
      {
         if (!m_JobIsRunning)
            break;

         SetColor2(color, i);
         vTaskDelay(50);
      }

      iStep++;

      if (iStep == 4)
      {
         iStep = 0;
      }
   }

   ProcessStopSequence();
}

void ProcessStopSequence(void)
{
   switch (m_StopEffectSequence)
   {
      case SS_HARD_SWITCH_OFF:
         SetColor(CRGB::Black);
         break;

      case SS_SOFT_SWITCH_OFF:
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

void BuildPixelTable(void)
{
   int i;

   for (i=0; i<5; i++)
   {
      if ((cValue2[i].index == -1) || (cValue2[i].onShow == 5))
      {
         cValue2[i].color = CRGB::White;
         cValue2[i].index = GetIndex(0,45);
         cValue2[i].onShow = 0;
         break;
      }
   }

   for (i=0; i<5; i++)
   {
      if (cValue2[i].index != -1)
      {
         cValue2[i].onShow++;
      }
   }
}

void SetColor(CRGB color)
{
   int iTmp;

   for (iTmp = 0; iTmp < NUM_OF_LEDS; iTmp++)
   {
      leds[iTmp] = color;
   }

   FastLED.show();
}

void SetColor2(CRGB color, uint8_t brightness)
{
   int iTmp;

   for (iTmp = 0; iTmp < NUM_OF_LEDS; iTmp++)
   {
      leds[iTmp] = color;
   }

   FastLED.setBrightness(brightness); 
   FastLED.show();
}

void SetColor3(CRGB color, int Index)
{
   int iTmp;

   for (iTmp = 0; iTmp < NUM_OF_LEDS; iTmp++)
   {
      if (iTmp == Index)
      {
         leds[iTmp] = color;
      }
      else
      {
         leds[iTmp] = CRGB::Black;
      }
   }

   FastLED.show();
}

void SetColor4(int Index0, int Index1, int Index2, int Index3, int Index4) 
{
   int iTmp;

   for (iTmp = 0; iTmp < NUM_OF_LEDS; iTmp++)
   {
      if (iTmp == Index0)
      {
         leds[iTmp] = CRGB::LightBlue;
      }
      else if (iTmp == Index1)
      {
         leds[iTmp] = CRGB::LightBlue;
      }
      else if (iTmp == Index2)
      {
         leds[iTmp] = CRGB::LightBlue;
      }
      else if (iTmp == Index3)
      {
         leds[iTmp] = CRGB::LightBlue;
      }
      else if (iTmp == Index4)
      {
         leds[iTmp] = CRGB::LightBlue;
      }
      else
      {
         float test2 = ((float)GetIndex(5,10)) / 100;
         leds[iTmp].raw[0] = 255 * test2;
         leds[iTmp].raw[1] = 255 * test2;
         leds[iTmp].raw[2] = 255 * test2;
      }
   }

   FastLED.show();
}

int GetIndex(int lower, int upper)
{
   return (rand() % (upper - lower + 1)) + lower;
}
#endif