#include "task.h"
#include "../DS18B20/DS18B20.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s_gpio.h"
#include "../TM1638/TM1638.h"
#include "../core/PrintfUtility.h"
#include "../core/atomic.h"
#include "Control.h"
#include "Display.h"
#include "param.h"

#define NUM_OF_TASK 8
#define KEYBOARD_TASK 1
#define LED_TASK 6
#define TEMPERATURE_TASK 7
#define NO_KEY_PRESSED 0xFF

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

void TemperatureTask(void)
{
    if (TaskCanRun(TEMPERATURE_TASK)) {
        static unsigned char convertSuccess = 0;
        short int            temperature;
        if (!convertSuccess) {
            if (DS18B20ConvertTemp()) {
                convertSuccess = 1; // convert success, next cycle we can read temperature
            }
        } else {
            convertSuccess = 0;
            if (DS18B20GetTemp(&temperature)) {
                DisplayRealTemperature(temperature);
                if (ControlFanSpeed(temperature)) {
                    DisplayFanValveState(GetFanSpeedState(), GetValveState());
                }
            }
        }
        TaskRunClear(TEMPERATURE_TASK);
    }
}

void BlinkPD3LedTask(void)
{
    if (TaskCanRun(LED_TASK)) {
        GPIO_WriteReverse(GPIOD, GPIO_PIN_3);
        TaskRunClear(LED_TASK);
    }
}

void ReadKeyboardTask(void)
{
    static unsigned char lastKey             = NO_KEY_PRESSED;
    static unsigned char longPressCounter    = 0; // long press first triger counter
    static unsigned char longPressSubCounter = 0; // sub counter for quick triger of long press
    static unsigned char longPressMode       = 0; // 0: not in long press mode, !0: in long press mode

    const unsigned char longPressTime = 200; // period of this task T*100 = 9.6*200 ms
    const unsigned char trigerPeriod  = 10;  // trigerPeriod 10*9.6 ms

    unsigned char keyValue = NO_KEY_PRESSED;

    if (TaskCanRun(KEYBOARD_TASK)) {
        TM1638Readkey(&keyValue);
        if (keyValue != lastKey) {          // key state changed
            if (keyValue == NO_KEY_PRESSED) // key released event, last key is lastKey
            {
                putchar(lastKey);
                putchar(0xAA);
                putchar(0xAA);
                putchar(0x01); // 1 for release event
                switch (lastKey) {
                case 0x01:
                    IncreaseTemperatureSetpoint(1);
                    DisplayTemperatureSetpoint(ReadTemperatureSetpoint());
                    break;
                case 0x05:
                    IncreaseTemperatureSetpoint(-1);
                    DisplayTemperatureSetpoint(ReadTemperatureSetpoint());
                    break;
                case 0x02:
                    ChangeFanMode();
                    DisplayFanSpeedMode(GetFanSpeedMode());
                    if (GetFanSpeedMode() != FAN_SPEED_AUTO_MODE) {
                        DisplayFanValveState(GetFanSpeedState(), GetValveState());
                    }
                    break;
                case 0x06:
                    ChangeValveOutput(!GetValveState());
                    DisplayFanValveState(GetFanSpeedState(), GetValveState());
                    break;
                default:
                    break;
                }
            } else { // new key pressed event, new key is keyValue
                     //                putchar(keyValue);
                     //                putchar(0xAA);
                     //                putchar(0xAA);
                     //                putchar(0x02); // 2 for pressed down event
            }
            lastKey             = keyValue; // update lastKey
            longPressCounter    = 0;
            longPressSubCounter = 0;
            longPressMode       = 0;
        } else if (keyValue == NO_KEY_PRESSED) { // no key pressed, should do nothing
        } else {                                 // key pressed and not released, key is keyValue
            if (!longPressMode) {
                if (longPressCounter++ > longPressTime) {
                    longPressMode       = 1;
                    longPressSubCounter = 0;
                    // triger the first long press event
                    //                    putchar(keyValue);
                    //                    putchar(0xAA);
                    //                    putchar(0xAA);
                    //                    putchar(0x03); // 3 for long press first trigered
                }
            } else {
                if (longPressSubCounter++ > trigerPeriod) {
                    longPressSubCounter = 0;
                    //                    putchar(keyValue);
                    //                    putchar(0xBB);
                    //                    putchar(0xBB);
                    //                    putchar(0x04); // 3 for long press first trigered
                    // triger next time long press event
                }
            }
        }
    }
    TaskRunClear(KEYBOARD_TASK);
}
