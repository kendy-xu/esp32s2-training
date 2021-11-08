#include "include.h"
#include <stdint.h>

const char enter_fvt_cmd[] = "????????";
const char set_io_level[] = "DIGIO-LEVEL-SET";
const char get_io_level[] = "DIGIO-LEVEL-GET";
const char get_adc_value[] = "ADC-GET";
const char set_pwm[] = "DIGIO-PWM-SET";
const char eeprom_wirte[] = "EEPROM-WRITE";
const char eeprom_read[] = "EEPROM-READ";
const char eeprom_read_all[] = "EEPROM-READE-ALL";
const char iic_read[] = "I2C-MEM-READ";
const char iic_write[] = "I2C-MEM-WRITE";
const char master_rly_set[] = "MASTER-RELAY-SET";
const char reset_chip[] = "RESET";
const char help[] = "HELP";

const command_items_t TAB_FVT_CMD[] = {
//str_num   ptrToData               lenght               
    {1,     (void*)&enter_fvt_cmd,  sizeof(enter_fvt_cmd)  },
    {3,     (void*)&set_io_level,   sizeof(set_io_level)   },
    {2,     (void*)&get_io_level,   sizeof(get_io_level)   },
    {2,     (void*)&get_adc_value,  sizeof(get_adc_value)  },
    {3,     (void*)&set_pwm,        sizeof(set_pwm)        },
    {3,     (void*)&eeprom_wirte,   sizeof(eeprom_wirte)   },
    {2,     (void*)&eeprom_read,    sizeof(eeprom_read)    },
    {1,     (void*)&eeprom_read_all,sizeof(eeprom_read_all)},
    {3,     (void*)&iic_read,       sizeof(iic_read)       }, 
    {3,     (void*)&iic_write,      sizeof(iic_write)      },
    {2,     (void*)&master_rly_set, sizeof(master_rly_set) },
    {1,     (void*)&reset_chip,     sizeof(reset_chip)     },
    {1,     (void*)&help,           sizeof(help)           }
};
static uint8_t SizeOfFvtTable = sizeof(TAB_FVT_CMD) / sizeof(TAB_FVT_CMD[0]);

const char DIO_0[] = "DIO_0";
const char DIO_1[] = "DIO_1";
const char DIO_2[] = "DIO_2";
const char DIO_3[] = "DIO_3";
const char DIO_4[] = "DIO_4";
const char DIO_5[] = "DIO_5";
const char DIO_6[] = "DIO_6";
const char DIO_7[] = "DIO_7";
const char DIO_8[] = "DIO_8";
const char DIO_9[] = "DIO_9";
const char DIO_10[] = "DIO_10";
const char DIO_11[] = "DIO_11";

const gpio_items_t TAB_GPIO_MAP[] = {
  // gpio_pin        ptrToData          lenght       access
  { GPIO0_RELAY,     (void*)&DIO_0,  sizeof(DIO_0),  WRITE},         
  { GPIO1_TRAIC,     (void*)&DIO_1,  sizeof(DIO_1),  WRITE},  
  { GPIO2_MAP,       (void*)&DIO_2,  sizeof(DIO_2),  WRITE},
  { GPIO3_MAP,       (void*)&DIO_3,  sizeof(DIO_3),  WRITE}, 
  { GPIO4_CWT_PROBE, (void*)&DIO_4,  sizeof(DIO_4),  WRITE},        
  { GPIO5_HWT_PROBE, (void*)&DIO_5,  sizeof(DIO_5),  WRITE},   
  { GPIO6_DOOR_SW,   (void*)&DIO_6,  sizeof(DIO_6),   READ},
  { GPIO7_ENC_EN,    (void*)&DIO_7,  sizeof(DIO_7),  WRITE}, 
  { GPIO8_ENC_STATE, (void*)&DIO_8,  sizeof(DIO_8),   READ},       
  { GPIO9_EEPROM_EN, (void*)&DIO_9,  sizeof(DIO_9),  WRITE},      
  { GPIO10_MAP,      (void*)&DIO_10, sizeof(DIO_10), WRITE}, 
  { GPIO11_LED,      (void*)&DIO_11, sizeof(DIO_11), WRITE}  
};                             

