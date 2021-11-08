#ifndef LEDC_PWM_H
#define LEDC_PWM_H


void ledc_pwm_init(void);
void ledc_Channel_SetDuty(uint32_t pwm_freq, gpio_num_t gpio, uint8_t ch, uint8_t percent);


#endif
