
#ifndef USER_UART_H
#define USER_UART_H

#define EX_UART_NUM UART_NUM_1

#define UART_MODE_EVENT
//#define UART_MODE_TASK
//#define UART_MODE_ISR //此模式还有问题，在接收到数据时会复位




//===============================================
void Creat_uart_task(void);


//===============================================
#endif