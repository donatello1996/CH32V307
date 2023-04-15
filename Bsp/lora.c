#include "lora.h"

tLoRaSettings LoRaSettings =
{
    400000000,
    20,
    8,             // BW [0:7.8kHz, 1:10.4kHz, 2:15.6kHz, 3:20.8kHz, 4:31.2kHz, 5:41.6kHz, 6: 62.5kHz, 7:125kHz, 8:250kHz, 9:500kHz, other: Reserved]
				   // 432MHZ~434MHZ
    11,            // SF [6:64, 7:128, 8:256, 9:512, 10:1024, 11:2048, 12:4096  chips]
	               // 
    1,             // [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    true,          // CRC[0: OFF, 1: ON]
    false,         // [0: OFF, 1: ON]
    0,             // [0: Continuous, 1 Single]
    0,             // FreqHopOn[0: OFF, 1: ON] 
    4,             // HopPeriod
    100000,        // 
    1000000,       // 
    128,           //
};

struct _SX1276
{    
    uint8_t InitOK;
    uint8_t Busy;
    uint8_t RxMode;
    uint8_t TxMode;    
}xLORA;

#define RF_BUFFER_SIZE    256
static uint8_t regTemp;
static uint8_t RxSize;
static uint8_t RxBuffer[RF_BUFFER_SIZE];

const double SignalBwLog[] =
{
    3.8927900303521316335038277369285,  // 7.8 kHz
    4.0177301567005500940384239336392,  // 10.4 kHz
    4.193820026016112828717566631653,   // 15.6 kHz
    4.31875866931372901183597627752391, // 20.8 kHz
    4.4948500216800940239313055263775,  // 31.2 kHz
    4.6197891057238405255051280399961,  // 41.6 kHz
    4.795880017344075219145044421102,   // 62.5 kHz
    5.0969100130080564143587833158265,  // 125 kHz
    5.397940008672037609572522210551,   // 250 kHz
    5.6989700043360188047862611052755   // 500 kHz
};

const double RssiOffsetLF[] =
{   
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
};

void RA02_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = LORA_NSS_PIN;
	GPIO_Init(LORA_NSS_GPIO , &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LORA_SPI_RESET_PIN;
	GPIO_Init(LORA_SPI_RESET_GPIO, &GPIO_InitStruct);
	
	GPIO_WriteBit(LORA_SPI_RESET_GPIO , LORA_SPI_RESET_PIN , Bit_SET);

}

void RA02_EXTI_Int_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE , ENABLE);

    GPIO_InitStructure.GPIO_Pin = LORA_DIO0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(LORA_DIO0_GPIO , &GPIO_InitStructure);

    GPIO_EXTILineConfig(LORA_DIO0_PORT_SOURCE , LORA_DIO0_PIN_SOURCE);
    EXTI_InitStructure.EXTI_Line = LORA_DIO0_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SX1276_Init(uint32_t fre , uint8_t bw)
{    
    xLORA.InitOK = 0;
    LoRaSettings.RFFrequency = fre * 1000 * 1000;
    LoRaSettings.SpreadingFactor = 11;
    LoRaSettings.SignalBw = bw;
    
    SX1276_Reset();   

	SX1276Read(0x06, &regTemp ,1);
	while(regTemp != 0x6C)
	{
	    printf("SX1276_Init error.\n");
	    Delay_ms(300);
	}
   
    SX1276_SetOpMode(RFLR_OPMODE_SLEEP);

    SX1276Read(0x01, &regTemp , 1);
    printf("read 0x01 regTemp = 0x%x\n" , regTemp);
    regTemp |= 0x80 ;        
    SX1276Write( 0x01, &regTemp , 1);
    printf("write 0x01 regTemp = 0x%x\n" , regTemp);
    SX1276Read (0x01, &regTemp , 1);
	printf("read 0x01 regTemp = 0x%x\n" , regTemp);
    
    regTemp  = RFLR_LNA_GAIN_G1;
    SX1276Write ( REG_LR_LNA , &regTemp , 1);
    
    SX1276LoRaSetRFFrequency(LoRaSettings.RFFrequency);
    SX1276LoRaSetSpreadingFactor(LoRaSettings.SpreadingFactor);
    SX1276LoRaSetErrorCoding(LoRaSettings.ErrorCoding);
    SX1276LoRaSetPacketCrcOn(LoRaSettings.CrcOn);
    SX1276LoRaSetSignalBandwidth(LoRaSettings.SignalBw);

    SX1276LoRaSetImplicitHeaderOn(LoRaSettings.ImplicitHeaderOn );
    SX1276LoRaSetSymbTimeout(0x3FF);
        
    SX1276Write( 0x22, &LoRaSettings.PayloadLength , 1 );
    
    SX1276LoRaSetLowDatarateOptimize( true );
   
    SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_PABOOST );
    
    regTemp = 0x87;
    SX1276Write(REG_LR_PADAC , &regTemp , 1);
    regTemp = 0x8F;    
    SX1276Write(REG_LR_PACONFIG , &regTemp , 1);
    
    //SX1276Read (0x26, &regTemp , 1);
    //regTemp = (regTemp & 0xF7) | ( 1<<3);
    //SX1276Write (0x26, &regTemp, 1);       
    
    regTemp = 0xFF;
    SX1276Write(REG_LR_IRQFLAGS ,&regTemp, 1);

    xLORA.InitOK = 1;    
    
    printf("SX1276_Init OK.\n");