static uint8_t SizeOfMapTable = sizeof(TAB_GPIO_MAP) / sizeof(TAB_GPIO_MAP[0]);

char adc_0[] = "ADC_0";
char adc_1[] = "ADC_1";
char adc_2[] = "ADC_2";
char adc_3[] = "ADC_3";
char adc_4[] = "ADC_4";
char adc_5[] = "ADC_5";

const adc_items_t TAB_ADC_MAP[] = {
  // adc_ch          adc_unit    ptrToData    
  { ADC_0,     ADC_UNIT_1, (void*)&adc_0},         
  { ADC_1,     ADC_UNIT_1, (void*)&adc_1},
  { ADC_2,     ADC_UNIT_1, (void*)&adc_2},
  { ADC_3,     ADC_UNIT_1, (void*)&adc_3}, 
  { ADC_4,     ADC_UNIT_1, (void*)&adc_4},        
  { ADC_5,     ADC_UNIT_1, (void*)&adc_5}
 };                             

static uint8_t SizeOfAdcMapTab = sizeof(TAB_ADC_MAP) / sizeof(TAB_ADC_MAP[0]);


void FVT_ReceDataHandler(uint8_t *ptr, uint16_t length)
{
    //char* set_io_h = "Hello, world!";
    static uint16_t rx_counter = 0;
    static char command_receive_buf[256] = {0}; 
    uint8_t temp[8];   
    char temp_str[256];
    int i = 0;
    memset(temp_str, 0, sizeof(temp_str));
    memset(temp, 0, sizeof(temp));
    while(length)
    {        
        temp_str[i] = *ptr;
        temp[i] = *ptr;
        i++;
        printf("command_receive_buf[%d]:0x%02x ASCII:%s\n", rx_counter, *ptr, temp_str);
        if(*ptr == '\b') {
            if(rx_counter) rx_counter--;
            command_receive_buf[rx_counter] = *ptr++;
        }
        else{
            command_receive_buf[rx_counter++] = *ptr++; 
        }               
        length--;
    }

    for(i = 0; i < rx_counter; i++){
        if(temp[i] == 0x1b && temp[i+1] == 0x5b && 
          (temp[i+2] == 0x41 || temp[i+2] == 0x42 ||
           temp[i+2] == 0x43 || temp[i+2] == 0x44)){
           rx_counter -= 3; 
        }
    }
   
    if(Del_CR_LF_of_string(command_receive_buf, rx_counter))
    {
        Delete_extra_spaces(command_receive_buf);//Delete extra Spaces
        printf("command_receive_buf:%s\n", command_receive_buf);
        if(strlen(command_receive_buf) > 2)
        {            
            FVT_Command_Split(command_receive_buf);
        }
        else
        {
            char* temp_str = ">";            
            uart_write_bytes(EX_UART_NUM, temp_str, strlen(temp_str)); 
        }
        rx_counter = 0;
        memset(command_receive_buf, 0 , sizeof(command_receive_buf));
    }
}

