#include "TasksImplementation.h"

#include "gpio.h"
#include "std_types.h"
#include "UART/uart0.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tm4c123gh6pm_registers.h"
#include "FreeRTOSConfig.h"
#include "BIT_MATH.h"

//global variable for the on and off queue
uint8 gONandOFFvalueToSend;
//global variable for the Lock queue
uint8 gLOCKvalueToSend;
//global variable for the lock flag
uint8 gFlagLock;
//global variable for the Jamming flag
uint8 gFlagWindowUpJamming;
//global variable for PassengerUp
uint8 gPassengerUp;
//global variable for PassengerDown
uint8 gPassengerDown;
//global variable for JammingCondition
uint8 gJammingCondition;



//Tasks Handle
TaskHandle_t xDriverTaskHandle;
TaskHandle_t xPassengerTaskHandle;
TaskHandle_t xWindowUpTaskHandle;
TaskHandle_t xWindowDownTaskHandle;
TaskHandle_t xWindowLockTaskHandle;
TaskHandle_t xJammingButtonTaskHandle;

//on and off queue for handling the on and off of the system
xQueueHandle xQueueOnOff;

//on and off queue for handling the LOCK button of the system
xQueueHandle xQueueLOCK;

/* FreeRTOS Mutexes */
xSemaphoreHandle xMutex;

xSemaphoreHandle xBinarySemaphoreWindowUP;
xSemaphoreHandle xBinarySemaphoreWindowDOWN;
xSemaphoreHandle xBinarySemaphorJammingButton;


#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 369
void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while (count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n));
}


void vMotorFunction(char Direction)
{
    switch(Direction)
    {
    case STOP:
        GPIO_PORTE_DATA_REG &= ~(1 << 4);
        GPIO_PORTE_DATA_REG &= ~(1 << 5);
        break;
    case ACW:
        GPIO_PORTE_DATA_REG |= (1 << 4);
        GPIO_PORTE_DATA_REG &= ~(1 << 5);
        break;
    case CW:
        GPIO_PORTE_DATA_REG |= (1 << 5);
        GPIO_PORTE_DATA_REG &= ~(1 << 4);
        break;
    }
}

void vMainCreations(void)
{
    /* Create a Mutex */
    xMutex = xSemaphoreCreateMutex();

    /* Create a queue with length of 2 and size of uint8 for each item */
    xQueueOnOff = xQueueCreate(2, sizeof(uint8));
    xQueueLOCK = xQueueCreate(2, sizeof(uint8));

    xBinarySemaphoreWindowUP = xSemaphoreCreateBinary();
    xBinarySemaphoreWindowDOWN = xSemaphoreCreateBinary();
    xBinarySemaphorJammingButton = xSemaphoreCreateBinary();

    xTaskCreate(vONandOFFHandlerTask, "System's On and Off Handler",
    configMINIMAL_STACK_SIZE,
                NULL_PTR, 6, NULL_PTR);

    xTaskCreate(vJammingButtonHandlerTask, "System's Jamming Button",
    configMINIMAL_STACK_SIZE,
    NULL_PTR, 5, &xJammingButtonTaskHandle);

    xTaskCreate(vWindowLockHandlerTask, "Window Lock Task",
    configMINIMAL_STACK_SIZE,
    NULL_PTR, 4, &xWindowLockTaskHandle);

    xTaskCreate(vWindowUpTask, "Window up Task",
    configMINIMAL_STACK_SIZE,
    NULL_PTR, 3, &xWindowUpTaskHandle);

    xTaskCreate(vWindowDownTask, "Window Down Task",
    configMINIMAL_STACK_SIZE,
                NULL_PTR, 3, &xWindowDownTaskHandle);

    xTaskCreate(vDriverTask, "Driver Task",
    configMINIMAL_STACK_SIZE,
    NULL_PTR, 2, &xDriverTaskHandle);

    xTaskCreate(vPassengerTask, "Passenger Task",
    configMINIMAL_STACK_SIZE,
    NULL_PTR, 2, &xPassengerTaskHandle);

    //han3mel suspend le kol al tasks ma3ada task al vONandOFFHandlerTask
    vTaskSuspend(xWindowUpTaskHandle);
    vTaskSuspend(xDriverTaskHandle);
    vTaskSuspend(xWindowDownTaskHandle);
    vTaskSuspend(xPassengerTaskHandle);
    vTaskSuspend(xWindowLockTaskHandle);
    vTaskSuspend(xJammingButtonTaskHandle);

    vTaskStartScheduler();
}



