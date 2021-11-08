#ifndef INCLUDE_H
#define INCLUDE_H
//===============================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"

#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_attr.h"
#include "esp_adc_cal.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "hal\uart_ll.h"
#include "driver/adc.h"
#include "driver/ledc.h"

#include "string.h"
#include "uart.h"
#include "button.h"
#include "led.h"
#include "ledc_pwm.h"
#include "fvt.h"
#include "ad.h"

//================================================
#endif