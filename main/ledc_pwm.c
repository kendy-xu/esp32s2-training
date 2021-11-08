#include "include.h"

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE// LEDC_HIGH_SPEED_MODE //

#define LEDC_LS_CH0_GPIO       GPIO_AIR_PUMP_PWM//(37) //PIN41-GPIO37
#define LEDC_LS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_LS_CH1_GPIO       GPIO_CW_PUMP_PWM//(33) //PIN37-GPIO33 used for test
#define LEDC_LS_CH1_CHANNEL    LEDC_CHANNEL_1

#define LEDC_CH_NUM            (2)
#define LECC_PWM_FREQ          (11435)
#define LEDC_DUTY_RES          LEDC_TIMER_10_BIT


/*
 * Prepare and set configuration of timers
 * that will be used by LED Controller
*/

void ledc_Channel_SetDuty(uint32_t pwm_freq, gpio_num_t gpio, uint8_t ch, uint8_t percent)
{
    uint16_t duty;
    uint16_t period;

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_DUTY_RES,     // resolution of PWM duty
        .freq_hz = pwm_freq,             // frequency of PWM signal
        .speed_mode = LEDC_LS_MODE,           // timer mode
        .timer_num = LEDC_LS_TIMER,           // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
    };

    ledc_channel_config_t ledc_channel = 
    {        
        .channel    = ch,
        .duty       = 0,
        .gpio_num   = gpio,
        .speed_mode = LEDC_LS_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_LS_TIMER        
    };

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

   
    // Set LED Controller with previously prepared configuration    
    ledc_channel_config(&ledc_channel);
   

    // Initialize fade service.
    ledc_fade_func_install(0);

    period = pow(2,LEDC_DUTY_RES);
    duty =  period * percent / 100;
    //printf("LEDC PWM FREQ: %dHz.\n", ledc_timer.freq_hz);
    //printf("LEDC period:%d, set duty = %d, %d percent.\n", period, duty, percent);
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