void FVT_Command_Split(char* pstr)
{
    uint16_t len = strlen(pstr) +1;
	char str_rx[len];
	memcpy(str_rx, pstr, len);
	printf("str_rx:%s\n", str_rx);
	
	char sub_str[strlen(str_rx)];
    char cmd_str[strlen(str_rx)];
    char target_str[strlen(str_rx)];
    char value_str[strlen(str_rx)];

    memset(sub_str, 0, sizeof(sub_str));
    memset(cmd_str, 0, sizeof(cmd_str));
    memset(target_str, 0, sizeof(target_str));
    memset(value_str, 0, sizeof(value_str));
    uint16_t i;
    uint16_t strings_counter = 0;
    uint16_t j = 0;
    uint8_t str_num = 3;
    bool bCmdState = false;
    for(i = 0; i<strlen(str_rx) + 1; i++)
    {
        sub_str[j] = str_rx[i]; 
        if(sub_str[j] == ' ')
        {
            if(strings_counter < (str_num - 1))
            {
                sub_str[j] = '\0';   
                strings_counter++;       
                if(strings_counter == 1){
                    bCmdState = Get_FVT_Receive_cmd(sub_str, &str_num, cmd_str);
                    printf("str_num = %d. \n", str_num);                    
                }
                else if(strings_counter == 2)     
                {                    
                    memcpy(target_str, sub_str, strlen(sub_str));                    
                }
                
                j = 0;
                //vTaskDelay(100 / portTICK_PERIOD_MS); 
                //uart_write_bytes(EX_UART_NUM, sub_str, strlen(sub_str));
                memset(sub_str, 0, sizeof(sub_str));                  
            }    
            else
            {
                j++;
            }                      
        }
        else if(str_rx[i] == '\0')
        {
            strings_counter++;
            //vTaskDelay(100 / portTICK_PERIOD_MS);
            printf("Receive strings number: %d. \n", strings_counter);
            if(strings_counter == 1)
            {
                bCmdState = Get_FVT_Receive_cmd(sub_str, &str_num, cmd_str);                    
            }
            
            if(bCmdState == true)
            {
            	if(strings_counter == 2)
            	{                                        
            	    memcpy(target_str, sub_str, strlen(sub_str));    
            	}
            	else if(strings_counter == 3)
            	{      
            	    memcpy(value_str, sub_str, strlen(sub_str)); 
            	}
            	
            	if(strings_counter < str_num )
            	{
            	    if(strings_counter == 1)
            	    {
            	    	if(str_num == 3)
            	    	{
            	    		char buf[] = "There is no target and parameters!\n>";
            	        	uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
            	    	}
            	    	else
            	    	{
            	    		char buf[] = "There is no target!\n>";
            	        	uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
            	    	}
            	    }
            	    else
            	    {
            	        char buf[] = "There is no parameter!\n>";
            	        uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
            	    }
            	}
            	else 
            	{//正确接收到上位机命令处理
                    FVT_Command_handler(cmd_str, target_str, value_str);
            	}   
            }

            //vTaskDelay(100 / portTICK_PERIOD_MS);
            //uart_write_bytes(EX_UART_NUM, sub_str, strlen(sub_str));            
        }      
        else
        {
            j++;
        }  
    }
}

bool Get_FVT_Receive_cmd(char* pstr, uint8_t* str_num, char* cmd_str)
{
    uint8_t i;
    bool b_state = false;
    uint16_t len = strlen(pstr)+1;
    char tempbuf2[len+1];
    memset(tempbuf2, 0, len);
    memcpy(tempbuf2, pstr, len);
    printf("tempbuf2:%s length:%d\n", tempbuf2, strlen(tempbuf2)); 

    for(i=0; i<SizeOfFvtTable; i++)
    {      
        char tempbuf1[TAB_FVT_CMD[i].length+1];        
        memset(tempbuf1, 0, TAB_FVT_CMD[i].length+1);         
        memcpy(tempbuf1, TAB_FVT_CMD[i].ptrToData, TAB_FVT_CMD[i].length+1);        
        printf("CMD_ID:%d %s length:%d\n", i, tempbuf1, strlen(tempbuf1));
        
        if(strcmp(tempbuf1, tempbuf2) == 0)  
        {                       
            *str_num = TAB_FVT_CMD[i].str_num;
            memcpy(cmd_str, pstr, TAB_FVT_CMD[i].length);
            b_state = true;
            printf("FVT_CMD_ID = %d. \n", i);
            break;
        }
    }
    
    if(b_state == false)
    {
        char buf[] = "Command error!\n>";
        uart_write_bytes(EX_UART_NUM, buf, strlen(buf));          
    }         

    return b_state;
}

