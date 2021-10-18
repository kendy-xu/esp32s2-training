#include "include.h"


/* 定义LED闪烁定时器句柄*/
TimerHandle_t LED_Timer_Handle;
/* 声明定时器回调函数 */
void LED_Timer_Callback(TimerHandle_t xTimer);


//================================================================
void GPIO_init(const uint16_t GPIO_INDEX,
               gpio_mode_t GPI0_MODE,
							 bool pull_up_flag,
							 bool pull_down_flag,
							 GPIO_INT_TYPE GPIO_TYPE)
{
  /* 定义一个gpio配置结构体 */
	gpio_config_t gpio_config_structure;
	
	/* 初始化gpio配置结构体*/
	gpio_config_structure.pin_bit_mask = (1ULL << GPIO_INDEX);
	gpio_config_structure.mode = GPI0_MODE;                 
	gpio_config_structure.pull_up_en = pull_up_flag;        
	gpio_config_structure.pull_down_en = pull_down_flag;    
	gpio_config_structure.intr_type = GPIO_TYPE;            
	/* 根据设定参数初始化并使能 */  
	gpio_config(&gpio_config_structure);
}
//================================================================

//================================================================
void GPIO_LED_init(void)
{
	GPIO_init(V6OZ_8OZ_LED_GPIO,      //选择 V6OZ_8OZ_LED_GPIO
            GPIO_MODE_OUTPUT,        // 输出模式
						false,                   // 不上拉
						false,                   // 不下拉
						GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(V6OZ_8OZ_LED_GPIO, true);//输出高电平

	GPIO_init(V8OZ_10OZ_LED_GPIO,     //选择 V8OZ_10OZ_LED_GPIO
            GPIO_MODE_OUTPUT,        // 输出模式
						false,                   // 不上拉
						false,                   // 不下拉
						GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(V8OZ_10OZ_LED_GPIO, true);//输出低电平

	GPIO_init(V10OZ_12OZ_LED_GPIO,     //选择 V10OZ_12OZ_LED_GPIO
            GPIO_MODE_OUTPUT,        // 输出模式
						false,                   // 不上拉
						false,                   // 不下拉
						GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(V10OZ_12OZ_LED_GPIO, true);//输出低电平

	GPIO_init(DESCALE_LED_GPIO,     //选择 DESCALE_LED_GPIO
            GPIO_MODE_OUTPUT,        // 输出模式
						false,                   // 不上拉
						false,                   // 不下拉
						GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(DESCALE_LED_GPIO, true);//输出低电平

	GPIO_init(ADD_WATER_LED_GPIO,     //选择 ADD_WATER_LED_GPIO
            GPIO_MODE_OUTPUT,        // 输出模式
						false,                   // 不上拉
						false,                   // 不下拉
						GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(ADD_WATER_LED_GPIO, true);//输出低电平

	GPIO_init(CW_ENCODER_LED_GPIO,     //选择 CW_ENCODER_LED_GPIO
            GPIO_MODE_OUTPUT,        // 输出模式
						false,                   // 不上拉
						false,                   // 不下拉
						GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(CW_ENCODER_LED_GPIO, true);//输出低电平

	vTaskDelay(1000 / portTICK_PERIOD_MS);
	gpio_set_level(V6OZ_8OZ_LED_GPIO, false);
	gpio_set_level(V8OZ_10OZ_LED_GPIO, false);
	gpio_set_level(V10OZ_12OZ_LED_GPIO, false);
	gpio_set_level(DESCALE_LED_GPIO, false);
	gpio_set_level(CW_ENCODER_LED_GPIO, false);
}
//================================================================

//================================================================
/* 定时器回调函数 */
void LED_Timer_Callback(TimerHandle_t xTimer)
{
  Led_flash(ADD_WATER_LED_GPIO);
}
//================================================================

//================================================================
bool LedTimerCreat(uint16_t nms)
{	
	if(nms < 10) nms = 10; //不能小于10ms,否则会一直复位；
	LED_Timer_Handle = xTimerCreate((const char*)"LED Timer",   /* 软件定时器名称 */
			          (TickType_t )(nms / portTICK_PERIOD_MS),      /* 定时周期1000ms，单位为时钟节拍 */
				        (UBaseType_t)pdTRUE,                          /* 定时器模式，是否为周期定时模式 */
				        (void*)1,                                     /* 定时器ID号 */
				        (TimerCallbackFunction_t)LED_Timer_Callback); /*定时器回调函数 */
	if((LED_Timer_Handle != NULL))
	{
		printf("LED Timer Create success !!! \n");  //定时器创建成功
		return true;
	}
	else
	{
		printf("LED Timer Create failure !!! \n");  //定时器创建失败
		return false;
	}
}
//================================================================

//================================================================
void LedTimerStart(void)
{
	BaseType_t ret = 0;
	ret = xTimerStart(LED_Timer_Handle,0);	    // 创建成功，开启定时器
	if(ret == pdPASS)
	    printf("LED Timer Start OK. \n");           //定时器启动成功
	else
	    printf("LED Timer Start err. \n");          //定时器启动失败
}
//================================================================

//================================================================
void Led_flash(const uint16_t GPIO_INDEX)
{
	static bool LedOnOffFlag = false;
  LedOnOffFlag = !LedOnOffFlag;  
  //printf("Turning on the Add water LED\n");
  gpio_set_level(GPIO_INDEX, LedOnOffFlag);       
    
  /*    
    printf("Turning off the Add water LED\n");
    gpio_set_level(ADD_WATER_LED_GPIO, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    printf("Turning on the Add water LED\n");
    gpio_set_level(ADD_WATER_LED_GPIO, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    */
}
//================================================================

//================================================================
void CreatLedTimer(void)
{
  if(LedTimerCreat(500))
	{
		LedTimerStart();
	}
}
//================================================================

//================================================================
void CloseLedTimer(void)
{
  if( xTimerIsTimerActive(LED_Timer_Handle) != pdFALSE )//定时器在激活状态
	{			
		if(xTimerStop(LED_Timer_Handle,0) == pdPASS)
		{
			printf("Led Timer stop OK! \n");
		}
		gpio_set_level(ADD_WATER_LED_GPIO, false);
	}
}
//================================================================

//================================================================
void OpenLedTimer(void)
{
  if( xTimerIsTimerActive(LED_Timer_Handle) == pdFALSE )//定时器在非激活状态
	{
		xTimerChangePeriod(LED_Timer_Handle, 200 / portTICK_PERIOD_MS, 0);//周期改为200ms
		LedTimerStart();
		gpio_set_level(ADD_WATER_LED_GPIO, true);
	}
}
//================================================================

//================================================================
void On_Off_Led(void)
{
  if( xTimerIsTimerActive(LED_Timer_Handle) == pdFALSE )//定时器在非激活状态
	{
		Led_flash(ADD_WATER_LED_GPIO);
	}
}
//================================================================