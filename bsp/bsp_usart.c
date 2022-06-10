#include "bsp_usart.h"
#include <stdarg.h>
#include <string.h>

void UART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	
#if USART1_IRQ_ALLOW != 0
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	//USART1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	// �򿪴��������ʱ��

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	USART_InitStructure.USART_BaudRate = baud;	// ���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// ���� �������ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	// ����ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;	// ����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ����Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// ���ù���ģʽ���շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);

#if USART1_IRQ_ALLOW == 1 || USART1_IRQ_ALLOW == 3
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
#endif
#if USART1_IRQ_ALLOW == 2 || USART1_IRQ_ALLOW == 3
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	//�������ڷ����ж�
#endif
	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);	    
}

void UART2_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	
#if USART2_IRQ_ALLOW != 0
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	//USART1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
#endif
	
	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	// �򿪴��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = baud;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART2, &USART_InitStructure);

#if USART2_IRQ_ALLOW == 1 || USART2_IRQ_ALLOW == 3
	//�������ڽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif
#if USART2_IRQ_ALLOW == 2 || USART2_IRQ_ALLOW == 3
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
#endif
	// ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);	    
}

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout; 
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART1, (uint8_t) ch);
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}


/*-------------------------------------------------*/
/*������������1 printf����                         */
/*��  ����char* fmt,...  ��ʽ������ַ����Ͳ���     */
/*����ֵ����                                       */
/*-------------------------------------------------*/
__align(8) char Usart1_TxBuff[USART1_TXBUFF_SIZE];  

void u1_printf(char * fmt, ...) 
{  
	unsigned int i, length;
	
	va_list ap;
	va_start(ap, fmt);
	vsprintf(Usart1_TxBuff, fmt, ap);
	va_end(ap);	
	
	length = strlen((const char*)Usart1_TxBuff);		
	while((USART1->SR&0X40) == 0);
	for(i = 0; i < length; i++)
	{			
		USART1->DR = Usart1_TxBuff[i];
		while((USART1->SR&0X40) == 0);	
	}	
}

/*-------------------------------------------------*/
/*������������2 printf����                          */
/*��  ����char* fmt,...  ��ʽ������ַ����Ͳ���      */
/*����ֵ����                                        */
/*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt, ...) 
{  
	unsigned int i, length;
	
	va_list ap;
	va_start(ap, fmt);
	vsprintf(USART2_TxBuff, fmt, ap);
	va_end(ap);	
	
	length=strlen((const char*)USART2_TxBuff);		
	while((USART2->SR&0X40) == 0);
	for(i = 0; i < length; i++)
	{			
		USART2->DR = USART2_TxBuff[i];
		while((USART2->SR&0X40) == 0);	
	}	
}