#if BOARD_MODEL == BOARD_CH32_EVB_LORA

    printf("SX1276 TxMode.\n");

#elif BOARD_MODEL == BOARD_CH32_TOS_LORA

    Lora_RxMode();
    printf("SX1276 RxMode.\n");

#endif

}    

uint8_t SX1276_SpiSendByte(uint8_t data)
{
	return SPI1_ReadWriteByte(data);
}

void SX1276Write( uint8_t addr, uint8_t *buffer, uint8_t size )
{    
    int i = 0;
    LORA_NSS_LOW;
    SX1276_SpiSendByte( addr | 0x80 );
    for(i = 0 ; i < size ; i++)
    {
        SX1276_SpiSendByte( buffer[i] );
    }
    LORA_NSS_HIGH;
}

void SX1276Read(uint8_t addr, uint8_t *buffer, uint8_t size )
{
    int i = 0;
    LORA_NSS_LOW;
    SX1276_SpiSendByte( addr & 0x7F );
    for(i = 0; i < size; i++ )
    {
        buffer[i] = SX1276_SpiSendByte(0);
    }    
    LORA_NSS_HIGH;
}

void SX1276_Reset(void)
{
	LORA_SPI_RESET_GPIO ->BCR |= LORA_SPI_RESET_PIN;
    Delay_ms (1);
    
    LORA_SPI_RESET_GPIO ->BSHR |= LORA_SPI_RESET_PIN;
    Delay_ms (6); 
}

void SX1276LoRaSetRFFrequency(uint32_t freq)
{    
    uint8_t F[3];
    freq = ( uint32_t )( ( double )freq / ( double )61.03515625f );
    F[0] = ( uint8_t )( ( freq >> 16 ) & 0xFF );
    F[1] = ( uint8_t )( ( freq >> 8 ) & 0xFF );
    F[2] = ( uint8_t )( freq & 0xFF );
    SX1276Write(0x06 , F , 3);
}

void SX1276LoRaSetSignalBandwidth(uint8_t bw)
{
    SX1276Read( 0x1D, &regTemp , 1 );
    regTemp  = ( regTemp  & 0x0F ) | ( bw << 4 );
    SX1276Write( 0x1D, &regTemp , 1);
    //LoRaSettings.SignalBw = bw;
}

void SX1276LoRaSetSpreadingFactor( uint8_t factor )
{
    if( factor > 12 )   factor = 12;    
    if( factor < 6 )    factor = 6;  
    
    SX1276Read (0x31, &regTemp ,1);
    if( factor == 6 )   
        regTemp = (regTemp & 0xF8) | 5;             
    else       
        regTemp = (regTemp & 0xF8) | 3;
    SX1276Write (0x31, &regTemp , 1);         

    SX1276Read ( 0x1E, &regTemp , 1);
    regTemp =  (regTemp & 0x0F) | (factor << 4);
    SX1276Write (0x1E, &regTemp , 1);  
}

