#ifndef __USART_H
#define	__USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include <stdio.h>

/* �����ж�����
 * 0���������ж�
 * 1�����������жϡ��رշ����ж�
 * 2�����������жϣ��رս����ж�
 * 3���������ͺͽ����ж�
*/
#define USART1_IRQ_ALLOW	1
#define USART2_IRQ_ALLOW	1

#define USART1_TXBUFF_SIZE   256
#define USART2_TXBUFF_SIZE   256


void UART1_Init(uint32_t baud);
void UART2_Init(uint32_t baud);
void u1_printf(char*, ...);         	//����1 printf����
void u2_printf(char*, ...);         	//����2 printf����

#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
