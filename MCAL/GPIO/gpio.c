#include "gpio.h"
#include "tm4c123gh6pm_registers.h"

void GPIO_BuiltinButtonsLedsInit(void)
{
    /*
     * PF0 --> SW2
     * PF1 --> Red LED
     * PF2 --> Blue LED
     * PF3 --> Green LED
     * PF4 --> SW1
     */

    /* Enable clock for PORTF and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x20;
    while(!(SYSCTL_PRGPIO_REG & 0x20));

    GPIO_PORTF_LOCK_REG   = 0x4C4F434B;                       /* Unlock the GPIO_PORTF_CR_REG */
    GPIO_PORTF_CR_REG    |= (1<<0);                           /* Enable changes on PF0 */
    GPIO_PORTF_AMSEL_REG &= 0xE0;                             /* Disable Analog on PF0, PF1, PF2, PF3 and PF4 */
    GPIO_PORTF_PCTL_REG  &= 0xFFF00000;                       /* Clear PMCx bits for PF0, PF1, PF2, PF3 and PF4 to use it as GPIO pins */
    GPIO_PORTF_DIR_REG   &= ~(1<<0) & ~(1<<4);                /* Configure PF0 & PF4 as input pins */
    GPIO_PORTF_DIR_REG   |= ((1<<1) | (1<<2) | (1<<3));       /* Configure PF1, PF2 & PF3 as output pins */
    GPIO_PORTF_AFSEL_REG &= 0xE0;                             /* Disable alternative function on PF0, PF1, PF2, PF3 and PF4 */
    GPIO_PORTF_PUR_REG   |= ((1<<0)|(1<<4));                  /* Enable pull-up on PF0 & PF4 */
    GPIO_PORTF_DEN_REG   |= 0x1F;                             /* Enable Digital I/O on PF0, PF1, PF2, PF3 and PF4 */
    GPIO_PORTF_DATA_REG  &= ~(1<<1) & ~(1<<2) & ~(1<<3);      /* Clear bits 1, 2 & 3 in Data register to turn off the LEDs */

}

/*Enable PD0 , PD1 ,PD2 & PD3*/
void GPIO_DriverPassengerButtons_Init(void)
{
    /* Enable clock for PORTD and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x08;
    while(!(SYSCTL_PRGPIO_REG & 0x08));

    GPIO_PORTD_LOCK_REG = 0x4C4F434B; /* Unlock the GPIO_PORTD_CR_R */
    GPIO_PORTD_CR_REG |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3); /* Enable changes on PD0 , PD1 ,PD2 & PD3 */
    GPIO_PORTD_AMSEL_REG &= ~(1 << 0) & ~(1 << 1) & ~(1 << 2) & ~(1 << 3); /* Disable Analog on PD0 , PD1 ,PD2 & PD3 */
    GPIO_PORTD_PCTL_REG &= 0xFFFF0000; /* Clear PMCx bits for PD0 , PD1 ,PD2 & PD3 to use it as GPIO pin */
    GPIO_PORTD_DIR_REG &= ~(1 << 0) & ~(1 << 1) & ~(1 << 2) & ~(1 << 3); /* Configure PD0 , PD1 ,PD2 & PD3 as input pin */
    GPIO_PORTD_AFSEL_REG &= ~(1 << 0) & ~(1 << 1) & ~(1 << 2) & ~(1 << 3); /* Disable alternative function on PD0 , PD1 ,PD2 & PD3 */
    GPIO_PORTD_PUR_REG   |= (1<< 0)| (1 << 1) | (1 << 2) | (1 << 3);       /* Enable pull-up on PD0 , PD1 , PD2 & PD3 */
    GPIO_PORTD_DEN_REG |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3); /* Enable Digital I/O on PD0 , PD1 ,PD2 & PD3 */
}

