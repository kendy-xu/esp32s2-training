#ifndef LEDC_PWM_H
#define LEDC_PWM_H


void ledc_pwm_init(void);
void ledc_Channel_SetDuty(uint8_t percent);
void ledc_pwm_app(void);

#endif
