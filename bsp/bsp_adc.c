#include "bsp_adc.h"

#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1�ĵ�ַ

extern uint16_t adc_buf[1024];
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure = {0}; 
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE);	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 3;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	// ����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

    
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��ADC1ͨ��ʱ��

    // DMA1��ʼ��
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;                 // ADC1��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_buf;       //�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //����(�����赽�ڴ�)
    DMA_InitStructure.DMA_BufferSize = 100;                                    //�������ݵĴ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //�����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݵ�λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //�ڴ����ݵ�λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // DMAģʽ��ѭ������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //���ȼ�����
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //��ֹ�ڴ浽�ڴ�Ĵ���
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                //����DMA1

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //ʹ�ܴ�������ж�
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;	//ת�����ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	// ����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);				//��λУ׼�Ĵ���
    while(ADC_GetResetCalibrationStatus(ADC1));				//�ȴ�У׼�Ĵ�����λ���
    ADC_StartCalibration(ADC1);				//ADCУ׼
    while(ADC_GetCalibrationStatus(ADC1));				//�ȴ�У׼���
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);		//�����ⲿ����ģʽʹ��

}

void ADC_SetTimerFre(uint16_t psc, uint16_t arr)
{
    TIM3->PSC = psc;
    TIM3->ARR = arr;
}

