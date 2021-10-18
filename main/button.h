#ifndef BUTTON_H
#define BUTTON_H

//===============================================
#define KEY_6_8OZ_GPIO   6
#define KEY_8_10OZ_GPIO  7
#define KEY_10_12OZ_GPIO 8
#define KEY_POWER_GPIO   9


typedef enum {
	BUTTON_RELEASED,
	BUTTON_DOWN,
	BUTTON_PRESSED,
	BUTTON_UP
}button_state_t;

typedef struct{
	uint16_t Counter;
	uint8_t pressCounter;
	uint8_t releaseCounter;
	bool pressFlag;
	bool releaseFlag;
	button_state_t ButtonState;
}button_scan_t;

//===============================================
void GPIO_Key_init(void);
void CreatKeyScanTimer(void);
void KeyInt_init(void);
void CreatKeyIntTimer(void);
void CreatKeyQueue(void);
void ReceiveKeyQueue(void);

//===============================================
#endif