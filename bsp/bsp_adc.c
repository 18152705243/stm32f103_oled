#include "bsp_adc.h"

#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1的地址

extern uint16_t adc_buf[1024];
		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure = {0}; 
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE);	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 3;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	// 根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

    
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

void ADC_TIM_Init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructre = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_DeInit(TIM3);
    TIM_TimeBaseInitStructre.TIM_Prescaler = 71;
    TIM_TimeBaseInitStructre.TIM_Period = 1;
    TIM_TimeBaseInitStructre.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructre.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructre);

    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_Cmd(TIM3, ENABLE);
}

void ADC_DMA_Init()
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能ADC1通道时钟

    // DMA1初始化
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;                 // ADC1地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_buf;       //内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //方向(从外设到内存)
    DMA_InitStructure.DMA_BufferSize = 100;                                    //传输内容的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //内存数据单位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // DMA模式：循环传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //优先级：高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //禁止内存到内存的传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                //配置DMA1

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //使能传输完成中断
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void ADC1_Init()
{
    ADC_InitTypeDef ADC_InitStructure = {0};
	GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_TIM_Init();
    ADC_DMA_Init();

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;	//转换由外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	// 根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);				//复位校准寄存器
    while(ADC_GetResetCalibrationStatus(ADC1));				//等待校准寄存器复位完成
    ADC_StartCalibration(ADC1);				//ADC校准
    while(ADC_GetCalibrationStatus(ADC1));				//等待校准完成
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);		//设置外部触发模式使能

}

void ADC_SetTimerFre(uint16_t psc, uint16_t arr)
{
    TIM3->PSC = psc;
    TIM3->ARR = arr;
}

