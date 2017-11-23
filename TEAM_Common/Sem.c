/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

#include "Platform.h" /* interface to the platform */
#if PL_CONFIG_HAS_SEMAPHORE
#include "FRTOS1.h"
#include "Sem.h"
#include "LED.h"
#include "Shell.h"


static xSemaphoreHandle sem = NULL;

static void vSlaveTask(void *pvParameters) {
  /*! \todo Implement functionality */
	unsigned char msg[] = "take";
	for (;;)
	{
		xSemaphoreTake(sem,500);
		sharedWarehouse(msg);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

}

static void vMasterTask(void *pvParameters) {
  /*! \todo send semaphore from master task to slave task */

	unsigned char msg[] = "give";

	for (;;)
	{
		sharedWarehouse(msg);
		xSemaphoreGive(sem);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


void sharedWarehouse(unsigned char* msg)
{
  SHELL_SendString("item: ");
  SHELL_SendString(msg);
  SHELL_SendString("\r\n");
}

void SEM_Deinit(void) {
}

/*! \brief Initializes module */
void SEM_Init(void) {
	  sem = xSemaphoreCreateBinary();

	  if (sem == NULL)
	  {
	    /* Failed! not enough memory?*/
	  } else {
	    /* The semaphore can now be used. Calling
	     * SemaphoreTake() on the semaphore  will fail
	     * until the semaphore first has been given */
	  }

	  //BaseType_t res;
	  //res =   xTaskCreate(vMasterTask,     /* function */
	  //    "MasterTask",                   /* Kernel awarness name */
	  //    configMINIMAL_STACK_SIZE+50,       /* stack in Bytes*/
	  //    sem,                   /* task parameter */
	  //    tskIDLE_PRIORITY,               /* priority */
	  //    NULL                        /* handle */
	  //    );
	  //if (res != pdPASS) {/*Error Handling*/}


	  //res =   xTaskCreate(vSlaveTask,     /* function */
	  //    "SlaveTask",                   /* Kernel awarness name */
	  //    configMINIMAL_STACK_SIZE+50,       /* stack in Bytes*/
	  //    sem,                   /* task parameter */
	  //    tskIDLE_PRIORITY,               /* priority */
	  //    NULL                        /* handle */
	  //    );
	  //if (res != pdPASS) {/*Error Handling*/}

}
#endif /* PL_CONFIG_HAS_SEMAPHORE */