void SX1276LoRaSetErrorCoding( uint8_t value )
{
    SX1276Read( 0x1D, &regTemp , 1 );
    regTemp  = ( regTemp  & 0xF1 ) | ( value << 1 );
    SX1276Write( 0x1D, &regTemp ,1 );
}

void SX1276LoRaSetPacketCrcOn( bool enable )
{
    SX1276Read( REG_LR_MODEMCONFIG2, &regTemp ,1 );
    regTemp  = ( regTemp  & 0xFB ) | ( enable << 2);
    SX1276Write( REG_LR_MODEMCONFIG2, &regTemp , 1);
}

void SX1276LoRaSetImplicitHeaderOn( bool enable )
{   
    SX1276Read( 0x1D, &regTemp ,1 );
    regTemp = ( regTemp & 0xFE) | ( enable );
    SX1276Write(0x1D, &regTemp ,1 );
}

void SX1276LoRaSetPAOutput( uint8_t outputPin )
{
    SX1276Read( REG_LR_PACONFIG, &regTemp ,1 );
    regTemp  = (regTemp  & 0x7F ) | outputPin;
    SX1276Write(REG_LR_PACONFIG, &regTemp, 1 );
}

void SX1276LoRaSetSymbTimeout( uint16_t value )
{
    uint8_t reg[2];
    SX1276Read( 0x1E, reg, 2 ); 
    reg[0] = ( reg[0] & 0xFC ) | ( ( value >> 8 ) & ~0xFC );
    reg[1] = value & 0xFF;
    SX1276Write( 0x1E, reg, 2);
}

void SX1276LoRaSetLowDatarateOptimize( bool enable )
{  
    SX1276Read( REG_LR_MODEMCONFIG3, &regTemp, 1 );
    regTemp = ( regTemp & 0xF7 ) | ( enable << 3 );
    SX1276Write( REG_LR_MODEMCONFIG3, &regTemp,1 ); 
}


void SX1276_SetOpMode( uint8_t opMode )
{
    SX1276Read( 0x01, &regTemp, 1 );
    printf("SX1276_SetOpMode read 0x01 regTemp = 0x%x\n" , regTemp);
    regTemp &= 0xF8;
    regTemp |= (uint8_t)opMode ;
    printf("SX1276_SetOpMode write 0x01 regTemp = 0x%x\n" , regTemp);
    SX1276Write( 0x01, &regTemp , 1 ); 
    SX1276Read( 0x01, &regTemp, 1 );
    printf("SX1276_SetOpMode read 0x01 regTemp = 0x%x\n" , regTemp);
}

uint8_t Lora_GetGain( void )                    
{	
	SX1276Read( REG_LR_LNA, &regTemp , 1);
	return( regTemp >> 5 ) & 0x07;
}

int8_t Lora_GetSNR (void)                                                
{
    int8_t _SNR;
	uint8_t rxSnrEstimate;				    
	SX1276Read(REG_LR_PKTSNRVALUE, &rxSnrEstimate , 1);			
			
	if( rxSnrEstimate & 0x80 )  
	{
		_SNR = ( ( ~rxSnrEstimate + 1 ) & 0xFF ) >> 2;	
		_SNR = -_SNR;
	}
	else                        
	{		
		_SNR = ( rxSnrEstimate & 0xFF ) >> 2;			
	}
	return _SNR;
}

double Lora_GetRSSI(void)                                             
{
    double _RSSI;
    int8_t _SNR = Lora_GetSNR();  
	if(  _SNR < 0 )
	{
		_RSSI = (-174) + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + 4.0 + (double)_SNR;
	}
	else
	{
		SX1276Read( REG_LR_PKTRSSIVALUE, &regTemp ,1);
		_RSSI = RssiOffsetLF[LoRaSettings.SignalBw] + (double)regTemp ;
	}
	return _RSSI;
}

