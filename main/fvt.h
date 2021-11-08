
#ifndef FVT_H
#define FVT_H

#define GPIO_HWT_NTC_ADC       1
#define GPIO_HWT_LEVEL_ADC     2
#define GPIO_CWT_LEVEL_ADC     3
#define GPIO_PRESSURE_ADC      4
#define GPIO_BUTTON2_ADC       5
#define GPIO_BUTTON1_ADC       6
#define GPIO_CW_ENCODER        7
#define GPIO_DOOR_SWITCH       8
#define GPIO_CW_ENCODER_LED    9
#define GPIO_CWT_LED          10
#define GPIO_RELAY_PARALLEL   11
#define GPIO_MASTER_RELAY     12
#define GPIO_LEDS_RCK_SS      13
#define GPIO_TRIAC_PARALLEL   14
#define GPIO_USB_D_N          19
#define GPIO_USB_D_P          20
#define GPIO_LEDS_G           21
#define GPIO_LEDS_SCK_SRCK    17 //27
#define GPIO_EEPROM_WP        18 //28
#define GPIO_LEDS_CLR         39 //29
#define GPIO_EEPROM_SCL       40 //30
#define GPIO_LEDS_SDO_SER_IN  41 //31
#define GPIO_EEPROM_SDA       42 //32
#define GPIO_HWT_LEVEL_PWR    33
#define GPIO_CWT_LEVEL_PWR    34
#define GPIO_FROTHER_ALLOWED  35
#define GPIO_FROTHER_ACTIVE   36
#define GPIO_AIR_PUMP_PWM     37
#define GPIO_CW_PUMP_PWM      38


typedef struct{
  uint8_t str_num;
  void* ptrToData;
  uint8_t length;
}command_items_t;


typedef enum{
  GPIO0_RELAY = GPIO_RELAY_PARALLEL,
  GPIO1_TRAIC = GPIO_TRIAC_PARALLEL,
  GPIO2_MAP = GPIO_LEDS_SDO_SER_IN,
  GPIO3_MAP = GPIO_LEDS_SCK_SRCK,
  GPIO4_CWT_PROBE = GPIO_CWT_LEVEL_PWR,
  GPIO5_HWT_PROBE = GPIO_HWT_LEVEL_PWR,
  GPIO6_DOOR_SW = GPIO_DOOR_SWITCH,
  GPIO7_ENC_EN = GPIO_CW_ENCODER_LED,
  GPIO8_ENC_STATE = GPIO_CW_ENCODER,
  GPIO9_EEPROM_EN = GPIO_EEPROM_WP,
  GPIO10_MAP = GPIO_LEDS_G,
  GPIO11_LED = GPIO_LEDS_RCK_SS //GPIO_CWT_LED
}gpio_map_t;

typedef enum{
  READ,
  WRITE
}rw_access_t;

typedef struct{
  gpio_map_t gpio_pin;  
  void* ptrToData;
  uint8_t length;
  rw_access_t access;
}gpio_items_t;

typedef enum{
  NO_ACTION,
  ACK_OK,
  PARAMETERS_ERROR,
  READ_ONLY_ALART,
  READ_SUCCESS
}cmd_respo_inf_t;

#define HWT_NTC_ADC    ADC_CHANNEL_0
#define HWT_LEVEL_ADC  ADC_CHANNEL_1
#define CWT_LEVEL_ADC  ADC_CHANNEL_2
#define PRESSURE_ADC   ADC_CHANNEL_3
#define BUTTON2_ADC    ADC_CHANNEL_4
#define BUTTON1_ADC    ADC_CHANNEL_5

typedef enum{
    ADC_0 = BUTTON1_ADC,
    ADC_1 = BUTTON2_ADC,
    ADC_2 = PRESSURE_ADC,
    ADC_3 = CWT_LEVEL_ADC,
    ADC_4 = HWT_NTC_ADC,
    ADC_5 = HWT_LEVEL_ADC
}adc_map_t;


typedef struct{
	adc_map_t adc_ch;
	adc_unit_t  adc_unit;
	void* ptrToData;
}adc_items_t;

#define EE_ADDR 0x50


void Fvt_Gpio_init(void);

void FVT_ReceDataHandler(uint8_t *ptr, uint16_t length);
void Delete_extra_spaces(char* p_str);
bool Get_FVT_Receive_cmd(char* pstr, uint8_t* str_num, char* cmd_str);
void FVT_Command_handler(char* p_cmd, char* p_target, char* p_value);
void cmd_respo_information(cmd_respo_inf_t respo_inf);
void FVT_Command_Split(char* pstr);
bool Del_CR_LF_of_string(char* str, uint16_t len);
uint16_t StringToHexByte(char* str_in, uint8_t* ptr);

#endif