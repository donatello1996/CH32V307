#include "ad7606.h"

#define AD_DATA_GPIO					GPIOE

#define READ_AD_DATA   	 				AD_DATA_GPIO->INDR

#define AD7606_RD_1						GPIOD->BSHR = GPIO_Pin_2;
#define AD7606_RD_0						GPIOD->BCR = GPIO_Pin_2;

#define AD7606_CS_1						GPIOD->BSHR = GPIO_Pin_3;
#define AD7606_CS_0						GPIOD->BCR = GPIO_Pin_3;

#define AD7606_RESET_1				    GPIOD->BSHR = GPIO_Pin_4;
#define AD7606_RESET_0				    GPIOD->BCR = GPIO_Pin_4;

uint16_t ad0_buffer[403] = {0xadad , 0x0000 , 0xffff};
uint16_t ad0_count = 0;
bool flag_ad0 = false;
extern uint8_t socketdip[2][4];
extern uint16_t dport[2];

void AD7606_PWM_ModeConfig(uint32_t frequency)
{
	uint32_t arr_value, psc_value;

	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PWM(Hz) = 144MHz / ((ARR + 1) * (PSC + 1))
	//ARR(arr_value) -> TIM_Period
	//PSC(psc_value) -> TIM_Prescaler
	
	//psc_value = 144 - 1;
	//arr_value = SystemCoreClock / (psc_value + 1) / frequency - 1;

	psc_value = 1;
	arr_value = 899;
	
	TIM_TimeBaseStructure.TIM_Period = arr_value;
	TIM_TimeBaseStructure.TIM_Prescaler = psc_value;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OCInitStructure.TIM_Pulse = 4;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1 , TIM_OCPreload_Enable);

	TIM_OCInitStructure.TIM_Pulse = 4;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1 , TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1 , ENABLE);

	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1 , ENABLE);
}

void AD7606_RD_CS_RST_Dataport_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO , ENABLE);
	
	//AD7606 DATA GPIO
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	// GPIO_Init(GPIOE , &GPIO_InitStructure);

	// ADREAD->PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD , &GPIO_InitStructure);

	// ADCS->PD3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD , &GPIO_InitStructure);

	// COVA->PA8, COVB->PA11 PWM
	// ADRESET->PD4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD , &GPIO_InitStructure);

	// ADBUSY->PA15 EXTI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	Delay_ms(1);
	AD7606_RESET_0;
//	AD7606_SetOS(AD_OS_NO);
	AD7606_CS_0;
	AD7606_RD_1;
	Delay_ms(1);
	AD7606_RESET_1;
	Delay_us(1);
	AD7606_RESET_0;
	Delay_us(1);
}

void AD7606_EXTI5_Int_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD , &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static int count_exti = 0 , count_exti_400 = 0;

void AD7606ReadSample0(void)
{
	int i;
	short ad0;
	AD7606_RD_0;
	__NOP();
	ad0 = READ_AD_DATA;
	AD7606_RD_1;
	__NOP();
	
	if(ad0_count + 3 >= 403)
	{
		ad0_count = 0;
		flag_ad0 = true;
	}
	ad0_buffer[0] = 0xadad;
	ad0_buffer[1] = 0;
	ad0_buffer[2] = 0xffff;
	ad0_buffer[ad0_count + 3] = ad0;
	ad0_count ++;
	
	if(flag_ad0 == true)
	{
		//printf("count_exti_400 = %d\n" , count_exti_400);
		count_exti_400++;
		flag_ad0 = false;
		for(i = 0 ; i < 403 ; i++)
		{
			//printf("i = %d %f\n" , i , ad0_buffer[i] / 3276.8);
		}
		W5500_Send_UDP_Buffer_Specify_IP_port(0 , (uint8_t *)ad0_buffer , 806 , socketdip[0] , dport[0]);
		memset(ad0_buffer, 0, sizeof(ad0_buffer));
	}
	//printf("count_exti = %d\n" , count_exti);
	//count_exti++;
}

__attribute__((interrupt(), weak)) void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
    AD7606ReadSample0();
    EXTI_ClearITPendingBit(EXTI_Line5);
  }
}