//ON & OFF Button -> PF0 "In Tiva"
void vONandOFFHandlerTask(void *pvParameters)
{
    UART0_SendString("On and off task started \r\n");
    uint8 ReceivedONOFFvalue;
    for (;;)
    {
        /* Waits until the queue is not empty (button is pressed). */
        if (xQueueReceive(xQueueOnOff, &ReceivedONOFFvalue,
        portMAX_DELAY) == pdPASS)
        {
            //check once more -> 3lshan may7salsh eno ye3mel recieve marteen
            //                -> aw en al zorar yefdal medas
            while (GPIO_SW2GetState() == PRESSED);

            //lw al value = 2 -> yeb2a kda OFF
            //            = 1 -> yeb2a kda ON
            switch (ReceivedONOFFvalue)
            {
            case SYSTEM_ON:
                //ba3mel taskresume lel tasks kolaha
                UART0_SendString("System is ON \r\n");

                vTaskResume(xDriverTaskHandle);
                //Lw al system kan OFF , wana dost 3ala zorar al lock
                //Zorar al lock haye3mel al function bta3to eno ye2fel al passenger ; awl lama al system yetft7 (Te3mel contact al 3rbya y3ny)
                if (gFlagLock == 0)
                {
                    vTaskResume(xPassengerTaskHandle);
                }
                vTaskResume(xWindowUpTaskHandle);
                vTaskResume(xWindowDownTaskHandle);
                vTaskResume(xWindowLockTaskHandle);
                vTaskResume(xJammingButtonTaskHandle);
                break;
            case SYSTEM_OFF:
                //basafar al global variable "Flag" 3lshan al mara aly gaya
                //w b3mel suspend lel tasks
                gONandOFFvalueToSend = 0;
                UART0_SendString("System OFF \r\n");
                vTaskSuspend(xDriverTaskHandle);
                vTaskSuspend(xPassengerTaskHandle);
                vTaskSuspend(xWindowUpTaskHandle);
                vTaskSuspend(xWindowDownTaskHandle);
                vTaskSuspend(xWindowLockTaskHandle);
                vTaskSuspend(xJammingButtonTaskHandle);
                break;
            //Da check zyada -> in case eno 7asal w geh ay rakam mokhtlf
            // AL MAFROUD DA MAY7SLSH TAB3AN -> bas "Just in case y3ny"
            default:
                gONandOFFvalueToSend = 0;
                break;
            }
        }
    }
}


//PB3 -> Jamming Button
void vJammingButtonHandlerTask(void *pvParameters)
{
    //baasafer al BinarySemaohore bta3y -> brdo just in case eno awl lama bad2na system kan be True wala haga
    //                                     "True dy azdy byha -> ka2eno ma3molo give"
    xSemaphoreTake(xBinarySemaphorJammingButton, 0);

    for (;;)
    {
        if (xSemaphoreTake(xBinarySemaphorJammingButton,
                           portMAX_DELAY) == pdTRUE)
        {
            GPIO_RedLedOff();
            GPIO_BlueLedOn();
            UART0_SendString("Blue Window Jamming Down \r\n");
            //Motor Down
            vMotorFunction(ACW);
            Delay_MS(500);
            vMotorFunction(STOP);
            GPIO_BlueLedOff();
        }
    }
}

