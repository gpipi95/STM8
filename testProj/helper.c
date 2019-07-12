#include "helper.h"
#include "../DS18B20/DS18B20.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s_gpio.h"
#include "../TM1638/TM1638.h"
#include "../core/PrintfUtility.h"
#include "../core/atomic.h"

#define NUM_OF_TASK 8
#define KEYBOARD_TASK 1
#define LED_TASK 6
#define TEMPERATURE_TASK 7

unsigned int testData = 0;
typedef struct {
    unsigned char reloadPeriod; // time length of interrupt period
    unsigned char counter;      // timer interrupt couter
    unsigned char isRun;        // is time to run
} Task_t;

Task_t TaskData[NUM_OF_TASK] = {
    // clang-format off
    // 4.8ms is the base period, see TIM2Init for detail
    { 2  , 2  , 0 }, // 9.6ms for key board read
    { 5  , 5  , 0 }, // 24.0ms for ...
    { 10 , 10 , 0 }, // 48.0ms for ...
    { 20 , 20 , 0 }, // 96.0ms for ...
    { 30 , 30 , 0 }, // 144.0ms for ...
    { 50 , 50 , 0 }, // 240.0ms for ...
    { 208, 208, 0 }, // 998.4ms for ...
    { 208, 208, 0 }  // 998.4ms for temperature read and display
    // clang-format on
};
void TimeInterruptWork(void)
{
    unsigned char i;
    for (i = 0; i < NUM_OF_TASK; ++i) {
        TaskData[i].counter--;
        if (!TaskData[i].counter) {
            TaskData[i].counter = TaskData[i].reloadPeriod;
            ATOMIC_BEGIN();
            TaskData[i].isRun = 1;
            ATOMIC_END();
        }
    }
}
unsigned char TaskCanRun(unsigned char taskNum)
{
    unsigned char result = 0;
    if (taskNum < NUM_OF_TASK) {
        ATOMIC_BEGIN();
        result = TaskData[taskNum].isRun;
        ATOMIC_END();
    }
    return result;
}
void TaskRunClear(unsigned char taskNum)
{
    if (taskNum < NUM_OF_TASK) {
        ATOMIC_BEGIN();
        TaskData[taskNum].isRun = 0;
        ATOMIC_END();
    }
}

void GetDisplayTempTask(void)
{
    if (TaskCanRun(TEMPERATURE_TASK)) {
        static unsigned char flip = 2;
        short int            temperature;
        unsigned char        temp;
        flip--;
        if (flip != 0) {
            if (!DS18B20ConvertTemp()) {
                flip = 2; // convert faild reload counter
            }
        } else {
            flip = 2;
            if (DS18B20GetTemp(&temperature)) {
                if (temperature < 0) {
                    temp = 38; // display minus flag
                } else if (CAST_UC((temperature / 1000) % 10) == 0) {
                    temp = 40; // turn off led
                } else {
                    temp = CAST_UC((temperature / 1000) % 10);
                }
                TM1638OneSymbolDisplay(7, temp);
                if (temp == 40) {
                    if (CAST_UC((temperature / 100) % 10) != 0) {
                        temp = CAST_UC((temperature / 100) % 10);
                    }
                }
                TM1638OneSymbolDisplay(6, temp);
                TM1638OneSymbolDisplay(5, CAST_UC((temperature / 10) % 10) + 16);
                TM1638OneSymbolDisplay(4, CAST_UC(temperature % 10));
            }
        }
        TaskRunClear(TEMPERATURE_TASK);
    }
}

void BlinkPD3LedTask(void)
{
    if (TaskCanRun(LED_TASK)) {
        //        PD_ODR = PD_ODR ^ 0x08; // LED reverse
        printf("Data is: %u\n", testData++);
        GPIO_WriteReverse(GPIOD, GPIO_PIN_3);
        TaskRunClear(LED_TASK);
    }
}

void ReadKeyboardTask(void)
{
    unsigned char keyValue;
    if (TaskCanRun(KEYBOARD_TASK)) {
        TM1638Readkey(&keyValue);
        if (keyValue != 0xFF) {
            if (keyValue < 10) {
                TM1638OneSymbolDisplay(0, keyValue);
                switch (keyValue) {
                case 1:
                    break;
                case 2:
                    break;
                default:
                    break;
                }
            } else if (keyValue < 20) {
                TM1638OneSymbolDisplay(1, keyValue - 10);
            }
        }
        TaskRunClear(KEYBOARD_TASK);
    }
}
