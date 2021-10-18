
#ifndef USER_UART_H
#define USER_UART_H

#include "hal\uart_ll.h"

#define UART_MOCE_EVENT
//#define UART_MODE_TASK
//#define UART_MODE_ISR //此模式还有问题，在接收到数据时会复位



//#define DIGIO-LEVEL-SET DIO_0 H


//===============================================
void Creat_uart_task(void);


//===============================================
#endif