//PE4 -> IN1
//PE5 -> IN2
void GPIO_Motor_Init(void)
{
    /* Enable clock for PORTE and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x10;
    while(!(SYSCTL_PRGPIO_REG & 0x10));

    GPIO_PORTE_LOCK_REG = 0x4C4F434B; /* Unlock the GPIO_PORTE_CR_R */
    GPIO_PORTE_CR_REG |= (1 << 4) | (1 << 5); /* Enable changes on PE3 */
    GPIO_PORTE_AMSEL_REG &= ~(1 << 4) & ~(1 << 5); /* Disable Analog on PE3*/
    GPIO_PORTE_PCTL_REG &= 0xFF00FFFF; /* Clear PMCx bits for PE3 to use it as GPIO pin */
    GPIO_PORTE_DIR_REG |= (1 << 4) | (1 << 5); /* Configure PE3 as input pin */
    GPIO_PORTE_AFSEL_REG &= ~(1 << 4) | (1 << 5); /* Disable alternative function on PE3*/
//    GPIO_PORTE_PUR_REG   |= (1 << 4) | (1 << 5);                  /* Enable pull-up on PE3 */
    GPIO_PORTE_DEN_REG |= (1 << 4) | (1 << 5); /* Enable Digital I/O on PE3*/
}

/*Enable PD6 & PD7*/
void GPIO_LimitSiwtchesButtons_Init(void)
{
    /* Enable clock for PORTD and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x08;
    while(!(SYSCTL_PRGPIO_REG & 0x08));

    GPIO_PORTD_LOCK_REG = 0x4C4F434B; /* Unlock the GPIO_PORTD_CR_R */
    GPIO_PORTD_CR_REG |= (1 << 6) | (1 << 7); /* Enable changes on PD6 & PD7 */
    GPIO_PORTD_AMSEL_REG &= ~(1 << 6) & ~(1 << 7); /* Disable Analog on PB1 , PB2 ,PB3 & PB4 */
    GPIO_PORTD_PCTL_REG &= 0x00FFFFFF; /* Clear PMCx bits for PB1 to use it as GPIO pin */
    GPIO_PORTD_DIR_REG &= ~(1 << 6) & ~(1 << 7); /* Configure PD6 & PD7 as input pin */
    GPIO_PORTD_AFSEL_REG &= ~(1 << 6) & ~(1 << 7); /* Disable alternative function on PD6 & PD7 */
    GPIO_PORTD_PUR_REG   |= (1<<6) | (1<<7) ;                  /* Enable pull-up on PD6 & PD7 */
    GPIO_PORTD_DEN_REG |= (1 << 6) | (1 << 7); /* Enable Digital I/O on PD6 & PD7 */
}

//PB3
void GPIO_JammingButtonInit(void)
{
    /* Enable clock for PORTE and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x02;
    while(!(SYSCTL_PRGPIO_REG & 0x02));

    GPIO_PORTB_LOCK_REG = 0x4C4F434B; /* Unlock the GPIO_PORTE_CR_R */
    GPIO_PORTB_CR_REG |= (1 << 3); /* Enable changes on PE3 */
    GPIO_PORTB_AMSEL_REG &= ~(1 << 3); /* Disable Analog on PE3*/
    GPIO_PORTB_PCTL_REG &= 0xFFFF0FFF; /* Clear PMCx bits for PE3 to use it as GPIO pin */
    GPIO_PORTB_DIR_REG &= ~(1 << 3); /* Configure PE3 as input pin */
    GPIO_PORTB_AFSEL_REG &= ~(1 << 3); /* Disable alternative function on PE3*/
    GPIO_PORTB_PUR_REG   |= (1 << 3);                  /* Enable pull-up on PE3 */
    GPIO_PORTB_DEN_REG |= (1 << 3); /* Enable Digital I/O on PE3*/

    //Enable Interrupt
    GPIO_PORTB_IS_REG    &= ~(1<<3);      /* PF0 detect edges */
    GPIO_PORTB_IBE_REG   &= ~(1<<3);      /* PF0 will detect a certain edge */
    GPIO_PORTB_IEV_REG   &= ~(1<<3);      /* PF0 will detect a falling edge */
    GPIO_PORTB_ICR_REG   |= (1<<3);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
    GPIO_PORTB_IM_REG    |= (1<<3);       /* Enable Interrupt on PF0 pin */
    /* Set GPIO PORTF priority as 5 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_PRI0_REG = (NVIC_PRI0_REG & GPIO_PORTB_PRIORITY_MASK) | (GPIO_PORTB_INTERRUPT_PRIORITY<<GPIO_PORTB_PRIORITY_BITS_POS);
    NVIC_EN0_REG         |= 0x00000002;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Register */
}

