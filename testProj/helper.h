#ifndef _HELPER_H_
#define _HELPER_H_

/**
 * @brief call this function the first time to start convert temp, next time to read temperature
 */
void GetDisplayTempTask(void);
/**
 * @brief Only time interrupt can invoke this function, see the code for detail
 */
void TimeInterruptWork(void);
/**
 * @brief check the whether the task can run.
 *
 * @param taskNum task number of a task
 *
 * @return  0 for can't run, 1 for can run.
 */
unsigned char TaskCanRun(unsigned char taskNum);
/**
 * @brief blink led on pd3 port
 */
void BlinkPD3LedTask(void);
/**
 * @brief read the keyboard task
 */
void ReadKeyboardTask(void);
/**
 * @brief clear isRun flag in task data
 *
 * @param taskNum task number
 */
void TaskRunClear(unsigned char taskNum);
void DisplayTemperature(unsigned char temperature);
void DisplayFanSpeed(void);
#endif
