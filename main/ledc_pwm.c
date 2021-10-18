#include "include.h"
#include "driver/ledc.h"

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE// LEDC_HIGH_SPEED_MODE //

#define LEDC_LS_CH0_GPIO       (37) //PIN41-GPIO37
#define LEDC_LS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_LS_CH1_GPIO       (33) //PIN37-GPIO33 used for test
#define LEDC_LS_CH1_CHANNEL    LEDC_CHANNEL_0

#define LEDC_CH_NUM            (2)
#define LECC_PWM_FREQ          (11435)
#define LEDC_DUTY_RES          LEDC_TIMER_10_BIT


/*
 * Prepare and set configuration of timers
 * that will be used by LED Controller
*/

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_DUTY_RES,     // resolution of PWM duty
        .freq_hz = LECC_PWM_FREQ,             // frequency of PWM signal
        .speed_mode = LEDC_LS_MODE,           // timer mode
        .timer_num = LEDC_LS_TIMER,           // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
    };

    ledc_channel_config_t ledc_channel[LEDC_CH_NUM] = 
    {
        {
            .channel    = LEDC_LS_CH0_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH0_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
        },    
        {
            .channel    = LEDC_LS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH1_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
        },
    };


void ledc_pwm_init(void)
{
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    uint8_t ch;    
    // Set LED Controller with previously prepared configuration
    for (ch = 0; ch < LEDC_CH_NUM; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    ledc_fade_func_install(0);
    ledc_Channel_SetDuty(60); //duty set 60%
}

void ledc_Channel_SetDuty(uint8_t percent)
{
    uint8_t ch;
    uint16_t duty;
    uint16_t period;
    period = pow(2,LEDC_DUTY_RES);
    duty =  period * percent / 100;
    printf("LEDC PWM FREQ: %dHz.\n", ledc_timer.freq_hz);
    printf("LEDC period:%d, set duty = %d, %d percent.\n", period, duty, percent);
    for (ch = 0; ch < LEDC_CH_NUM; ch++) {
        ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, duty);
        ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
    }
}

void ledc_pwm_app(void)
{    
    ;
    
}