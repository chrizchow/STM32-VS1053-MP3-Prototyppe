/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "ff.h"	
#include "lcd.h"
#include <string.h>
#include "sdcard.h"
#include "vs10xx.h"
#include "delay.h"
#include "usart_printf.h"
//#include <stdio.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
GPIO_InitTypeDef GPIO_InitStructure;

/* Private define ------------------------------------------------------------*/
#define BlockSize            512 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define NumberOfBlocks       2  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks) >> 2)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//SD_CardInfo SDCardInfo;
uint32_t Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
uint32_t Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;


/* Extern variables ----------------------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);
TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
TestStatus eBuffercmp(uint32_t* pBuffer, uint16_t BufferLength);

/* Private function prototypes -----------------------------------------------*/
volatile u32 nCount = 0;
void LongDelay(u32 nC);

  FRESULT result;
  FATFS fs;
  FIL file;
  FIL g;
  DIR DirInf;  

  u16 br;
  u8 *databuf;
  
  u16 i=0;
  
  uint32_t bw;
  //char buf[1024];
  
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

int main(void)
{
  SystemInit();
  
  /* Clock Config */
  RCC_Configuration();

  /* Interrupt Config */
  NVIC_Configuration();
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);
  
  STM3210E_LCD_Init(); 
  LCD_Clear();

  USART_printf_init();
  Status = SD_Init();
  
  printf("\n\rSD_Init has run\n\r");
  
  /**************************Code Starting Point****************************************/
  char title[14] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
  LCD_DrawString(1, 1, title, 14);
      
  result = f_mount(&fs, "0", 1);
  
  if (result != FR_OK)
  {
    printf("Mount Filesystem Failed (%d)\r\n", result);
  }
  
  result = f_opendir(&DirInf, "/");
  if (result != FR_OK) 
  {
    printf("Open root directory failed! (%d)\r\n", result);
    return;
  }
  /*
  result = f_open(&file, "armfly.txt", FA_OPEN_EXISTING | FA_READ);
  if (result !=  FR_OK)
  {
    printf("Cannot Find File : armfly.txt\r\n");
    return;		
  }
  
  result = f_read(&file, &buf, sizeof(buf) - 1, &bw);
  if (bw > 0)
  {
    buf[bw] = 0;
    printf("\r\narmfly.txt Content: \r\n%s\r\n", buf);
  }
  else
  {
    printf("\r\narmfly.txt Content: \r\n");		
  }
  
  f_close(&file);
  */
  delay_init(72);
  VS_Init();
  //VS_Sine_Test();

  while(1){
  
    //Play Song
    result  = f_open(&file, "song.mp3", FA_OPEN_EXISTING | FA_READ);

    VS_Restart_Play();  					//重启播放 
    //VS_Set_All();        					//设置音量等信息 	
    VS_Set_Vol(200);
    VS_Reset_DecodeTime();					//复位解码时间 	
    
    if (result == FR_OK){
      printf("f_open of song.mp3 successfully\n");
      //VS_SPI_SpeedHigh();	//高速	
      VS_SPI_SpeedLow();
      
      while(1){
        databuf = malloc(480);
        if(databuf == 0){
          printf("Not enough memory.\n");
          return 0;
        }
        
        result = f_read(&file, databuf, 480, (UINT*)&br);//读出4096个字节
        i=0;
        
        do{
          //printf("%s", databuf+i);
          if(VS_Send_MusicData(databuf+i)==0){i+=32;}
        }while(i<480);
        
        free(databuf);
        
        if(br!=480 || result!=FR_OK){
          printf("f_read ends.\n");
          printf("Avg Bitrate: %d\n", VS_Get_ByteRate());
          
          break;
        }
      }
      f_close(&file);
    }else{
      printf("f_open of song.mp3 with ERROR!\n");
      return 0;
    }
  }
     
}


/***************************Code Ending Point****************************************/

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
}

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  *   : - BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/**
  *   Function name : Fill_Buffer
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLenght: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLenght; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  *   Function name : eBuffercmp
  * @brief  Checks if a buffer has all its values are equal to zero.
  * @param  pBuffer: buffer to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer values are zero
  *   FAILED: At least one value from pBuffer buffer is diffrent 
  *   from zero.
  */
TestStatus eBuffercmp(uint32_t* pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != 0x00)
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  
  while (1)
  {
    LCD_Clear();
    char ty [8] = {'F','a','i','l','u','r','e','!'};
    LCD_DrawString(1, 1, ty, 8);
  }
}
#endif



/***************************************************************************/


/* DELAY  */

void LongDelay(u32 nC)
{
  volatile u32 nCount = nC;
  for(; nCount != 0; nCount--);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
