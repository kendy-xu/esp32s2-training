#include "include.h"


//================================================================
void app_main(void)
{	
	printf("ESP32-S2 KDP K90 POWER! \n");	
	GPIO_LED_init();   
  	GPIO_Key_init();	
	Creat_uart_task();
	KeyInt_init();		
	ledc_pwm_init();
	CreatLedTimer();
	CreatKeyScanTimer();
	CreatKeyIntTimer();
	CreatKeyQueue();	

	ledc_pwm_app();
	
	while(1)
	{
	    ReceiveKeyQueue();
	    vTaskDelay(100 / portTICK_PERIOD_MS);                              /* 延时100ms*/
	}       
}
//================================================================