//PE3 -> Lock button
void GPIO_LockButtonInit(void)
{
    /* Enable clock for PORTE and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x10;
    while(!(SYSCTL_PRGPIO_REG & 0x10));

    GPIO_PORTE_LOCK_REG = 0x4C4F434B; /* Unlock the GPIO_PORTE_CR_R */
    GPIO_PORTE_CR_REG |= (1 << 3); /* Enable changes on PE3 */
    GPIO_PORTE_AMSEL_REG &= ~(1 << 3); /* Disable Analog on PE3*/
    GPIO_PORTE_PCTL_REG &= 0xFFFF0FFF; /* Clear PMCx bits for PE3 to use it as GPIO pin */
    GPIO_PORTE_DIR_REG &= ~(1 << 3); /* Configure PE3 as input pin */
    GPIO_PORTE_AFSEL_REG &= ~(1 << 3); /* Disable alternative function on PE3*/
    GPIO_PORTE_PUR_REG   |= (1<<3);                  /* Enable pull-up on PE3 */
    GPIO_PORTE_DEN_REG |= (1 << 3); /* Enable Digital I/O on PE3*/

    //to enable Interrupt
    GPIO_PORTE_IS_REG    &= ~(1<<3);      /* PE3 detect edges */
    GPIO_PORTE_IBE_REG   &= ~(1<<3);      /* PE3 will detect a certain edge */
    GPIO_PORTE_IEV_REG   &= ~(1<<3);      /* PE3 will detect a falling edge */
    GPIO_PORTE_ICR_REG   |= (1<<3);       /* Clear Trigger flag for PE3 (Interrupt Flag) */
    GPIO_PORTE_IM_REG    |= (1<<3);       /* Enable Interrupt on PE3 pin */
    /* Set GPIO PORTE priority as 7 by set Bit number 5, 6 and 7 with value 3 */
    NVIC_PRI1_REG = (NVIC_PRI1_REG & GPIO_PORTE_PRIORITY_MASK) | (GPIO_PORTE_INTERRUPT_PRIORITY<<GPIO_PORTE_PRIORITY_BITS_POS);
    NVIC_EN0_REG         |= 0x00000010;   /* Enable NVIC Interrupt for GPIO PORTE by set bit number 4 in EN0 Register */
}


//PE3 -> LOCK button
uint8 GPIO_LockButton_GetState(void){
    return ((GPIO_PORTE_DATA_REG >> 3) & 0x01);
}

//PD0 -> driver up
uint8 GPIO_DriverUpButton_GetState(void){
    return ((GPIO_PORTD_DATA_REG >> 0) & 0x01);
}

//PD1 -> driver down
uint8 GPIO_DriverDownButton_GetState(void){
    return ((GPIO_PORTD_DATA_REG >> 1) & 0x01);
}

//PD2 -> passenger up
uint8 GPIO_PassengerUpButton_GetState(void){
    return ((GPIO_PORTD_DATA_REG >> 2) & 0x01);
}

//PD3 -> passenger down
uint8 GPIO_PassengerDownButton_GetState(void){
    return ((GPIO_PORTD_DATA_REG >> 3) & 0x01);
}

//PD6 -> UP Limit Switch
uint8 GPIO_LimitSwitchUP_GetState(void){
    return ((GPIO_PORTD_DATA_REG >> 6) & 0x01);
}

