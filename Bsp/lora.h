#ifndef __LORA_H__
#define __LORA_H__

#include <stdbool.h>

#include "ch32v30x.h"
#include "tos_lcd.h"
#include "global_variables.h"

#define LORA_SPI_PORT           SPI1
#define LORA_SPI_CLK_EN         RCC->APB2ENR |= RCC_APB2ENR_SPI1EN
#define LORA_SPI_AF

#define LORA_SPI_SCK_GPIO       GPIOA
#define LORA_SPI_SCK_PIN        GPIO_Pin_5

#define LORA_SPI_MISO_GPIO      GPIOA
#define LORA_SPI_MISO_PIN       GPIO_Pin_6

#define LORA_SPI_MOSI_GPIO      GPIOA
#define LORA_SPI_MOSI_PIN       GPIO_Pin_7

#if BOARD_MODEL == BOARD_CH32_EVB_LORA

#define LORA_NSS_GPIO           GPIOA
#define LORA_NSS_PIN            GPIO_Pin_12

#define LORA_SPI_RESET_GPIO     GPIOA
#define LORA_SPI_RESET_PIN      GPIO_Pin_11
  
#define LORA_DIO0_GPIO          GPIOA
#define LORA_DIO0_PIN           GPIO_Pin_8
#define LORA_DIO0_EXTI_LINE     EXTI_Line8
#define LORA_DIO0_IRQN          EXTI9_5_IRQn
#define LORA_DIO0_PORT_SOURCE   GPIO_PortSourceGPIOA
#define LORA_DIO0_PIN_SOURCE    GPIO_PinSource8
#define LORA_DIO0_HANDLER       EXTI9_5_IRQHandler


#elif BOARD_MODEL == BOARD_CH32_TOS_LORA

#define LORA_NSS_GPIO           GPIOB
#define LORA_NSS_PIN            GPIO_Pin_9

#define LORA_SPI_RESET_GPIO     GPIOB
#define LORA_SPI_RESET_PIN      GPIO_Pin_8
  
#define LORA_DIO0_GPIO          GPIOE
#define LORA_DIO0_PIN           GPIO_Pin_6
#define LORA_DIO0_EXTI_LINE     EXTI_Line6
#define LORA_DIO0_IRQN          EXTI9_5_IRQn
#define LORA_DIO0_PORT_SOURCE   GPIO_PortSourceGPIOE
#define LORA_DIO0_PIN_SOURCE    GPIO_PinSource6
#define LORA_DIO0_HANDLER       EXTI9_5_IRQHandler

#endif

#define LORA_NSS_LOW            LORA_NSS_GPIO->BCR |= LORA_NSS_PIN
#define LORA_NSS_HIGH           LORA_NSS_GPIO->BSHR |= LORA_NSS_PIN

//SX1276�����ṹ��
typedef struct sLoRaSettings
{
    uint32_t RFFrequency;			    //����ͨ��Ƶ��
    int8_t Power;                       //����
    uint8_t SignalBw;                   //LORA ����[0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                                        //5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  
    uint8_t SpreadingFactor;            //��Ƶ���� LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    uint8_t ErrorCoding;                //LORA ������ [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    bool CrcOn;                         //CRCЧ�鿪�� [0: OFF, 1: ON]
    bool ImplicitHeaderOn;              //����ͷ����Ϣ���� [0: OFF, 1: ON]
    bool RxSingleOn;                    //���յ���ģʽ\����ģʽ����[0: Continuous, 1 Single]
    bool FreqHopOn;                     //��Ƶģʽ���� [0: OFF, 1: ON]
    uint8_t HopPeriod;                  //��Ƶ֮������ڳ��� Hops every frequency hopping period symbols
    uint32_t TxPacketTimeout;           //�����ʱ��
    uint32_t RxPacketTimeout;           //������ʱ��
    uint8_t PayloadLength;              //���ݳ���
}tLoRaSettings;


/*LoRaģʽ�Ĵ���ӳ��*/
//SX1276�ڲ��Ĵ�����ַ
#define REG_LR_FIFO                                 0x00 
//ͨ�üĴ���//ͨ�üĴ���
#define REG_LR_OPMODE                               0x01 
#define REG_LR_FRFMSB                               0x06 
#define REG_LR_FRFMID                               0x07
#define REG_LR_FRFLSB                               0x08 
//RFģ��Ĵ���
//��������
#define REG_LR_PACONFIG                             0x09 
#define REG_LR_OCP                                  0x0B 
//��������
#define REG_LR_LNA                                  0x0C  // �����豸, Ĭ�������
//LoRaҳ��Ĵ���
#define REG_LR_FIFOADDRPTR                          0x0D  // FIFI������·SPI�ӿڵ�ַָ��
#define REG_LR_FIFOTXBASEADDR                       0x0E  // FIFI��������д���ַ
#define REG_LR_FIFORXBASEADDR                       0x0F 
#define REG_LR_FIFORXCURRENTADDR                    0x10 
#define REG_LR_IRQFLAGSMASK                         0x11   //IAQ��־���� 
#define REG_LR_IRQFLAGS                             0x12 
#define REG_LR_NBRXBYTES                            0x13 
#define REG_LR_PKTSNRVALUE                          0x19 
#define REG_LR_PKTRSSIVALUE                         0x1A 
#define REG_LR_HOPCHANNEL                           0x1C   // ��Ƶ
#define REG_LR_MODEMCONFIG1                         0x1D   // ���ݰ������������������롢
#define REG_LR_MODEMCONFIG2                         0x1E   // ���ݰ���������Ƶ���ӡ�CRC
#define REG_LR_SYMBTIMEOUTLSB                       0x1F 
#define REG_LR_PREAMBLEMSB                          0x20 
#define REG_LR_PREAMBLELSB                          0x21 
#define REG_LR_PAYLOADLENGTH                        0x22  // ���س��ȣ�����ʽ��ͷʱ������
#define REG_LR_PAYLOADMAXLENGTH                     0x23 
#define REG_LR_HOPPERIOD                            0x24 
#define REG_LR_FIFORXBYTEADDR                       0x25
#define REG_LR_MODEMCONFIG3                         0x26
#define REG_LR_PADAC                                0x4D // ���ӼĴ���

// ����ģʽ
#define RFLR_OPMODE_MASK                            0xF8 // [2��0]
#define RFLR_OPMODE_SLEEP                           0x00 // ˯��ģʽ**
#define RFLR_OPMODE_STANDBY                         0x01 // ����ģʽ**
#define RFLR_OPMODE_SYNTHESIZER_TX                  0x02 // Ƶ�ʺϳ���ת����TxƵ��
#define RFLR_OPMODE_TRANSMITTER                     0x03 // ����ģʽ
#define RFLR_OPMODE_SYNTHESIZER_RX                  0x04 // Ƶ�ʺϳ���ת����RxƵ��
#define RFLR_OPMODE_RECEIVER                        0x05 // ����ģʽ**
// LoRa specific modes                                   
#define RFLR_OPMODE_RECEIVER_SINGLE                 0x06 // ���ν���ģʽ **  
#define RFLR_OPMODE_CAD                             0x07 // CADģʽ 

//PA�����ʷŴ����� ѡ���������ʿ���������غ궨�� RegPaConfig���Ĵ�����ַ0X09��     
#define RFLR_PACONFIG_PASELECT_PABOOST              0x80 
////LNA���������Ŵ��� ��������غ궨�� RegLna���Ĵ�����ַ0X0C��
#define RFLR_LNA_GAIN_G1                            0x20 // Default
//FIFO ���ݻ������� SPI �ӿڵ�ַָ�루�Ĵ�����ַ0X0D��
#define RFLR_FIFOADDRPTR                            0x00 // Default
//������Ϣ����ʼλ��
#define RFLR_FIFOTXBASEADDR                         0x80 // Default
//������Ϣ����ʼλ��
#define RFLR_FIFORXBASEADDR                         0x00 // Default

//�����жϴ���صĺ궨��
#define RFLR_IRQFLAGS_RXTIMEOUT                     0x80 
#define RFLR_IRQFLAGS_RXDONE                        0x40 
#define RFLR_IRQFLAGS_PAYLOADCRCERROR               0x20 
#define RFLR_IRQFLAGS_VALIDHEADER                   0x10 
#define RFLR_IRQFLAGS_TXDONE                        0x08 
#define RFLR_IRQFLAGS_CADDONE                       0x04 
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL            0x02 
#define RFLR_IRQFLAGS_CADDETECTED                   0x01 

// RegDioMapping1
#define RFLR_DIOMAPPING1_DIO0_MASK                  0x3F
#define RFLR_DIOMAPPING1_DIO0_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO0_01                    0x40
#define RFLR_DIOMAPPING1_DIO0_10                    0x80
#define RFLR_DIOMAPPING1_DIO0_11                    0xC0

#define RFLR_DIOMAPPING1_DIO1_MASK                  0xCF
#define RFLR_DIOMAPPING1_DIO1_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO1_01                    0x10
#define RFLR_DIOMAPPING1_DIO1_10                    0x20
#define RFLR_DIOMAPPING1_DIO1_11                    0x30

#define RFLR_DIOMAPPING1_DIO2_MASK                  0xF3
#define RFLR_DIOMAPPING1_DIO2_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO2_01                    0x04
#define RFLR_DIOMAPPING1_DIO2_10                    0x08
#define RFLR_DIOMAPPING1_DIO2_11                    0x0C

#define RFLR_DIOMAPPING1_DIO3_MASK                  0xFC
#define RFLR_DIOMAPPING1_DIO3_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO3_01                    0x01
#define RFLR_DIOMAPPING1_DIO3_10                    0x02
#define RFLR_DIOMAPPING1_DIO3_11                    0x03

void RA02_GPIO_Init(void);
void RA02_EXTI_Int_Init(void);

void SX1276_Init(uint32_t fre , uint8_t bw);
void Lora_StandbyMode(void);
void Lora_RxMode();
void Lora_TxData(char* buf, uint8_t size);

int8_t Lora_GetSNR(void);
double Lora_GetRSSI(void);
uint8_t Lora_GetGain(void);

uint8_t SX1276_SpiSendByte(uint8_t outData );
void  SX1276Write(uint8_t addr , uint8_t *buffer , uint8_t size);
void  SX1276Read(uint8_t addr , uint8_t *buffer , uint8_t size);

void  SX1276_Reset( void );
void  SX1276_SetOpMode( uint8_t opMode );  // ���� ����ģʽ
void  SX1276LoRaSetRFFrequency( uint32_t freq );
void  SX1276LoRaSetSignalBandwidth( uint8_t bw );
void  SX1276LoRaSetSpreadingFactor( uint8_t factor );
void  SX1276LoRaSetErrorCoding( uint8_t value );
void  SX1276LoRaSetPacketCrcOn( bool enable );
void  SX1276LoRaSetImplicitHeaderOn( bool enable );
void  SX1276LoRaSetPAOutput( uint8_t outputPin );
void  SX1276LoRaSetSymbTimeout( uint16_t value );
void  SX1276LoRaSetLowDatarateOptimize( bool enable );

#endif 