void Delete_extra_spaces(char* p_str)
{
    uint16_t i = 0;
    uint16_t j = 0;    
    while(p_str[j] != '\0')
    {
        if(p_str[j] != ' ' || p_str[j+1] != ' ' )
        {
            p_str[i] = p_str[j];
            i++;
            j++;
        }
        else
        {
            p_str[i] = p_str[j+1];
            j++;
        }
    }

    if(p_str[i-1] == ' ')
    {//delete end space
        p_str[i-1] = '\0';
        p_str[i] = 0;
        j = i;
    }
    else
    {
        p_str[i] = '\0';
        j = i + 1;
    }

    if(p_str[0] == ' ')
    {//delete first space
        for(i=0; i<j; i++)
        {
            p_str[i] = p_str[i+1];
        }
    }
}


bool Del_CR_LF_of_string(char* str, uint16_t len)
{
    bool cr_lf_flag = false;
	uint16_t i;
	for(i = 0 ; i < len; i++)
    {
        if(str[i] == '[' && str[i+1] == 'D')
        {
            for(uint16_t j = 0; j < (i+2); j++)
            {
                str[j] = ' '; //fill space
            }
        }

        if(str[i] == '\r' && str[i+1] == '\n')
        {
            str[i] = '\0';
            str[i+1] = 0;
            str[i+2] = 0;
            cr_lf_flag = true;
            break;
        }
    }

    return cr_lf_flag;
}

