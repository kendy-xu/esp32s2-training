#include "include.h"
#include "driver/uart.h"

#define EX_UART_NUM UART_NUM_1
#define TXD_PIN (GPIO_NUM_19)
#define RXD_PIN (GPIO_NUM_20)

#if defined(UART_MOCE_EVENT)
static const char *TAG = "uart_events";
#elif defined(UART_MODE_TASK)
static const char *TAG = "uart_task";
#endif

char* set_io_level = "DIGIO-LEVEL-SET";
char* read_io_level = "DIGIO-LEVEL-GET";
char* read_adc_value = "ADC-GET";


void del_spare_space(char* p_str)
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

void uart1_rx_handler(uint8_t *ptr, uint16_t length)
{
    //char* set_io_h = "Hello, world!";
    uint16_t len;
    char* ack = "ACK";
    len = length+1;
    char str_rx[len];
    uint16_t i;
    for(i=0; i<length; i++)
    {
        str_rx[i] = *ptr++;        
    }

    str_rx[length] = '\0';

    del_spare_space(str_rx);
                         
    if(strcmp(set_io_level, str_rx) == 0)
    {
        ESP_LOGI(TAG, "UART[%d] DATA TX: %d bytes", EX_UART_NUM, sizeof(ack)-1);
        uart_write_bytes(EX_UART_NUM, ack, sizeof(ack)-1);                
        printf("Uart1 send ack. \n");
    }
    else
    {
        uart_write_bytes(EX_UART_NUM, str_rx, strlen(str_rx));//sizeof(str_rx)-1);//
        ESP_LOGI(TAG, "UART[%d] DATA TX: %d bytes", EX_UART_NUM, strlen(str_rx));//sizeof(str_rx)-1);//
    }    
}

#if defined(UART_MODE_ISR)
static const int RX_BUF_SIZE = 128;

//中断服务函数放在IRAM中执行，所以在这里定义的时候要添加IRAM_ATTR
static void IRAM_ATTR uart1_irq_handler(void *arg) 
{       
    volatile uart_dev_t *uart = &UART1;
    static uint8_t uart1RxBuf[64];
    static uint8_t uart1RxCount;
    uint8_t recSize = 0;
    uart->int_clr.rxfifo_full = 1;
    uart->int_clr.frm_err = 1;
    if(uart->int_st.rxfifo_tout) //检查是否产生超时中断
	{
		uart->int_clr.rxfifo_tout = 1;
		recSize = uart->status.rxfifo_cnt;
		if(recSize != 0)
		{
			while(uart->status.rxfifo_cnt)
			{
				uart1RxBuf[uart1RxCount++] = uart->ahb_fifo.rw_byte; 
                if(uart1RxCount >= 64)
                {
                    uart1RxCount = 0;
                }
			}
		}
	}   
}

void uart_init(void) 
{
    //串口基本参数的配置
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(EX_UART_NUM, &uart_config);
    //设置串口使用的引脚
    uart_set_pin(EX_UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    //这里是关键点，必须要先安装驱动(uart_driver_install)，再把中断服务给释放掉
    uart_driver_install(EX_UART_NUM, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_isr_free(EX_UART_NUM);
    //重新注册中断服务函数
    uart_isr_handle_t handle;
    uart_isr_register(EX_UART_NUM, uart1_irq_handler, NULL, ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM, &handle);
    //使能串口接收中断
    uart_enable_rx_intr(EX_UART_NUM);
    uart_set_rx_timeout(EX_UART_NUM,10); //配置接收超时中断时间，单位为按照当前波特率传输1个bytes的时间
}
#elif defined(UART_MOCE_EVENT)
#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/
#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
static QueueHandle_t uart1_queue;

void uart_init(void)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    //Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart1_queue, 0);
    uart_param_config(EX_UART_NUM, &uart_config);

    //Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);
    //Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(EX_UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
    //Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(EX_UART_NUM, 20);
}

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
    while(1)//for(;;) 
    {
        //Waiting for UART event.
        if(xQueueReceive(uart1_queue, (void * )&event, (portTickType)portMAX_DELAY)) 
        {
            bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    ESP_LOGI(TAG, "UART[%d] DATA RX: %d bytes", EX_UART_NUM, event.size);
                    uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                    //ESP_LOGI(TAG, "UART[%d] DATA TX: %d bytes", EX_UART_NUM, event.size);
                    //uart_write_bytes(EX_UART_NUM, (const char*) dtmp, event.size);
                    uart1_rx_handler(dtmp, event.size);
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart1_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart1_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);
                    int pos = uart_pattern_pop_pos(EX_UART_NUM);
                    ESP_LOGI(TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                    if (pos == -1) 
                    {
                        // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                        // record the position. We should set a larger queue size.
                        // As an example, we directly flush the rx buffer here.
                        uart_flush_input(EX_UART_NUM);
                    } 
                    else 
                    {
                        uart_read_bytes(EX_UART_NUM, dtmp, pos, 100 / portTICK_PERIOD_MS);
                        uint8_t pat[PATTERN_CHR_NUM + 1];
                        memset(pat, 0, sizeof(pat));
                        uart_read_bytes(EX_UART_NUM, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                        ESP_LOGI(TAG, "read data: %s", dtmp);
                        ESP_LOGI(TAG, "read pat : %s", pat);
                    }
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

#elif defined(UART_MODE_TASK)
static const int RX_BUF_SIZE = 1024;
void uart_init(void) 
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(EX_UART_NUM, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(EX_UART_NUM, &uart_config);
    uart_set_pin(EX_UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(EX_UART_NUM, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

static void tx_task(void *arg)
{
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1) 
    {
        sendData(TX_TASK_TAG, "Hello, Esp32-S2 world !");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) 
    {
        const int rxBytes = uart_read_bytes(EX_UART_NUM, data, RX_BUF_SIZE, 25 / portTICK_RATE_MS);
        if (rxBytes > 0) 
        {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
            uart1_rx_handler(data, rxBytes);
        }
    }
    free(data);
}

#endif

void Creat_uart_task(void)
{
    uart_init();
    
    #if defined(UART_MODE_ISR) 

    #elif defined(UART_MOCE_EVENT)    
    //Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
    #elif defined(UART_MODE_TASK)
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    //xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
    #endif
}
