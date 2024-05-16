/*
 * TasksImplementation.h
 *
 *  Created on: May 11, 2024
 *      Author: ahmed
*/

#ifndef TASKSIMPLMN_TASKSIMPLEMENTATION_H_
#define TASKSIMPLMN_TASKSIMPLEMENTATION_H_


#define SYSTEM_ON 1
#define SYSTEM_OFF 2
#define LOCK_ON 1


//Motor directions
#define STOP 0
#define CW 1
#define ACW 2


/* FreeRTOS tasks */
void vDriverTask(void *pvParameters);
void vPassengerTask(void *pvParameters);

void vWindowUpTask(void *pvParameters);
void vWindowDownTask(void *pvParameters);

void vONandOFFHandlerTask(void *pvParameters);

void vWindowLockHandlerTask(void *pvParameters);

void vJammingButtonHandlerTask(void *pvParameters);
void prvSetupHardware(void);
void Delay_MS(unsigned long long n);
void vMainCreations(void);
void vMotorFunction(char Direction);
void GPIO_Motor_Init(void);

#endif /* TASKSIMPLMN_TASKSIMPLEMENTATION_H_ */