void Lora_StandbyMode(void)
{    
    if(xLORA.InitOK == 0) 
    {
        printf("Lora_StandbyMode Init error.\n");
        return;
    }
    regTemp = 0xFF;
    SX1276Write( REG_LR_IRQFLAGS, &regTemp  ,1);
    SX1276_SetOpMode( RFLR_OPMODE_STANDBY ); 
   
    xLORA. TxMode =0;    
    xLORA .RxMode=0;    
}

void Lora_TxData(char* buf, uint8_t size)
{    
    if(xLORA.InitOK == 0) 
    {
        printf("Lora_TxData Init error.\n");
        return;
    }  
    
    if(xLORA.TxMode == 1) 
    {
         printf("xLORA.TxMode == 1.\n");
        return;  
    }     
    
    if(size==0) return;        
    
    SX1276_SetOpMode( RFLR_OPMODE_STANDBY );
    
    regTemp  = 0;                                      
    SX1276Write( REG_LR_HOPPERIOD, &regTemp ,1 );
    
    regTemp  = RFLR_IRQFLAGS_RXTIMEOUT         |
               RFLR_IRQFLAGS_RXDONE            |
               RFLR_IRQFLAGS_PAYLOADCRCERROR   |
               RFLR_IRQFLAGS_VALIDHEADER       |
               //RFLR_IRQFLAGS_TXDONE          |
               RFLR_IRQFLAGS_CADDONE           |
               RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL|
               RFLR_IRQFLAGS_CADDETECTED;
    SX1276Write( 0x11, &regTemp, 1 );
    
    regTemp  = RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_01;
    SX1276Write(0x40 , &regTemp , 1); 

    SX1276Read(0x40, &regTemp ,1);
	printf("RFLR_DIOMAPPING regTemp = 0x%x\n" , regTemp);
    
    regTemp  = 0x00;                                                    
    SX1276Write( REG_LR_FIFOTXBASEADDR, &regTemp , 1);
    SX1276Write( REG_LR_FIFOADDRPTR, &regTemp, 1);
   
    regTemp = size;
    SX1276Write( REG_LR_PAYLOADLENGTH, &regTemp , 1 ); 
 
    SX1276Write(0 , (uint8_t*)buf , size);
    
    xLORA.TxMode = 1; 
    SX1276_SetOpMode( RFLR_OPMODE_TRANSMITTER );
}

void Lora_RxMode()
{
    if(xLORA.InitOK == 0) 
    {
        printf("Lora_RxMode Init error.\n");
        return;
    }     

    SX1276_SetOpMode( RFLR_OPMODE_STANDBY );      
   
    regTemp = 0; 
    SX1276Write( REG_LR_HOPPERIOD, &regTemp  , 1);       
       
    regTemp  = RFLR_IRQFLAGS_RXTIMEOUT |                  // ��ʱ�ж�                   RegIrqFlagsMask�Ĵ���0x11, 0:������ 1���ر� 
               //RFLR_IRQFLAGS_RXDONE  |                  // ���ݰ���������ж�
               //RFLR_IRQFLAGS_PAYLOADCRCERROR |          // ����CRC�����ж� 
               RFLR_IRQFLAGS_VALIDHEADER |                // Rx ģʽ�½��յ�����Ч��ͷ
               RFLR_IRQFLAGS_TXDONE      |                // FIFO ���ط�������ж�
               RFLR_IRQFLAGS_CADDONE     |                // CAD ����ж�����
               RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL|          // FHSS�ı��ŵ��ж�
               RFLR_IRQFLAGS_CADDETECTED;                 // ��⵽ CAD �ж�
    SX1276Write( REG_LR_IRQFLAGSMASK, &regTemp ,1 );      // �µ��ж�ֵд�뵽�豸�жϼĴ���
    
    // �ж�ӳ�� ��7��6��DIO0 ��������ж�   ��5��4��DIO1 RxTimeout�ж�  ��3��2��DIO2 FhssChange�ж�  ��1��0��DIO3 CadDone�ж�   
    regTemp  = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_00;
    SX1276Write ( 0x40, &regTemp ,1);
	SX1276Read(0x40, &regTemp ,1);
	printf(" RFLR_DIOMAPPING regTemp = 0x%x\n" , regTemp);

    //SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
    //sx1276_SpiWriteByte( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr ); 
          
    regTemp = 0xFF;                                   // �����жϱ�־λ
    SX1276Write(REG_LR_IRQFLAGS , &regTemp , 1);      
    
    SX1276_SetOpMode( RFLR_OPMODE_RECEIVER );    
    xLORA .RxMode =1;    
}

