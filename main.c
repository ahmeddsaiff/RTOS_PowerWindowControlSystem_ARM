#include "TasksImplmn/TasksImplementation.h"


int main()
{
    /* Setup the hardware for use with the Tiva C board. */
    prvSetupHardware();

    //Tasks , semph , mutex , queues , etc ..... Creations
    vMainCreations();

    /* Should never reach here!  If you do then there was not enough heap
     available for the idle task to be created. */
    for (;;);

}


/* Idle Hook API */
void vApplicationIdleHook(void)
{
    for (;;);
}
