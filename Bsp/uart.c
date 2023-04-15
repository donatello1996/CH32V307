#include "uart.h"

void UART1_Init(uint32_t baud)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

}

__attribute__((used)) int _write(int fd, char *buf, int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        while((USART1->STATR & USART_FLAG_TC) == (uint16_t)RESET);
        USART1->DATAR = (buf[i] & (uint16_t)0x01FF);
    }

    return size;
}

void UART1_Send_Char(char c)
{
    while((USART1->STATR & USART_FLAG_TC) == (uint16_t)RESET);
    USART1->DATAR = (c & (uint16_t)0x01FF);
}

void UART1_Send_String(char s[])
{
    int i;
    for(i = 0 ; s[i] != '\0' ; i++)
    {
        while((USART1->STATR & USART_FLAG_TC) == (uint16_t)RESET);
        USART1->DATAR = (s[i] & (uint16_t)0x01FF);
    }
}

uint8_t		com2_rx_buffer[1024];
uint16_t 	com2_rx_count;
uint8_t		com4_rx_buffer[1024];
uint16_t 	com4_rx_count;

#define 	COM_RX_BUFFER_SIZE  1024

USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

void UART2_Init(int baud , int flag_idle_dma)
{
	GPIO_InitTypeDef USART_GPIO;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	USART_GPIO.GPIO_Pin =  GPIO_Pin_2;
	USART_GPIO.GPIO_Speed = GPIO_Speed_50MHz;        
	USART_GPIO.GPIO_Mode = GPIO_Mode_AF_PP;          
	GPIO_Init(GPIOA , &USART_GPIO);                
	USART_GPIO.GPIO_Pin = GPIO_Pin_3;
	USART_GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;    
	GPIO_Init(GPIOA , &USART_GPIO);
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2 , &USART_InitStructure);
	USART_Cmd(USART2 , ENABLE);
	
	if(flag_idle_dma)
	{
		USART_ITConfig(USART2 , USART_IT_IDLE , ENABLE);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
							 
		DMA_DeInit(DMA1_Channel6);						 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                   
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;             
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DATAR);  
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)com2_rx_buffer;
		DMA_InitStructure.DMA_BufferSize = COM_RX_BUFFER_SIZE;
		DMA_Init(DMA1_Channel6 , &DMA_InitStructure);  
		USART_DMACmd(USART2 , USART_DMAReq_Rx , ENABLE);
		DMA_Cmd(DMA1_Channel6 , ENABLE);
	}
}

void UART2_Send_Char(char c)
{
    while((USART2->STATR & USART_FLAG_TC) == (uint16_t)RESET);
    USART2->DATAR = (c & (uint16_t)0x01FF);
}

void UART2_Send_String(char s[])
{
    int i;
    for(i = 0 ; s[i] != '\0' ; i++)
    {
        while((USART2->STATR & USART_FLAG_TC) == (uint16_t)RESET);
        USART2->DATAR = (s[i] & (uint16_t)0x01FF);
    }
}

uint8_t spo2 = 0 , heart = 0;

__attribute__((interrupt(), weak)) void USART2_IRQHandler(void)
{
	int i = 0;
 	if(USART_GetITStatus(USART2 , USART_IT_IDLE) != RESET)  
	{
		USART_ReceiveData(USART2);
		DMA_Cmd(DMA1_Channel6 , DISABLE);  
		com2_rx_count = COM_RX_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);
		DMA1_Channel6->CNTR = COM_RX_BUFFER_SIZE;
		DMA_Cmd(DMA1_Channel6 , ENABLE);
		for(i = 0 ; i < com2_rx_count ; i ++)
		{
			if(com2_rx_buffer[i] == '+' && com2_rx_buffer[i + 1] == 'S' && com2_rx_buffer[i + 2] == 'P' 
			&& com2_rx_buffer[i + 3] == 'O' && com2_rx_buffer[i + 4] == '2'  && com2_rx_buffer[i + 5] == '='
			&& com2_rx_buffer[i + 6] != 'N'
			)
			{
				spo2 =  (com2_rx_buffer[i + 6] - '0') * 10 + com2_rx_buffer[i + 7] - '0';
			}
			if(com2_rx_buffer[i] == '+' && com2_rx_buffer[i + 1] == 'H' && com2_rx_buffer[i + 2] == 'E' 
			&& com2_rx_buffer[i + 3] == 'A' && com2_rx_buffer[i + 4] == 'R'  && com2_rx_buffer[i + 5] == 'T'
			&& com2_rx_buffer[i + 6] == '=' && com2_rx_buffer[i + 7] != 'N'
			)
			{
				heart = com2_rx_buffer[i + 7] - '0';

				if('0' <= com2_rx_buffer[i + 8] && com2_rx_buffer[i + 8] <= '9')
				{
					heart = (com2_rx_buffer[i + 7] - '0') * 10 + com2_rx_buffer[i + 8] - '0';
				}

				if('0' <= com2_rx_buffer[i + 9] && com2_rx_buffer[i + 9] <= '9')
				{
					heart = (com2_rx_buffer[i + 7] - '0') * 100 + (com2_rx_buffer[i + 8] - '0') * 10 + com2_rx_buffer[i + 9] - '0';
				}
				
			}
		}

	}
}

void UART4_Init(int baud , int flag_idle_dma)
{
	GPIO_InitTypeDef USART_GPIO;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4 , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2 , ENABLE);
	
	USART_GPIO.GPIO_Pin =  GPIO_Pin_10;
	USART_GPIO.GPIO_Speed = GPIO_Speed_50MHz;        
	USART_GPIO.GPIO_Mode = GPIO_Mode_AF_PP;          
	GPIO_Init(GPIOC , &USART_GPIO);                
	USART_GPIO.GPIO_Pin = GPIO_Pin_11;
	USART_GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_Init(GPIOC , &USART_GPIO);
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4 , &USART_InitStructure);
	USART_Cmd(UART4 , ENABLE);

	if(flag_idle_dma)
	{
		USART_ITConfig(UART4 , USART_IT_IDLE , ENABLE);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
							 
		DMA_DeInit(DMA2_Channel3);						 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                   
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;             
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DATAR);  
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)com4_rx_buffer;
		DMA_InitStructure.DMA_BufferSize = COM_RX_BUFFER_SIZE;
		DMA_Init(DMA2_Channel3 , &DMA_InitStructure);  
		USART_DMACmd(UART4 , USART_DMAReq_Rx , ENABLE);
		DMA_Cmd(DMA2_Channel3 , ENABLE);
	}
}

void UART4_Send_Char(char c)
{
    while((UART4->STATR & USART_FLAG_TC) == (uint16_t)RESET);
    UART4->DATAR = (c & (uint16_t)0x01FF);
}

void UART4_Send_String(char s[])
{
    int i;
    for(i = 0 ; s[i] != '\0' ; i++)
    {
        while((UART4->STATR & USART_FLAG_TC) == (uint16_t)RESET);
        UART4->DATAR = (s[i] & (uint16_t)0x01FF);
    }
}

__attribute__((interrupt(), weak)) void UART4_IRQHandler(void)
{
	int i = 0;
 	if(USART_GetITStatus(UART4 , USART_IT_IDLE) != RESET)  
	{
		USART_ReceiveData(UART4);
		DMA_Cmd(DMA2_Channel3 , DISABLE);  
		com2_rx_count = COM_RX_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA2_Channel3);
		DMA2_Channel3->CNTR = COM_RX_BUFFER_SIZE;
		DMA_Cmd(DMA2_Channel3 , ENABLE);
		printf("UART4 com4_rx_buffer = %s\n" , com4_rx_buffer);
	}
}