__attribute__((interrupt(), weak)) void LORA_DIO0_HANDLER(void)
{
    char str[20];
    short accx , accy , gyrox , gyroy , magx;
    EXTI_ClearITPendingBit(LORA_DIO0_PIN);
    printf("LORA_DIO0_HANDLER xLORA.TxMode = %d xLORA.RxMode = %d\n" , xLORA.TxMode , xLORA.RxMode);
#if BOARD_MODEL == BOARD_CH32_EVB_LORA

    if(xLORA.TxMode == 1)   
    {
        printf("LORA_DIO0_HANDLER xLORA.TxMode = 1\n");
        regTemp = 0xFF;
        SX1276Write( 0x12, &regTemp , 1);                 
        xLORA.TxMode = 0;
                 
        Lora_RxMode(0);
        xLORA .RxMode = 1;
              
        return;
    }

#endif

    if(xLORA.RxMode == 1)
	{            
        regTemp = 0xFF;
        SX1276Write( 0x12, &regTemp ,1 );           
    
        if( LoRaSettings.ImplicitHeaderOn == true ) 
            RxSize = LoRaSettings.PayloadLength ;       
        else
            SX1276Read( REG_LR_NBRXBYTES, &RxSize, 1 ); 
        
        SX1276Read( REG_LR_FIFORXCURRENTADDR, &regTemp ,1);
        SX1276Write( REG_LR_FIFOADDRPTR, &regTemp ,1 );
        SX1276Read(0, RxBuffer, RxSize );   

#if BOARD_MODEL == BOARD_CH32_TOS_LORA
                
        printf("RxSize %d RxBuffer: %s\r" , RxSize , RxBuffer);

        if(RxBuffer[0] == 0x01 && RxBuffer[1] == 0x03)
        {

            sprintf(str , "Pres:%04d.%d" ,  RxBuffer[6] << 8 | RxBuffer[7] ,  RxBuffer[8] << 8 | RxBuffer[9]);
			TOS_LCD_ShowString_1632(0 , 32 , str , WHITE , BLACK);

            sprintf(str , "Temp:%d Humi:%d" ,  RxBuffer[10] << 8 | RxBuffer[11] ,  RxBuffer[12] << 8 | RxBuffer[13]);
			TOS_LCD_ShowString_1632(0 , 0 , str , WHITE , BLACK);

            sprintf(str , "MQ135:%04d" ,  RxBuffer[2] << 8 | RxBuffer[3]);
			TOS_LCD_ShowString_1632(0 , 64 , str , WHITE , BLACK);

            accx = RxBuffer[14] << 8 | RxBuffer[15];
            accy = RxBuffer[16] << 8 | RxBuffer[17];
            sprintf(str , "AX:%d AY:%d" , accx , accy);
			TOS_LCD_ShowString_1632(0 , 96 , str , WHITE , BLACK);

            gyrox = RxBuffer[20] << 8 | RxBuffer[21];
            gyroy = RxBuffer[22] << 8 | RxBuffer[23];
            sprintf(str , "GX:%d GY:%d" ,  gyrox , gyroy);
			TOS_LCD_ShowString_1632(0 , 128 , str , WHITE , BLACK);

            magx = RxBuffer[26] << 8 | RxBuffer[27];
            sprintf(str , "MX:%d" , magx);
			TOS_LCD_ShowString_1632(0 , 160 , str , WHITE , BLACK);

            sprintf(str , "LIGHT:%05d" , RxBuffer[32] << 8 | RxBuffer[33]);
			TOS_LCD_ShowString_1632(0 , 192 , str , WHITE , BLACK);
        }

#endif

	}
}
