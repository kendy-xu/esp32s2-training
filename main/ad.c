
#include "include.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   640          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;
#if CONFIG_IDF_TARGET_ESP32
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
#elif CONFIG_IDF_TARGET_ESP32S2
static adc_channel_t channel = ADC_CHANNEL_0;     // GPIO1 if ADC1, GPIO11 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_13;
#endif
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

#define USER_ADC_CH_NUM  6
static const uint8_t adc_ch[USER_ADC_CH_NUM] = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4, ADC_CHANNEL_5};

static void check_efuse(void)
{
#if CONFIG_IDF_TARGET_ESP32
    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
#elif CONFIG_IDF_TARGET_ESP32S2
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("Cannot retrieve eFuse Two Point calibration values. Default calibration values will be used.\n");
    }
#else
#error "This example is configured for ESP32/ESP32S2."
#endif
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

void user_adc_init(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    for(uint8_t i = 0; i < USER_ADC_CH_NUM; i++)
    {
        channel = adc_ch[USER_ADC_CH_NUM];
        if (unit == ADC_UNIT_1) {
            adc1_config_width(width);
            adc1_config_channel_atten(channel, atten);
        } else {
            adc2_config_channel_atten((adc2_channel_t)channel, atten);
        }
    }

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);

}

void user_adc_sample(uint8_t adc_unit, uint8_t ch, uint32_t* result)
{
    uint32_t adc_reading = 0;    
    channel = adc_ch[ch];
    //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        if (unit == adc_unit) {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
        } else {
            int raw;
            adc2_get_raw((adc2_channel_t)channel, width, &raw);
            adc_reading += raw;
        }
    }
    adc_reading /= NO_OF_SAMPLES;
    printf("adc_channel_%d value: %d.\n", ch, adc_reading);
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
    printf("Raw: %d adc_channel_%d Voltage: %dmV\n", adc_reading, ch, voltage);
    adc1_set_cal_delta(1000);
    *result++ = adc_reading;
    *result = voltage;
}