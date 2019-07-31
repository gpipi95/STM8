#ifndef _CONTROL_H
#define _CONTROL_H

#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"

#define FAN_SPEED_AUTO_MODE 0
#define FAN_SPEED_LOW_MODE 1
#define FAN_SPEED_MIDDLE_MODE 2
#define FAN_SPEED_HIGH_MODE 3

#define FAN_SPEED_CLOSED 0
#define FAN_SPEED_LOW 1
#define FAN_SPEED_MIDDLE 2
#define FAN_SPEED_HIGH 3

#define HIGH_THRESHOLD 3        // unit is degree, difference is bigger than this turn fan speed to high
#define MIDDLE_THRESHOLD 2      // unit is degree, difference is bigger than this turn fan speed to middle
#define LOW_THRESHOLD 1         // difference below this turn to low fan speed
#define CLOSE_FAN_THRESHOLD 0.5 // below this turn off fan

#define CONTROL_PORT GPIOB
#define FAN_HIGH_IO GPIO_PIN_2    // 风扇高速输出口
#define FAN_MIDDLE_IO GPIO_PIN_1  // 风扇中速输出口
#define FAN_LOW_IO GPIO_PIN_0     // 风扇低速输出口
#define VALVE_OPEN_IO GPIO_PIN_3  // 打开阀门输出口
#define VALVE_CLOSE_IO GPIO_PIN_4 // 关闭阀门输出口

void          IncreaseTemperatureSetpoint(char step);
void          ChangeFanMode(void);
unsigned char GetFanSpeedMode(void);
unsigned char GetFanSpeedState(void);
bool          GetValveState(void);
void          InitFanControl(void);
/**
 * @brief ChangeFanOutput
 *
 * @param state FAN_SPEED_LOW or FAN_SPEED_MIDDLE or FAN_SPEED_HIGH or FAN_SPEED_CLOSED
 * @return true for real changed, false for no change
 */
bool ChangeFanOutput(unsigned char state);

/**
 * @brief change valve state 
 *
 * @param state true for open, false for close
 */
void ChangeValveOutput(bool state);
/**
 * @brief 控制风扇速度
 *
 * @return 风扇速度发生改变则返回TRUE，否者FALSE
 */
bool ControlFanSpeed(short int temperature);
#endif
