/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* set LCD command */
#define Reset_LCD         0xe2

#define Set_Start_Line_X  0x40
#define Set_Page_Addr_X   0xb0
#define Set_ColH_Addr_X   0x10
#define Set_ColL_Addr_X   0x0

#define LCD_Reset   0xe2
#define Display_Off   0xae
#define Display_On    0xaf
#define Set_ADC_Normal    0xa0
#define Set_ADC_Reverse   0xa1
#define Set_LCD_Bias_7    0xa3
#define Set_LCD_Bias_9    0xa2
#define RMW_Mode_En       0xe0
#define RMW_Mode_Dis      0xee
#define COM_Scan_Dir_Normal 0xc0
#define COM_Scan_Dir_Reverse  0xc8

#define Set_Resistor_Ratio_X   0x20
#define Set_Ref_Vol_Mode       0x81
#define Set_Ref_Vol_Reg        0x20

#define Display_Normal    0xa6
#define Display_Reverse   0xa7
#define Display_All_On    0xa5
#define Display_All_Normal  0xa4


// ZYMG12864
/*A0=0  -- cmd*/
#define LCD_Command  *((volatile unsigned char * )0x6c000000)
/*A0=1 -- data*/
#define LCD_Data  *((volatile unsigned char * )0x6c000001)



/*define the constant for display digital char*/
#define DSpace	0x20	//" "
#define DDash	0x2D	//"-"
#define	DPoint	0x2E	//"."
#define	D0	0x30
#define	D1	0x31
#define	D2	0x32
#define	D3	0x33
#define	D4	0x34
#define	D5	0x35
#define	D6	0x36
#define	D7	0x37
#define	D8	0x38
#define	D9	0x39
#define DColon	0x3A	//":"

#define DA	0x41
#define DB	0x42
#define DC	0x43
#define DD	0x44
#define DE	0x45
#define DF	0x46
#define DG	0x47
#define DH	0x48
#define DI	0x49
#define DJ	0x4A
#define DK	0x4B
#define DL	0x4C
#define DM	0x4D
#define DN	0x4E
#define DO	0x4F
#define DP	0x50
#define DQ	0x51
#define DR	0x52
#define DS	0x53
#define DT	0x54
#define DU	0x55
#define DV	0x56
#define DW	0x57
#define DX	0x58
#define DY	0x59
#define DZ	0x5A

#define Da	0x61
#define Db	0x62
#define Dc	0x63
#define Dd	0x64
#define De	0x65
#define Df	0x66
#define Dg	0x67
#define Dh	0x68
#define Di	0x69
#define Dj	0x6A
#define Dk	0x6B
#define Dl	0x6C
#define Dm	0x6D
#define Dn	0x6E
#define Do	0x6F
#define Dp	0x70
#define Dq	0x71
#define Dr	0x72
#define Ds	0x73
#define Dt	0x74
#define Du	0x75
#define Dv	0x76
#define Dw	0x77
#define Dx	0x78
#define Dy	0x79
#define Dz	0x7A


/* Exported macro ------------------------------------------------------------*/
extern u8 String1[10];
extern u8 String2[2];
extern u8 HexValueOffset[16];
extern u8 DispSTLoGoTable[];
extern u8 tempbuffer[];		
extern u8 ChineseTable[][16];

extern u8 StringTest1[12];  //----------- darwin
extern u8 StringTest2[5];   //------------darwin
extern u8 HValueOffset[16];

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void STM3210E_LCD_Init(void);

void LCD_SetBackColor(vu16 Color);
void LCD_ClearLine(u8 Line);
void LCD_Clear(void);

void LCD_DrawDot(u8 Xpos, u8 Ypos);
void LCD_DrawChar(u8 Xpos, u8 Ypos, u8 offset);
void LCD_DisplayChar(u8 Line, u16 Column, u8 Ascii);
void LCD_DisplayStringLine(u8 Line, u8 *ptr);
void LCD_SetDisplayWindow(u8 Xpos, u16 Ypos, u8 Height, u16 Width);
void LCD_WindowModeDisable(void);
void LCD_DrawLine(u8 Xpos1, u8 Ypos1, u8 Xpos2, u8 Ypos2); 
void LCD_DrawRect(u8 Xpos, u16 Ypos, u8 Height, u16 Width);
void LCD_DrawCircle(u8 Xpos, u8 Ypos, u8 Radius, int fill);
void LCD_DrawMonoPict(uc32 *Pict);
void LCD_WriteBMP(u32 BmpAddress);

/*----- Medium layer function -----*/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);

void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);

void LCD_PowerOn(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);

/*----- Low layer function -----*/
void LCD_CtrlLinesConfig(void);
void LCD_FSMCConfig(void);

void delay(void);
void LCD_Draw_ST_Logo(void);
void LCD_Reset_Cursor(void);
void LCD_Clr_Cursor(s8 x);
void LCD_Set_Cursor(s8 x);
u8 LCD_DrawString(u8 Xpos, u8 Ypos, u8 *c, u8 length);
#endif /* __LCD_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