//PE3 -> Lock Button
void vWindowLockHandlerTask(void *pvParameters)
{
    UART0_SendString("LOCK handler task started \r\n");
    uint8 ReceivedLOCKvalue;

    for (;;)
    {
        /* Waits until the queue is not empty (button is pressed). */
        if (xQueueReceive(xQueueLOCK, &ReceivedLOCKvalue,
        portMAX_DELAY) == pdPASS)
        {
            //check once more -> 3lshan may7salsh eno ye3mel recieve Kaza mara
            //                -> aw en al zorar yefdal medas
            while (GPIO_LockButton_GetState() == PRESSED);

            //lw al value = 1 -> yeb2a kda et3mlo Lock
            //            = otherwise -> yeb2a kda shelna al lock
            //          "mostly hateb2a
            //                  = 2    "
            switch (ReceivedLOCKvalue)
            {
            case LOCK_ON:
                //b3mel suspend lel tasks
                UART0_SendString("passenger system is LOCKED \r\n");
                vTaskSuspend(xPassengerTaskHandle);
                //ba7ot al Flag bta3 al lock be 1 -> 3lshan lw ana fel passenger w b3mel eny barfa3 ezaz aw banazelo
                //                                   w al lock edass -> sa3etaha mfroud mykmlsh wala raf3 wala nzoul lel ezaz
                //fa b3mel 3aleh check fel task bta3t window up and window down "Lw al passenger howa aly days al zorar"
                gFlagLock = pdTRUE;
                break;
            default:
                //basafar al global variable "Flag" 3lshan al mara aly gaya
                //ba3mel taskresume lel tasks bta3t al passenger
                UART0_SendString("passenger system is UNlocked \r\n");
                vTaskResume(xPassengerTaskHandle);
                gLOCKvalueToSend = 0;
                gFlagLock = pdFALSE;
                break;
            }
        }
    }
}

void vDriverTask(void *pvParameters)
{
    for (;;)
    {
        while (GPIO_DriverUpButton_GetState() == PRESSED)
        {
            xSemaphoreGive(xBinarySemaphoreWindowUP);
        }

        while (GPIO_DriverDownButton_GetState() == PRESSED)
        {
            xSemaphoreGive(xBinarySemaphoreWindowDOWN);
        }
    }

}

void vPassengerTask(void *pvParameters)
{
    for (;;)
    {
        while (GPIO_PassengerUpButton_GetState() == PRESSED)
        {
            gPassengerUp = 1;
            xSemaphoreGive(xBinarySemaphoreWindowUP);
        }
        while (GPIO_PassengerDownButton_GetState() == PRESSED)
        {
            gPassengerDown = 1;
            xSemaphoreGive(xBinarySemaphoreWindowDOWN);

        }
        gPassengerUp = 0;
        gPassengerDown = 0;
    }
}