//PD7 -> DOWN Limit Switch
uint8 GPIO_LimitSwitchDOWN_GetState(void){
    return ((GPIO_PORTD_DATA_REG >> 7) & 0x01);
}

//PB3 -> Jamming Switch
uint8 GPIO_JammingSwitchUP_GetState(void){
    return ((GPIO_PORTB_DATA_REG >> 3) & 0x01);
}

void GPIO_RedLedOn(void)
{
    GPIO_PORTF_DATA_REG |= (1<<1);  /* Red LED ON */
}

void GPIO_BlueLedOn(void)
{
    GPIO_PORTF_DATA_REG |= (1<<2);  /* Blue LED ON */
}

void GPIO_GreenLedOn(void)
{
    GPIO_PORTF_DATA_REG |= (1<<3);  /* Green LED ON */
}

void GPIO_RedLedOff(void)
{
    GPIO_PORTF_DATA_REG &= ~(1<<1);  /* Red LED OFF */
}

void GPIO_BlueLedOff(void)
{
    GPIO_PORTF_DATA_REG &= ~(1<<2);  /* Blue LED OFF */
}

void GPIO_GreenLedOff(void)
{
    GPIO_PORTF_DATA_REG &= ~(1<<3);  /* Green LED OFF */
}

void GPIO_RedLedToggle(void)
{
    GPIO_PORTF_DATA_REG ^= (1<<1);  /* Red LED is toggled */
}

void GPIO_BlueLedToggle(void)
{
    GPIO_PORTF_DATA_REG ^= (1<<2);  /* Blue LED is toggled */
}

void GPIO_GreenLedToggle(void)
{
    GPIO_PORTF_DATA_REG ^= (1<<3);  /* Green LED is toggled */
}

uint8 GPIO_SW1GetState(void)
{
    return ((GPIO_PORTF_DATA_REG >> 4) & 0x01);
}

uint8 GPIO_SW2GetState(void)
{
    return ((GPIO_PORTF_DATA_REG >> 0) & 0x01);
}

void GPIO_SW1EdgeTriggeredInterruptInit(void)
{
    GPIO_PORTF_IS_REG    &= ~(1<<4);      /* PF4 detect edges */
    GPIO_PORTF_IBE_REG   &= ~(1<<4);      /* PF4 will detect a certain edge */
    GPIO_PORTF_IEV_REG   &= ~(1<<4);      /* PF4 will detect a falling edge */
    GPIO_PORTF_ICR_REG   |= (1<<4);       /* Clear Trigger flag for PF4 (Interrupt Flag) */
    GPIO_PORTF_IM_REG    |= (1<<4);       /* Enable Interrupt on PF4 pin */
    /* Set GPIO PORTF priority as 5 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_PRI7_REG = (NVIC_PRI7_REG & GPIO_PORTF_PRIORITY_MASK) | (GPIO_PORTF_INTERRUPT_PRIORITY<<GPIO_PORTF_PRIORITY_BITS_POS);
    NVIC_EN0_REG         |= 0x40000000;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Register */
}

void GPIO_SW2EdgeTriggeredInterruptInit(void)
{
    GPIO_PORTF_IS_REG    &= ~(1<<0);      /* PF0 detect edges */
    GPIO_PORTF_IBE_REG   &= ~(1<<0);      /* PF0 will detect a certain edge */
    GPIO_PORTF_IEV_REG   &= ~(1<<0);      /* PF0 will detect a falling edge */
    GPIO_PORTF_ICR_REG   |= (1<<0);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
    GPIO_PORTF_IM_REG    |= (1<<0);       /* Enable Interrupt on PF0 pin */
    /* Set GPIO PORTF priority as 5 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_PRI7_REG = (NVIC_PRI7_REG & GPIO_PORTF_PRIORITY_MASK) | (GPIO_PORTF_INTERRUPT_PRIORITY<<GPIO_PORTF_PRIORITY_BITS_POS);
    NVIC_EN0_REG         |= 0x40000000;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Register */
}

