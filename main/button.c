#include "include.h"

/* 定义按键扫描定时器句柄*/
TimerHandle_t KEY_Scan_Timer_Handle;
/* 声明定时器回调函数 */
void KEY_Scan_Timer_Callback(TimerHandle_t xTimer);

/* 定义按键定时器中断句柄*/
TimerHandle_t Key_Timer_Int_Handle;
/* 声明定时器回调函数 */
void KEY_Timer_Int_Callback(TimerHandle_t xTimer);

/* 定义一个按键值消息队列句柄 */
QueueHandle_t Key_Queue;

//================================================================
/* gpio中断处理函数*/
static void gpio_isr_handler(void* arg)
{
    xTimerResetFromISR(Key_Timer_Int_Handle,NULL);
}
//================================================================

//================================================================
void GPIO_Key_init(void)
{
  gpio_config_t gpio_config_structure;
	/* 初始化gpio配置结构体*/
	gpio_config_structure.pin_bit_mask = (1ULL << KEY_POWER_GPIO);  /* 选择gpio */
	gpio_config_structure.mode = GPIO_MODE_INPUT;                   /* 输入模式 */
	gpio_config_structure.pull_up_en = 1;                           /* 上拉 */
	gpio_config_structure.pull_down_en = 0;                         /* 不下拉 */
	gpio_config_structure.intr_type = GPIO_PIN_INTR_NEGEDGE;        /* 下降沿触发中断 */ 
	/* 根据设定参数初始化并使能 */  
	gpio_config(&gpio_config_structure);

	gpio_config_structure.pin_bit_mask = (1ULL << KEY_6_8OZ_GPIO);   /* 选择gpio */
	gpio_config_structure.mode = GPIO_MODE_INPUT;                    /* 输入模式 */
	gpio_config_structure.pull_up_en = 1;                            /* 上拉 */
	gpio_config_structure.pull_down_en = 0;                          /* 不下拉 */
	gpio_config_structure.intr_type = GPIO_PIN_INTR_DISABLE;         /* 禁止中断 */ 	
	/* 根据设定参数初始化并使能 */  
	gpio_config(&gpio_config_structure);

	gpio_config_structure.pin_bit_mask = (1ULL << KEY_8_10OZ_GPIO);  /* 选择gpio */
	gpio_config_structure.mode = GPIO_MODE_INPUT;                    /* 输入模式 */
	gpio_config_structure.pull_up_en = 1;                            /* 上拉 */
	gpio_config_structure.pull_down_en = 0;                          /* 不下拉 */
	gpio_config_structure.intr_type = GPIO_PIN_INTR_DISABLE;         /* 禁止中断 */ 	
	/* 根据设定参数初始化并使能 */  
	gpio_config(&gpio_config_structure);

	gpio_config_structure.pin_bit_mask = (1ULL << KEY_10_12OZ_GPIO); /* 选择gpio */
	gpio_config_structure.mode = GPIO_MODE_INPUT;                    /* 输入模式 */
	gpio_config_structure.pull_up_en = 1;                            /* 上拉 */
	gpio_config_structure.pull_down_en = 0;                          /* 不下拉 */
	gpio_config_structure.intr_type = GPIO_PIN_INTR_DISABLE;         /* 禁止中断 */ 	
	/* 根据设定参数初始化并使能 */  
	gpio_config(&gpio_config_structure);
	
	GPIO_init(DOOR_SW_GPIO,            //选择 DOOR_SW_GPIO
        	  GPIO_MODE_INPUT,         // 输入模式          
			  true,                    // 上拉
			  false,                   // 不下拉
			  GPIO_PIN_INTR_DISABLE);  // 禁止中断

}
//================================================================

//================================================================
bool KeyScanTimerCreat(uint16_t nms)
{
	if(nms < 10) nms = 10; //不能小于10ms,否则会一直复位；
	KEY_Scan_Timer_Handle = xTimerCreate((const char*)"KEY Scan Timer",  // 软件定时器名称
			          (TickType_t )(nms / portTICK_PERIOD_MS),               // 定时周期(不能低于10)10ms，单位为时钟节拍
				        (UBaseType_t)pdTRUE,                                   // 定时器模式，是否为周期定时模式
				        (void*)2,                                              // 定时器ID号
				        (TimerCallbackFunction_t)KEY_Scan_Timer_Callback);     //定时器回调函数 
	
	if((KEY_Scan_Timer_Handle != NULL))
	{
		printf("Key Scan Timer Create success !!! \n");  //定时器创建成功		
		return 1;
	}	    
	else
	{
		printf("Key Scan Timer Create failure !!! \n");  //定时器创建失败
		return 0;
	}	    	
}
//================================================================

//================================================================
void KeySeanTimerStart(void)
{
	BaseType_t ret = 0;
	ret = xTimerStart(KEY_Scan_Timer_Handle,0);	     //创建成功，开启定时器
	if(ret == pdPASS)
	    printf("Key Scan Timer Start OK. \n");           /* 定时器启动成功*/
	else
	    printf("Key Scan Timer Start err. \n");          /* 定时器启动失败*/
}
//================================================================

//================================================================

button_scan_t button6_8oz;
button_scan_t button8_10oz;
button_scan_t button10_12oz;
//================================================================
bool button_6_8oz_scan(void)
{
	return gpio_get_level(KEY_6_8OZ_GPIO);
}
//================================================================

