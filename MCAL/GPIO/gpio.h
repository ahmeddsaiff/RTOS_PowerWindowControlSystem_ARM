#ifndef GPIO_H_
#define GPIO_H_

#include "std_types.h"

#define GPIO_PORTF_PRIORITY_MASK      0xFF1FFFFF
#define GPIO_PORTF_PRIORITY_BITS_POS  21
#define GPIO_PORTF_INTERRUPT_PRIORITY 5

#define GPIO_PORTE_PRIORITY_MASK    0xFFFFFF1F
#define GPIO_PORTE_PRIORITY_BITS_POS  5
#define GPIO_PORTE_INTERRUPT_PRIORITY 7

#define GPIO_PORTB_PRIORITY_MASK    0xFFFF1FFF
#define GPIO_PORTB_PRIORITY_BITS_POS  13
#define GPIO_PORTB_INTERRUPT_PRIORITY 6


#define PRESSED                ((uint8)0x00)
#define RELEASED               ((uint8)0x01)

void GPIO_BuiltinButtonsLedsInit(void);
/*Enable PB1 , PB2 ,PB3 & PB4*/
void GPIO_DriverPassengerButtons_Init(void);
//PE3 -> Lock button
void GPIO_LockButtonInit(void);

/*Enable PD6 & PD7*/
void GPIO_LimitSiwtchesButtons_Init(void);
//PB7
void GPIO_JammingButtonInit(void);

void GPIO_RedLedOn(void);
void GPIO_BlueLedOn(void);
void GPIO_GreenLedOn(void);

void GPIO_RedLedOff(void);
void GPIO_BlueLedOff(void);
void GPIO_GreenLedOff(void);

void GPIO_RedLedToggle(void);
void GPIO_BlueLedToggle(void);
void GPIO_GreenLedToggle(void);

uint8 GPIO_SW1GetState(void);
uint8 GPIO_SW2GetState(void);

//PB1 -> driver up
uint8 GPIO_DriverUpButton_GetState(void);
//PB2 -> driver down
uint8 GPIO_DriverDownButton_GetState(void);
//PB3 -> passenger up
uint8 GPIO_PassengerUpButton_GetState(void);
//PB4 -> passenger down
uint8 GPIO_PassengerDownButton_GetState(void);
//PE3 -> LOCK button
uint8 GPIO_LockButton_GetState(void);
//PD7 -> DOWN Limit Switch
uint8 GPIO_LimitSwitchDOWN_GetState(void);
//PD6 -> UP Limit Switch
uint8 GPIO_LimitSwitchUP_GetState(void);
//PB7 -> Jamming Switch
uint8 GPIO_JammingSwitchUP_GetState(void);

void GPIO_SW1EdgeTriggeredInterruptInit(void);
void GPIO_SW2EdgeTriggeredInterruptInit(void);


#endif /* GPIO_H_ */
