#include "adc_pa0.h"
																   
void ADC_Function_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure={0};
	GPIO_InitTypeDef GPIO_InitStructure={0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
	ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE); 
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
	
    ADC_BufferCmd(ADC1 , ENABLE);
}

uint16_t Get_ADC_Val(uint8_t ch)
{
	ADC_RegularChannelConfig(ADC1 , ch , 1 , ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1 , ENABLE);

	while(!ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC));

	return ADC_GetConversionValue(ADC1);
}

void MQ135_Int_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA , &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

extern uint16_t lora_tx_buffer[50];

__attribute__((interrupt(), weak)) void EXTI15_10_IRQHandler(void)
{
    EXTI_ClearITPendingBit(GPIO_Pin_15);
    printf("MQ135 int warning.\n");
	lora_tx_buffer[4] = 1;
}


void DAC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;
	DAC_Init(DAC_Channel_1,&DAC_InitType);

	DAC_Cmd(DAC_Channel_1, ENABLE);

	DAC_SetChannel1Data(DAC_Align_12b_R, 0);

}
