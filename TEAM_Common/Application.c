/**
 * \file
 * \brief Main application file
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This provides the main application entry point.
 */

#include "Platform.h"
#include "Application.h"
#include "Event.h"
#include "LED.h"
#include "WAIT1.h"
#include "CS1.h"
#include "KeyDebounce.h"
#include "CLS1.h"
#include "KIN1.h"

#if PL_CONFIG_HAS_KEYS
  #include "Keys.h"
#endif
#if PL_CONFIG_HAS_SHELL
  #include "CLS1.h"
  #include "Shell.h"
  #include "RTT1.h"
#endif
#if PL_CONFIG_HAS_BUZZER
  #include "Buzzer.h"
#endif
#if PL_CONFIG_HAS_RTOS
  #include "FRTOS1.h"
  #include "RTOS.h"
#endif
#if PL_CONFIG_HAS_QUADRATURE
  #include "Q4CLeft.h"
  #include "Q4CRight.h"
#endif
#if PL_CONFIG_HAS_MOTOR
  #include "Motor.h"
#endif
#if PL_CONFIG_BOARD_IS_ROBO_V2
  #include "PORT_PDD.h"
#endif
#if PL_CONFIG_HAS_LINE_FOLLOW
  #include "LineFollow.h"
#endif
#if PL_CONFIG_HAS_LCD_MENU
  #include "LCD.h"
#endif
#if PL_CONFIG_HAS_SNAKE_GAME
  #include "Snake.h"
#endif
#if PL_CONFIG_HAS_REFLECTANCE
  #include "Reflectance.h"
#endif
#include "Sumo.h"

#if PL_CONFIG_HAS_EVENTS

static void BtnMsg(int btn, const char *msg) {
#if PL_CONFIG_HAS_SHELL
  #if PL_CONFIG_HAS_SHELL_QUEUE
    uint8_t buf[48];

    UTIL1_strcpy(buf, sizeof(buf), "Button pressed: ");
    UTIL1_strcat(buf, sizeof(buf), msg);
    UTIL1_strcat(buf, sizeof(buf), ": ");
    UTIL1_strcatNum32s(buf, sizeof(buf), btn);
    UTIL1_strcat(buf, sizeof(buf), "\r\n");
    SHELL_SendString(buf);
  #else
    CLS1_SendStr("Button pressed: ", CLS1_GetStdio()->stdOut);
    CLS1_SendStr(msg, CLS1_GetStdio()->stdOut);
    CLS1_SendStr(": ", CLS1_GetStdio()->stdOut);
    CLS1_SendNum32s(btn, CLS1_GetStdio()->stdOut);
    CLS1_SendStr("\r\n", CLS1_GetStdio()->stdOut);
  #endif
#endif
}

void APP_EventHandler(EVNT_Handle event) {
  switch(event) {
  case EVNT_STARTUP:
  {
      int i;
      for (i=0;i<5;i++) {
        LED1_Neg();
        WAIT1_Waitms(50);
      }
      LED1_Off();
/* Startup for ROBO ONLY */
#if	PL_CONFIG_BOARD_IS_ROBO
      BUZ_PlayTune(BUZ_THREE_BEEP);
#endif

/* Startup for REMOTE ONLY */
#if PL_CONFIG_BOARD_IS_REMOTE
#endif
  }
    break;
  case EVNT_LED_HEARTBEAT:
    LED2_Neg();
    break;
#if PL_CONFIG_NOF_KEYS>=1
  case EVNT_SW1_PRESSED:
    BtnMsg(1, "pressed");
     break;
  case EVNT_SW1_LPRESSED:
    BtnMsg(1, "long pressed");
     break;
  case EVNT_SW1_RELEASED:
    BtnMsg(1, "released");
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=2
  case EVNT_SW2_PRESSED:
    BtnMsg(2, "pressed");
     break;
  case EVNT_SW2_LPRESSED:
    BtnMsg(2, "long pressed");
     break;
  case EVNT_SW2_RELEASED:
    BtnMsg(2, "released");
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=3
  case EVNT_SW3_PRESSED:
    BtnMsg(3, "pressed");
     break;
  case EVNT_SW3_LPRESSED:
    BtnMsg(3, "long pressed");
     break;
  case EVNT_SW3_RELEASED:
    BtnMsg(3, "released");
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=4
  case EVNT_SW4_PRESSED:
    BtnMsg(4, "pressed");
     break;
  case EVNT_SW4_LPRESSED:
    BtnMsg(4, "long pressed");
     break;
  case EVNT_SW4_RELEASED:
    BtnMsg(4, "released");
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=5
  case EVNT_SW5_PRESSED:
    BtnMsg(5, "pressed");
     break;
  case EVNT_SW5_LPRESSED:
    BtnMsg(5, "long pressed");
     break;
  case EVNT_SW5_RELEASED:
    BtnMsg(5, "released");
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=6
  case EVNT_SW6_PRESSED:
    BtnMsg(6, "pressed");
     break;
  case EVNT_SW6_LPRESSED:
    BtnMsg(6, "long pressed");
     break;
  case EVNT_SW6_RELEASED:
    BtnMsg(6, "released");
     break;
#endif
#if PL_CONFIG_NOF_KEYS>=7
  case EVNT_SW7_PRESSED:
    BtnMsg(7, "pressed");
     break;
  case EVNT_SW7_LPRESSED:
    BtnMsg(7, "long pressed");
     break;
  case EVNT_SW7_RELEASED:
    BtnMsg(7, "released");
     break;
#endif
    default:
      break;
   } /* switch */
}
#endif /* PL_CONFIG_HAS_EVENTS */

