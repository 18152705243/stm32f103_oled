#include "bsp_usart.h"
#include <stdarg.h>
#include <string.h>

void UART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	
#if USART1_IRQ_ALLOW != 0
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	//USART1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	// 打开串口GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	// 打开串口外设的时钟

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	USART_InitStructure.USART_BaudRate = baud;	// 配置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 配置 针数据字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	// 配置停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;	// 配置校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 配置硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 配置工作模式，收发模式
	USART_Init(USART1, &USART_InitStructure);

#if USART1_IRQ_ALLOW == 1 || USART1_IRQ_ALLOW == 3
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启串口接受中断
#endif
#if USART1_IRQ_ALLOW == 2 || USART1_IRQ_ALLOW == 3
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	//开启串口发送中断
#endif
	// 使能串口
	USART_Cmd(USART1, ENABLE);	    
}

void UART2_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	
#if USART2_IRQ_ALLOW != 0
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	//USART1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
#endif
	
	// 打开串口GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	// 打开串口外设的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = baud;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART2, &USART_InitStructure);

#if USART2_IRQ_ALLOW == 1 || USART2_IRQ_ALLOW == 3
	//开启串口接受中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif
#if USART2_IRQ_ALLOW == 2 || USART2_IRQ_ALLOW == 3
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
#endif
	// 使能串口
	USART_Cmd(USART2, ENABLE);	    
}

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout; 
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USART1, (uint8_t) ch);
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}


/*-------------------------------------------------*/
/*函数名：串口1 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数     */
/*返回值：无                                       */
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
/*函数名：串口2 printf函数                          */
/*参  数：char* fmt,...  格式化输出字符串和参数      */
/*返回值：无                                        */
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