void vWindowUpTask(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(xBinarySemaphoreWindowUP,portMAX_DELAY) == pdTRUE)
        {
            if(xSemaphoreTake(xMutex , portMAX_DELAY))
            {
                //Bahot al flag bta3 al jamming b true 3lshan lw hasal jamming wana fel up -> yesht8l al jamming
                gFlagWindowUpJamming = pdTRUE;

                //b3d ma3amalt take lel semaphore w da5lt al task
                //ha3mel delay basseet w a3mel check -> lw lesa dayes al zorar yeb2a kda ana 3ayzo MANUAL
                //                                   -> lw msh dayes yeb2a ana kda 3ayzo automatic
                Delay_MS(500);
                //Manual
                if (GPIO_PassengerUpButton_GetState() == PRESSED
                        || GPIO_DriverUpButton_GetState() == PRESSED)
                {
                    //Toul mana dayes 3al zorar
                    //Ha5osh aashof al limit switch medas wala laa -> lw medas yeb2a may3mlsh haga
                    //b3d kda hashof meen aly dayes 3al zorar -> Driver yeb2a esht8l manual 3ady
                    //                                        -> Passenger yeb2a lazem a3mel check ->
                    //                                                                               al Lock ma3moul wala la
                    while (GPIO_PassengerUpButton_GetState() == PRESSED
                            || GPIO_DriverUpButton_GetState() == PRESSED)
                    {
                        if (GPIO_LimitSwitchUP_GetState() == RELEASED)
                        {
                            if(gFlagLock == pdFALSE && GPIO_PassengerUpButton_GetState() == PRESSED)
                            {
                                GPIO_BlueLedOff();
                                GPIO_RedLedToggle();
                                UART0_SendString("RED WINDOW UP (Passenger) \r\n");
                                vMotorFunction(CW);
                            }
                            else if(GPIO_DriverUpButton_GetState() == PRESSED)
                            {
                                GPIO_BlueLedOff();
                                GPIO_RedLedToggle();
                                UART0_SendString("RED WINDOW UP (Driver)\r\n");
                                vMotorFunction(CW);
                            }
                        }
                        else
                        {
                            vMotorFunction(STOP);
                        }
                    }
                    vMotorFunction(STOP);
                }
                //Automatic
                else if (GPIO_PassengerUpButton_GetState() == RELEASED
                        && GPIO_DriverUpButton_GetState() == RELEASED)
                {
                    //Lw al Lock ma3moul -> yeb2a 100% ana gy mn al driver w ha5osh fel else
                    if(gFlagLock == pdFALSE)
                    {
                        //Toul mana msh dayes 3ala ay zorar mn al "Lock , down (Driver aw Passenger)"
                        //hashof lw al passenger 3amel up w al lock msh m3moul -> yeb2a tamam
                        //Lw msh al passenger 3amel up -> dy m3naha en al driver howa aly 3amel up
                        while (GPIO_LimitSwitchUP_GetState() == RELEASED
                                && GPIO_DriverDownButton_GetState() == RELEASED
                                && GPIO_PassengerDownButton_GetState() == RELEASED
                                && gJammingCondition == pdFALSE)
                        {
                            if (gFlagLock == pdFALSE && gPassengerUp == pdTRUE)
                            {
                                GPIO_BlueLedOff();
                                GPIO_RedLedOn();
                                UART0_SendString("RED WINDOW UP (Passenger) \r\n");
                                vMotorFunction(CW);
                            }
                            else if (gPassengerUp == pdFALSE)
                            {
                                GPIO_BlueLedOff();
                                GPIO_RedLedOn();
                                UART0_SendString("RED WINDOW UP (Driver) \r\n");
                                vMotorFunction(CW);
                            }
                            else
                            {
                                vMotorFunction(STOP);
                                break;
                            }
                        }
                        vMotorFunction(STOP);
                    }
                    else
                    {
                        //talama lock ma3moul yeb2a ana ha3mel check 3ala al jamming w al w driverDownButton w limitSwitch bas
                        //al passenger kda kda ma3molo lock fa lw das msh hayw2fly al automatic bta3y
                        while (GPIO_LimitSwitchUP_GetState() == RELEASED
                                && GPIO_DriverDownButton_GetState() == RELEASED
                                && gJammingCondition == pdFALSE)
                        {
                            GPIO_BlueLedOff();
                            GPIO_RedLedOn();
                            UART0_SendString("RED WINDOW UP (Driver) \r\n");
                            vMotorFunction(CW);
                        }
                        vMotorFunction(STOP);
                    }

                }

            }

        }

        //Abl mat5rog mn al WindowUp -> Lazem te3mel False lel jamming ; 3lshan maysht8lsh fy ay heta tanya
        gFlagWindowUpJamming = pdFALSE;
        //W terg3 al condition bta3 al jamming le aslo -> 3lshan al mara aly gaya
        //                                                mayeb2ash dymn fy jamming -> fa msh hayeb2a fy autoamtic khales
        gJammingCondition = pdFALSE;

        xSemaphoreGive(xMutex);
    }
}

//Nafs al kalam aly mwgwd fel WindowUp bas ma3kous lel window down :)
void vWindowDownTask(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(xBinarySemaphoreWindowDOWN , portMAX_DELAY) == pdTRUE)
        {
            if(xSemaphoreTake(xMutex , portMAX_DELAY))
            {
                gFlagWindowUpJamming = pdFALSE;
                Delay_MS(500);
                //Manual
                if (GPIO_PassengerDownButton_GetState() == PRESSED
                        || GPIO_DriverDownButton_GetState() == PRESSED)
                {
                    while (GPIO_PassengerDownButton_GetState() == PRESSED
                            || GPIO_DriverDownButton_GetState() == PRESSED)
                    {
                        if (GPIO_LimitSwitchDOWN_GetState() == RELEASED)
                        {
                            if(gFlagLock == pdFALSE && GPIO_PassengerDownButton_GetState() == PRESSED)
                            {
                                GPIO_BlueLedOn();
                                GPIO_RedLedOff();
                                UART0_SendString("BLUE WINDOW DOWN (Passenger) \r\n");
                                vMotorFunction(ACW);
                            }
                            else if(GPIO_DriverDownButton_GetState() == PRESSED)
                            {
                                GPIO_BlueLedOn();
                                GPIO_RedLedOff();
                                UART0_SendString("BLUE WINDOW DOWN (Driver) \r\n");
                                vMotorFunction(ACW);
                            }
                        }
                        else
                        {
                            vMotorFunction(STOP);
                        }
                    }
                    vMotorFunction(STOP);
                }
                //Automatic
                else if (GPIO_PassengerDownButton_GetState() == RELEASED
                        && GPIO_DriverDownButton_GetState() == RELEASED)
                {
                    if(gFlagLock == pdFALSE)
                    {
                        while (GPIO_LimitSwitchDOWN_GetState() == RELEASED
                                && GPIO_DriverUpButton_GetState() == RELEASED
                                && GPIO_PassengerUpButton_GetState() == RELEASED)
                        {
                            if (gFlagLock == pdFALSE && gPassengerDown == pdTRUE)
                            {
                                GPIO_BlueLedOn();
                                GPIO_RedLedOff();
                                UART0_SendString("BLUE WINDOW DOWN (Passenger) \r\n");
                                vMotorFunction(ACW);
                            }
                            else if (gPassengerDown == pdFALSE)
                            {
                                GPIO_BlueLedOn();
                                GPIO_RedLedOff();
                                UART0_SendString("BLUE WINDOW DOWN (Driver) \r\n");
                                vMotorFunction(ACW);
                            }
                            else
                            {
                                vMotorFunction(STOP);
                                break;
                            }
                        }
                        vMotorFunction(STOP);
                    }
                    else
                    {
                        while (GPIO_LimitSwitchDOWN_GetState() == RELEASED
                                && GPIO_DriverUpButton_GetState() == RELEASED)
                        {
                            GPIO_BlueLedOn();
                            GPIO_RedLedOff();
                            UART0_SendString("BLUE WINDOW DOWN (Driver) \r\n");
                            vMotorFunction(ACW);
                        }
                        vMotorFunction(STOP);
                    }

                }
            }
        }
        xSemaphoreGive(xMutex);
    }
}

