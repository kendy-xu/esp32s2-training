#ifndef LED_H
#define LED_H


#define V6OZ_8OZ_LED_GPIO    13
#define V8OZ_10OZ_LED_GPIO   11
#define V10OZ_12OZ_LED_GPIO  12
#define DESCALE_LED_GPIO     21
#define ADD_WATER_LED_GPIO   35
#define CW_ENCODER_LED_GPIO  36


void GPIO_init(const uint16_t GPIO_INDEX,
               gpio_mode_t GPI0_MODE,
							 bool pull_up_flag,
							 bool pull_down_flag,
							 GPIO_INT_TYPE GPIO_TYPE);
void GPIO_LED_init(void);               
bool LedTimerCreat(uint16_t nms);
void LedTimerStart(void);
void Led_flash(const uint16_t GPIO_INDEX);
void CreatLedTimer(void);
void CloseLedTimer(void);
void OpenLedTimer(void);
void On_Off_Led(void);


#endif