void FVT_Command_handler(char* p_cmd, char* p_target, char* p_value)
{
	uint8_t i;
    uint16_t len1 = strlen(p_cmd)+1;
    uint16_t len2 = strlen(p_target)+1;
    uint16_t len3 = strlen(p_value)+1;
    char cmd_buf[len1];
    char target_buf[len2];
    char value_buf[len3];
    cmd_respo_inf_t respo_inf = NO_ACTION;
                 
    memcpy(cmd_buf, p_cmd, len1);
    memcpy(target_buf, p_target, len2);
    memcpy(value_buf, p_value, len3);
    printf("cmd_buf: %s\n", cmd_buf);
    printf("target_buf: %s\n", target_buf);
    printf("value_buf: %s\n", value_buf);
    
    static bool fvt_flag = false;
    if(strcmp(cmd_buf, enter_fvt_cmd) == 0)
    {
        if(fvt_flag == false)
        {
            fvt_flag = true;
            char buf[] = "TESTMODE/TESTVER 1.17/FWVER K80-90 MAIN PCBA 3.0.58\n>";
            uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
        }
        else
        {
            char buf[] = "Unsuport\n>";
            uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
        }
        return;
    }

    if(fvt_flag == false) return;
    
    if(strcmp(cmd_buf, set_io_level) == 0)
    {//write gpio   
        bool b_match = false;     
        for(i=0; i<SizeOfMapTable; i++)
        {
            if(strcmp(target_buf, TAB_GPIO_MAP[i].ptrToData) == 0)
            {
                b_match = true;
            	if(TAB_GPIO_MAP[i].access == WRITE)
            	{
                	char level_h[] = "H";  
                	char level_l[] = "L"; 
                	if(strcmp(value_buf, level_h) == 0)
                	{
                	    gpio_set_level((uint8_t)TAB_GPIO_MAP[i].gpio_pin, true);
                	    printf("set gpio%d high level. \n", TAB_GPIO_MAP[i].gpio_pin);
                	    respo_inf = ACK_OK;
                	}
                	else if(strcmp(value_buf, level_l) == 0)
                	{
                	   gpio_set_level((uint8_t)TAB_GPIO_MAP[i].gpio_pin, false); 
                	   printf("set gpio%d low level. \n", TAB_GPIO_MAP[i].gpio_pin);
                	   respo_inf = ACK_OK;
                	}
                    else
                    {
                        b_match = false;
                    }

                	break;
                }
                else
                {                	
                    respo_inf = READ_ONLY_ALART;
                }
            }
        }

        if(b_match == false)
        {
            respo_inf = PARAMETERS_ERROR;
        }
        
        //ESP_LOGI(TAG, "Command set IO level %s", value_buf);  
        printf("IO set level: %s. \n", value_buf);
    }
    else if(strcmp(cmd_buf, get_io_level) == 0)
    {//read gpio
        bool b_match = false;
        for(i=0; i<SizeOfMapTable; i++)
        {
            if(strcmp(target_buf, TAB_GPIO_MAP[i].ptrToData) == 0)
            {
                b_match = true;
            	bool level = gpio_get_level((uint8_t)TAB_GPIO_MAP[i].gpio_pin);
            	if(level == true)
            	{
            		char* ack = "H\n>";                    
                    uart_write_bytes(EX_UART_NUM, ack, strlen(ack));
            	}
            	else
            	{
            		char* ack = "L\n>";                    
                    uart_write_bytes(EX_UART_NUM, ack, strlen(ack));
            	}            	                                           
            	                
            	respo_inf = READ_SUCCESS;
            	break;
            }
        }   

        if(b_match == false)
        {
            respo_inf = PARAMETERS_ERROR;
        }     
    }    
    else if(strcmp(cmd_buf, get_adc_value) == 0)
    {
    	bool b_match = false;
    	for(i=0; i<SizeOfAdcMapTab; i++)
        {
            if(strcmp(target_buf, TAB_ADC_MAP[i].ptrToData) == 0)
            {
            	uint32_t test_ad_value[2];
                user_adc_sample(TAB_ADC_MAP[i].adc_unit, TAB_ADC_MAP[i].adc_ch, test_ad_value);
                printf("unit = %d, ch = %d. \n", TAB_ADC_MAP[i].adc_unit, TAB_ADC_MAP[i].adc_ch);
                char str_ad_value[5];
                itoa(test_ad_value[0], str_ad_value, 10);//int to decimal string
                uart_write_bytes(EX_UART_NUM, str_ad_value, strlen(str_ad_value));
                char buf[] = "\n";
                uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
                itoa(test_ad_value[1], str_ad_value, 10);//int to decimal string
                uart_write_bytes(EX_UART_NUM, str_ad_value, strlen(str_ad_value));
                uart_write_bytes(EX_UART_NUM, "mv\n>", 4);
            	//printf("Return %s value. \n", target_buf);
                
            	b_match = true;        		
            }
        }
                       
        if(b_match == false)
        {            
            respo_inf = PARAMETERS_ERROR;
        }
    }
    else if(strcmp(cmd_buf, set_pwm) == 0)
    {
    	bool b_match = false;
        char pwm_0[] = "PWM_0";
        char pwm_1[] = "PWM_1";
        uint16_t pwm_duty = atoi(value_buf);
        printf("Pwm dyty value: %d. \n", pwm_duty);
        if(strcmp(target_buf, pwm_0) == 0)
        {
            ledc_Channel_SetDuty(11435, GPIO_AIR_PUMP_PWM, LEDC_CHANNEL_0, pwm_duty / 10);
        	b_match = true;
        	respo_inf = ACK_OK;
        }
        else if(strcmp(target_buf, pwm_1) == 0)
        {
        	ledc_Channel_SetDuty(11435, GPIO_CW_PUMP_PWM, LEDC_CHANNEL_1, pwm_duty / 10);
        	b_match = true;
        	respo_inf = ACK_OK;
        }
        
        if(b_match == false)
        {
            respo_inf = PARAMETERS_ERROR;
        }
    }
    else if(strcmp(cmd_buf , master_rly_set) == 0)
    {
        bool b_match = false;
        char char_T[] = "T";
        char char_F[] = "F";
        if(strcmp(target_buf, char_T) == 0)
        {
            ledc_Channel_SetDuty(43, GPIO_MASTER_RELAY, LEDC_CHANNEL_2, 50);
        	b_match = true;
        	respo_inf = ACK_OK;
        }
        else if(strcmp(target_buf, char_F) == 0)
        {
        	ledc_Channel_SetDuty(43, GPIO_MASTER_RELAY, LEDC_CHANNEL_2, 0);
        	b_match = true;
        	respo_inf = ACK_OK;
        }
        
        if(b_match == false)
        {
            respo_inf = PARAMETERS_ERROR;
        }
    }
    else if(strcmp(cmd_buf , eeprom_wirte) == 0)
    {
        bool b_match = false;
        uint8_t ee_address;
        //ee_address = atoi(target_buf); //charactor to int
        StringToHexByte(target_buf, &ee_address);
        //printf("string length:%d ee_address: %d 0x%02x\n", strlen(target_buf), ee_address, ee_address);
        
        int len = strlen(value_buf);
        if(len){
            uint8_t array_tmp[len];
            uint16_t num = StringToHexByte(value_buf, array_tmp);
            printf("array_temp items:%d \n", num);   
            //uart_write_bytes(EX_UART_NUM, value_buf, len);          
            char tmp_str[4];
            for(int i = 0; i < num; i++)
            {
                //printf("%02x \r\n", array_tmp[i]); 
                itoa(array_tmp[i], tmp_str, 16);
                if(strlen(tmp_str) == 1) uart_write_bytes(EX_UART_NUM, "0", 1);
                uart_write_bytes(EX_UART_NUM, tmp_str, strlen(tmp_str));   
                if(((i+1) % 16) == 0){
                    uart_write_bytes(EX_UART_NUM, "\n", 1);
                }
                else{
                    uart_write_bytes(EX_UART_NUM, " ", 1);
                }
            }            
        }

        uart_write_bytes(EX_UART_NUM, "\n", 1);
        
        if(ee_address == EE_ADDR)
        {            
        	b_match = true;
        	respo_inf = ACK_OK;
        }
        
        if(b_match == false)
        {
            respo_inf = PARAMETERS_ERROR;
        }
    }
    else if(strcmp(cmd_buf , eeprom_read) == 0)
    {
        bool b_match = false;
        uint8_t ee_address;        
        StringToHexByte(target_buf, &ee_address);
                        
        if(ee_address == EE_ADDR)
        {            
        	b_match = true;
        	//respo_inf = ACK_OK;
            char tmp_str[4];
            uint8_t ee_data = 0x39;
            itoa(ee_data, tmp_str, 16);
            if(strlen(tmp_str) == 1) uart_write_bytes(EX_UART_NUM, "0", 1);
            uart_write_bytes(EX_UART_NUM, tmp_str, strlen(tmp_str)); 
        }
        
        if(b_match == false)
        {
            respo_inf = PARAMETERS_ERROR;
        }
    }

    cmd_respo_information(respo_inf);
}