void prvSetupHardware(void)
{
    /* Place here any needed HW initialization such as GPIO, UART, etc.  */
    GPIO_BuiltinButtonsLedsInit();
    GPIO_DriverPassengerButtons_Init();
    GPIO_LockButtonInit();
    GPIO_LimitSiwtchesButtons_Init();
    GPIO_JammingButtonInit();
    GPIO_Motor_Init();

    //PF0 -> in tiva
    GPIO_SW2EdgeTriggeredInterruptInit();
    UART0_Init();

}

/* Handlers */

//PF0 -> ON&OFF Button "sw2 in tiva"
void GPIO_PORTF_Handler(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    GPIO_PORTF_ICR_REG |= 1 << 0;

    gONandOFFvalueToSend++;

    xQueueSendFromISR(xQueueOnOff, &gONandOFFvalueToSend,
                      &xHigherPriorityTaskWoken);

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


//PB3 -> Jamming Button
void GPIO_PORTB_Handler(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    GPIO_PORTB_ICR_REG |= 1 << 3;

    //Lw al Jamming Flag = 1 ->
    //                          yeb2a ha3mel give lel semaohore bta3o
    //                          fa al task bta3ato hatetl3 mnn al blocked state w tesht8l
    if (gFlagWindowUpJamming == pdTRUE)
    {
        //ha7ot al jamming Condition 3lshan a3mel 3aleh check fel automatic
        //Lw = true -> yeb2a fy jamming
        //yeb2a lazem lama al jamming yekhlas sho8lo -> al automatic yo2f maykmlsh
        gJammingCondition = pdTRUE;
        xSemaphoreGiveFromISR(xBinarySemaphorJammingButton,
                              &xHigherPriorityTaskWoken);
    }

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


//PE3 -> Lock Button
void GPIO_PORTE_Handler(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    GPIO_PORTE_ICR_REG |= 1 << 3;

    gLOCKvalueToSend++;

    //send to the queue to the Task named vWindowLockHandlerTask,
    //use xQueueSendFromISR
    // clear the interrupt flag of PORTE
    /* sending to the queue may have unblocked a task - if it did and the
     unblocked task has a priority equal to or above the currently executing
     task then xHigherPriorityTaskWoken will have been set to pdTRUE and
     portEND_SWITCHING_ISR() will force a context switch to the newly unblocked
     higher priority task.

     NOTE: The syntax for forcing a context switch within an ISR varies between
     FreeRTOS ports. The portEND_SWITCHING_ISR() macro is provided as part of
     the Corte M3 port layer for this purpose. taskYIELD() must never be called
     from an ISR!, use portEND_SWITCHING_ISR, refer to FreeRTOS documentation.*/

    xQueueSendFromISR(xQueueLOCK, &gLOCKvalueToSend, &xHigherPriorityTaskWoken);

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