//================================================================
bool button_8_10oz_scan(void)
{
	return gpio_get_level(KEY_8_10OZ_GPIO);
}
//================================================================

//================================================================
bool button_10_12oz_scan(void)
{
	return gpio_get_level(KEY_10_12OZ_GPIO);
}
//================================================================

//================================================================
void ButtonStateDetect(bool (*pfunc)(), button_scan_t* ptr)
{
	if((*pfunc)())
	{
		ptr->pressCounter = 0;
		if(ptr->ButtonState == BUTTON_PRESSED ||
		   ptr->ButtonState == BUTTON_DOWN)
		{
			ptr->ButtonState = BUTTON_UP;
		}
		else if(ptr->ButtonState == BUTTON_UP)
		{
			if(++ptr->releaseCounter >= 6)
			{
				ptr->releaseCounter = 0;
				ptr->ButtonState = BUTTON_RELEASED;
				if(ptr->pressFlag == true)
				{
					ptr->releaseFlag = true;
					ptr->Counter++;
				}				
			}			
		}
	}
	else
	{
		ptr->releaseCounter = 0;
		if(ptr->ButtonState == BUTTON_RELEASED ||
		   ptr->ButtonState == BUTTON_UP)
		{
			ptr->ButtonState = BUTTON_DOWN;
		}
		else if(ptr->ButtonState == BUTTON_DOWN)
		{
			if(++ptr->pressCounter >= 6)
			{
				ptr->pressCounter = 0;
				ptr->ButtonState = BUTTON_PRESSED;
				ptr->pressFlag = true;
			}			
		}
	}
}
//================================================================

//================================================================
void KEY_Scan_Timer_Callback(TimerHandle_t xTimer)
{
	ButtonStateDetect(button_6_8oz_scan, &button6_8oz);	
	ButtonStateDetect(button_8_10oz_scan, &button8_10oz);
	ButtonStateDetect(button_10_12oz_scan, &button10_12oz);

	if(button6_8oz.releaseFlag == true)
	{
		button6_8oz.pressFlag = false;
		button6_8oz.releaseFlag = false;
		printf("6oz_8oz button has pressed and release %d times. \n", button6_8oz.Counter);
		CloseLedTimer();	
		ledc_Channel_SetDuty(11435, GPIO_AIR_PUMP_PWM, LEDC_CHANNEL_0, 0);	
	}

	if(button8_10oz.releaseFlag == true)
	{
		button8_10oz.pressFlag = false;
		button8_10oz.releaseFlag = false;
		printf("8oz_10oz button has pressed and release %d times. \n", button8_10oz.Counter);		
		OpenLedTimer();		
		ledc_Channel_SetDuty(11435, GPIO_AIR_PUMP_PWM, LEDC_CHANNEL_0, 80);
	}

	if(button10_12oz.releaseFlag == true)
	{
		button10_12oz.pressFlag = false;
		button10_12oz.releaseFlag = false;
		printf("10oz_12oz button has pressed and release %d times. \n", button10_12oz.Counter);
		On_Off_Led();
		ledc_Channel_SetDuty(11435, GPIO_AIR_PUMP_PWM, LEDC_CHANNEL_0, 60);
	}
}
//================================================================

//================================================================
void KEY_Timer_Int_Callback(TimerHandle_t xTimer)
{
    static int key_times = 0;
    char msg[50];
		if(gpio_get_level(KEY_POWER_GPIO) == false)
		{
			key_times++;
   		printf("BOOT KEY have pressed. \n");
    	sprintf(msg,"BOOT KEY have pressed %d times.",key_times);
    	xQueueSendFromISR(Key_Queue, msg, NULL);
		}    
}
//================================================================

//================================================================
void CreatKeyScanTimer(void)
{
  if(KeyScanTimerCreat(10))
	{
		KeySeanTimerStart();
	}
}
//================================================================

//================================================================
void KeyInt_init(void)
{
  /* 开启gpio中断服务 */
	gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);    /* LEVEL1为最低优先级 */
	/* 设置GPIO的中断回调函数 */
	gpio_isr_handler_add(KEY_POWER_GPIO, gpio_isr_handler, (void*) KEY_POWER_GPIO);
}
//================================================================

//================================================================
void CreatKeyIntTimer(void)
{
	/* 创建按键检测定时器，50ms,单次定时*/
	Key_Timer_Int_Handle = xTimerCreate("Key Timer", (50/portTICK_PERIOD_MS), pdFALSE, (void*)1, KEY_Timer_Int_Callback);
	
}
//================================================================

//================================================================
void CreatKeyQueue(void)
{
	/* 创建按键检测消息队列 */
	Key_Queue = xQueueCreate((UBaseType_t )10,      /* 队列长度，这里是队列的项目数，即这个队列可以接受多少个消息*/
	                         (UBaseType_t )50);     /* 队列中每个消息的长度，单位为字节 */	
}

//================================================================

//================================================================
void ReceiveKeyQueue(void)
{
  char msg[50];
  if(xQueueReceive(Key_Queue, msg, portMAX_DELAY)) 
	{
	  printf("in app_main : %s \n", msg);
	}
}
//================================================================