void cmd_respo_information(cmd_respo_inf_t respo_inf)
{
    if(respo_inf == ACK_OK)
    {
        char* ack = "ACK\n>";            
        uart_write_bytes(EX_UART_NUM, ack, strlen(ack));
    }
    else if(respo_inf == PARAMETERS_ERROR)
    {
        char buf[] = "Parameter error!\n>";
        uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
    }
    else if(respo_inf == READ_ONLY_ALART)
    {
        char buf[] = "This IO read only!\n>";
        uart_write_bytes(EX_UART_NUM, buf, strlen(buf));
    }
}

uint16_t StringToHexByte(char* str_in, uint8_t* ptr)
{
    bool  b_state = false;
    uint8_t temp = 0;
    uint16_t counter = 0;
    uint16_t hexByteNumber = 0;
    for(int i = 0; i<strlen(str_in); i++)
    {
        if(str_in[i] <= '9' && str_in[i] >= '0')
        {
            temp = str_in[i] - '0';            
            b_state = true;
        }
        else if(str_in[i] <= 'F' && str_in[i] >= 'A')
        {
            temp = str_in[i] - 'A' + 10;
            b_state = true;
        }
        else if(str_in[i] <= 'f' && str_in[i] >= 'a')
        {
            temp = str_in[i] - 'a' + 10;
            b_state = true;
        }

        if(b_state == true){
            
            if(counter % 2)
            {
                *ptr++ |= temp;
                hexByteNumber++;
            }
            else
            {
                if(str_in[i+1] == ' ' || str_in[i+1] == '\r' || str_in[i+1] == '\0')
                {
                    *ptr++ = temp;
                    counter++;
                    hexByteNumber++;
                }
                else
                {
                    *ptr = temp << 4;
                }                
            }
            counter++;   
            b_state = false;        
        }       
    }    
    return hexByteNumber;
}