#if PL_CONFIG_HAS_MOTOR /* currently only used for robots */
static const KIN1_UID RoboIDs[] = {
  /* 0: L20, V2 */ {{0x00,0x03,0x00,0x00,0x67,0xCD,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 1: L21, V2 */ {{0x00,0x05,0x00,0x00,0x4E,0x45,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 2: L4, V1  */ {{0x00,0x0B,0xFF,0xFF,0x4E,0x45,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x24}},
  /* 3: L23, V2 */ {{0x00,0x0A,0x00,0x00,0x67,0xCD,0xB8,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 4: L11, V2 */ {{0x00,0x19,0x00,0x00,0x67,0xCD,0xB9,0x11,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 5: L5, V2 */  {{0x00,0x38,0x00,0x00,0x67,0xCD,0xB5,0x41,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 6: L3, V1 */  {{0x00,0x33,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x0A}},
  /* 7: L1, V1 */  {{0x00,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x25}},
};
#endif

static void APP_AdoptToHardware(void) {
  KIN1_UID id;
  uint8_t res;

  res = KIN1_UIDGet(&id);
  if (res!=ERR_OK) {
    for(;;); /* error */
  }
#if PL_CONFIG_HAS_MOTOR
  if (KIN1_UIDSame(&id, &RoboIDs[0])) { /* L20 */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert left motor */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE); /* invert left motor */
  } else if (KIN1_UIDSame(&id, &RoboIDs[1])) { /* V2 L21 */
    /* no change needed */
  } else if (KIN1_UIDSame(&id, &RoboIDs[2])) { /* V1 L4 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* revert left motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[3])) { /* L23 */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert left motor */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE); /* invert left motor */
  } else if (KIN1_UIDSame(&id, &RoboIDs[4])) { /* L11 */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[5])) { /* L5, V2 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE); /* invert right motor */
    (void)Q4CRight_SwapPins(TRUE);
  } else if (KIN1_UIDSame(&id, &RoboIDs[6])) { /* L3, V1 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert right motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(TRUE);
#endif
  } else if (KIN1_UIDSame(&id, &RoboIDs[7])) { /* L1, V1 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert right motor */
#if PL_CONFIG_HAS_QUADRATURE
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(TRUE);
#endif
  }
#endif
#if PL_CONFIG_HAS_QUADRATURE && PL_CONFIG_BOARD_IS_ROBO_V2
  /* pull-ups for Quadrature Encoder Pins */
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 11, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 11, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 16, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 16, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 17, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 17, PORT_PDD_PULL_ENABLE);
#endif
}

static void BlinkyTask (void *pvParameters) {
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;) {
		LED1_Neg();
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
	}
}


static void MyAppTask(void *pvParam) {
	  for(;;)
	    {
	#if PL_CONFIG_HAS_DEBOUNCE
	    KEYDBNC_Process();
	#else
	    KEY_Scan(); /* scan keys and set events */
	#endif
	    EVNT_HandleEvent(APP_EventHandler, TRUE);
	    vTaskDelay(pdMS_TO_TICKS(100));
	    }
}

void APP_Start(void) {
  PL_Init();
  APP_AdoptToHardware();
  __asm volatile("cpsie i"); /* enable interrupts */

  EVNT_SetEvent(EVNT_STARTUP);						// set startup event (5x LED1 + BUZ)

/*##################create Blinky task###################*/
  /* elaborate task creation method*/
  BaseType_t res;
  xTaskHandle tskHndl;
  /* Start Blinky Task */
  res =   xTaskCreate(BlinkyTask,     /* function */
      "BlinkyTask",                   /* Kernel awarness name */
      configMINIMAL_STACK_SIZE+100,       /* stack in Bytes*/
      (void*) NULL,                   /* task parameter */
      tskIDLE_PRIORITY,               /* priority */
      &tskHndl                        /* handle */
      );
  if (res != pdPASS) {/*Error Handling*/}
/*####################end Blinky task####################*/


/*##################create App task###################*/
  /* elegant task creation method */
  if (xTaskCreate(MyAppTask, "App", 100, NULL,
      1, NULL) != pdPASS)
  {/*Error Handling*/}
/*####################end App task####################*/


 vTaskStartScheduler(); /* no code below this will be executed as long scheduler is running */

  for(;;) {

	  /*Key process routine for Polling*/
	  //KEY_Scan();

	  /*Event Handler for all events*/
	  //EVNT_HandleEvent(APP_EventHandler,TRUE);

	  //KEY_Scan();

	  //if (KEY1_Get()) {
		//  WAIT1_Waitms(50); /* simple debounce */
		//  if (KEY1_Get()) { /* still pressed? */
		//	  cnt = 0;
		//	  while(KEY1_Get()) {
		//		  WAIT1_Waitms(1);
		//		  cnt++; /* measure time */
		//	  } /* wait until released */
		//	  if (cnt<=1000) { /* short press*/
		//		  EVNT_SetEvent(EVNT_SW1_PRESSED);
		//	  } else { /* long press*/
		//		  EVNT_SetEvent(EVNT_SW1_PRESSED);
		//	  }
		//  }
	 // }
	//  EVNT_HandleEvent(APP_EventHandler,TRUE);


  }
}


