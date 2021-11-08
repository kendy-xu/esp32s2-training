
#ifndef USER_AD_H
#define USER_AD_H


void user_adc_init(void);
void user_adc_sample(uint8_t adc_unit, uint8_t ch, uint32_t* result);

#endif
