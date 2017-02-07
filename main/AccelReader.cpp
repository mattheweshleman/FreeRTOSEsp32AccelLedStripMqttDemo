/*
 * AccelReader.cpp
 *
 *  Created on: Dec 30, 2016
 *      Author: eshlemanm

Copyright (c) <2017> <Matthew Eshleman - https://covemountainsoftware.com/>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */

#include "AccelReader.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <esp_log.h>
#include "adxl345/adxl345.h"
#include <cstdint>

#include "FreeRtosTickConvert.hpp"

using namespace CoveMountainSoftware;

static constexpr const char *TAG = "AccelReader";
static constexpr gpio_num_t SCL_PIN = GPIO_NUM_17;
static constexpr gpio_num_t SDA_PIN = GPIO_NUM_16;
static AccelReader::AccelEventHandler m_handler = nullptr;

static void Task(void *);

/**
 *
 */
void AccelReader::Init(AccelReader::AccelEventHandler eventHandler)
{
   m_handler = eventHandler;

   xTaskCreate(&Task, "accelReaderTask", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
}

/**
 * properly take into account the sign bit of
 * the 16 bit data registers
 */
static int16_t ConvertSign(int acc)
{
   return (acc <= INT16_MAX) ? acc : (acc - 65536);
}

/**
 * This Task demonstrates a simple polling style FreeRTOS task.
 *   Note the use of vTaskDelay() within a while(1) loop.
 */
void Task(void *)
{
   using namespace AccelReader;

   ESP_LOGI(TAG, "task started\n");

   initAcc(SCL_PIN, SDA_PIN);

   ESP_LOGI(TAG, "accelerometer started");

   int acc[3];
   while (1)
   {
      vTaskDelay(33_milliseconds);

      gpio_set_level(GPIO_NUM_5, 1); //crude cpu usage monitor

      bool ok = getAccelerometerData(acc);
      if (ok)
      {
         if ((abs(acc[0]) <= UINT16_MAX) && (abs(acc[1]) <= UINT16_MAX)
               && (abs(acc[2]) <= UINT16_MAX))
         {
            AccelData data;
            int16_t x, y, z;
            x = ConvertSign(acc[0]);
            y = ConvertSign(acc[1]);
            z = ConvertSign(acc[2]);
            static constexpr float CONVERT_RAW_TO_mg = 0.00390625 * 1000;
            data.x = (float) x * CONVERT_RAW_TO_mg;
            data.y = (float) y * CONVERT_RAW_TO_mg;
            data.z = (float) z * CONVERT_RAW_TO_mg;

            if (m_handler)
            {
               m_handler(data);
            }
         }
         else
         {
            ESP_LOGI(TAG, "tossed: %d %d %d", acc[0], acc[1], acc[2]);
         }
      }
   }
}