void Fvt_Gpio_init(void)
{
    GPIO_init(GPIO_CW_ENCODER,      //选择 GPIO
            GPIO_MODE_INPUT,         // 输入模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断

    GPIO_init(GPIO_DOOR_SWITCH,     //选择 GPIO
            GPIO_MODE_INPUT,         // 输入模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断                

    GPIO_init(GPIO_CW_ENCODER_LED,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_CW_ENCODER_LED, false);//输出低电平

    GPIO_init(GPIO_CWT_LED,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_CWT_LED, false);//输出低电平
	
	GPIO_init(GPIO_RELAY_PARALLEL,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_RELAY_PARALLEL, false);//输出低电平
	
	GPIO_init(GPIO_MASTER_RELAY,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_MASTER_RELAY, false);//输出低电平
	
	GPIO_init(GPIO_LEDS_RCK_SS,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_LEDS_RCK_SS, false);//输出低电平
	
	GPIO_init(GPIO_TRIAC_PARALLEL,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_TRIAC_PARALLEL, false);//输出低电平
	
	
	GPIO_init(GPIO_LEDS_G,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_LEDS_G, false);//输出低电平
	
	GPIO_init(GPIO_LEDS_SCK_SRCK,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_LEDS_SCK_SRCK, false);//输出低电平
	
	GPIO_init(GPIO_EEPROM_WP,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_EEPROM_WP, false);//输出低电平
	
		
	GPIO_init(GPIO_LEDS_CLR,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_LEDS_CLR, false);//输出低电平
	
	GPIO_init(GPIO_EEPROM_SCL,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_EEPROM_SCL, false);//输出低电平
	
	GPIO_init(GPIO_LEDS_SDO_SER_IN,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_LEDS_SDO_SER_IN, false);//输出低电平
	
	
	GPIO_init(GPIO_EEPROM_SDA,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_EEPROM_SDA, false);//输出低电平
	
	
	GPIO_init(GPIO_HWT_LEVEL_PWR,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_HWT_LEVEL_PWR, false);//输出低电平
		
	GPIO_init(GPIO_CWT_LEVEL_PWR,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_CWT_LEVEL_PWR, false);//输出低电平
		
	GPIO_init(GPIO_FROTHER_ALLOWED,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_FROTHER_ALLOWED, false);//输出低电平
	
	GPIO_init(GPIO_FROTHER_ACTIVE,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_FROTHER_ACTIVE, false);//输出低电平
	
	GPIO_init(GPIO_AIR_PUMP_PWM,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_AIR_PUMP_PWM, false);//输出低电平
	
	GPIO_init(GPIO_CW_PUMP_PWM,     //选择 GPIO
            GPIO_MODE_INPUT_OUTPUT,  // 输入输出模式
			false,                   // 不上拉
			false,                   // 不下拉
			GPIO_PIN_INTR_DISABLE);  // 禁止中断
	gpio_set_level(GPIO_CW_PUMP_PWM, false);//输出低电平    
}
