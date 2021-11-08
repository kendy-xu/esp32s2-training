#include "include.h"


//================================================================
void app_main(void)
{	
	printf("ESP32-S2 KDP K90 POWER! \n");	
	//GPIO_LED_init();   
    //GPIO_Key_init();		
	//KeyInt_init();	
	//CreatLedTimer();
	//CreatKeyScanTimer();
	//CreatKeyIntTimer();
	//CreatKeyQueue();	
    Fvt_Gpio_init();
    vTaskDelay(250 / portTICK_PERIOD_MS);  
    Creat_uart_task();   
    user_adc_init();
    //ledc_pwm_init();	
	
	while(1)
	{
	    //ReceiveKeyQueue();        
	    vTaskDelay(100 / portTICK_PERIOD_MS);                              /* 延时100ms*/
	}       
}
//================================================================
