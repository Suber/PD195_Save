/*
 ********************************************************************************
 *                       ACTOS
 *                  driver demo
 *
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : Hdrv_HardInit.c
 * By     :	fiona
 * Version: 1> v1.00     first version     date	2010/9/21
 ********************************************************************************
 */
#include "ak211x.h"
#include "actos.h"
#include "display.h"
#include "Hdrv_S7565.h"
#include "string.h"

void Hdrv_ClearScreen(void);

#pragma userclass(hconst=HARD_INITDATA)
//extern int8 reent_flag; //store CE flag
const uint8 far LcdInitBuf[] =
{
#if(LCD_TYPE==1)    //SSD1303
	0xfd,0xae,
	0xfd,0xad,
	0xfd,0x8a,
	0xfd,0xa8,
	0xfd,0x3f,
	0xfd,0xd3,
	0xfd,0x00,
	0xfd,0x40,
	0xfd,0xa0,
	0xfd,0xc0,
	0xfd,0xA6,
	0xfd,0xA4,
	0xfd,0X81,
	0xfd,0x5e,
	0xfd,0xD5,
	0xfd,0x61,
	0xfd,0xd8,
	0xfd,0x05,
	//display on
	0xfd,0xAF,
	0xff

#elif((LCD_TYPE==2)||(LCD_TYPE==3)||(LCD_TYPE==4))  //SSD1306
    /*
    0xfd,0xae,
	0xfd,0x00,
	0xfd,0x10,
	0xfd,0x2e,
	0xfd,0x81,
	0xfd,0x32,  //contrast
	0xfd,0x82,
	0xfd,0x80,
	0xfd,0xa6,
	0xfd,0xa8,
	0xfd,0x3f,
	0xfd,0xa1,  //0xa0
	0xfd,0xc0,  //0xc0
	0xfd,0xd3,
	0xfd,0x18,
	0xfd,0x40,  //0x41
	0xfd,0xad,
	0xfd,0x8e,
	0xfd,0xd5,
	0xfd,0xf0,
	0xfd,0xd8,
	0xfd,0x05,
	0xfd,0xd9,
	0xfd,0xf1,
	0xfd,0xda,
	0xfd,0x12,
	0xfd,0xdb,
	0xfd,0x34,
	0xfd,0x91,
	0xfd,0x3f,
	0xfd,0x3f,
	0xfd,0x3f,
	0xfd,0x3f,
	#if(LCD_TYPE==4)
    0xfd,0x8d,
	0xfd,0x14,
	#endif
	//display on
	0xfd,0xaf,
	0xff
	*/
	//SC903
	0xfd,0xae,
	0xfd,0xd5,
	0xfd,0x80,
	0xfd,0xa8,
	0xfd,0x3f,
    0xfd,0xd3,
	0xfd,0x00,
	0xfd,0x40,  //0x41
	0xfd,0x8d,
	0xfd,0x14,
	0xfd,0x20,
	0xfd,0x02,
	0xfd,0xa0,  //0xa0
	0xfd,0xc8,  //0xc0
	0xfd,0xda,
	0xfd,0x12,
	0xfd,0x81,
	0xfd,0xb4,  //contrast
	0xfd,0xd9,
	0xfd,0xf1,
	0xfd,0xdb,
	0xfd,0x40,
	0xfd,0xa4,
	0xfd,0xa6,
	//display on
	0xfd,0xaf,
	0xff

#else
    0xff
#endif

#if 0 //ST7579
	0xfd, 0x33, //H[1,0] =================== [1,1]
	0xfe, 5,
	0xfd, 0x03, //software reset
	0xfe, 5,
	0xfd, 0x33, //H[1,0] =================== [1,1]
	0xfe, 5,
	0xfd, 0x9a, //booster efficiency set (0x98)
	0xfe, 5,
	0xfd, 0x31, //H[1,0] =================== [0,1]
	0xfe, 5,
	0xfd, 0x13, //bias system
	0xfe, 5,
	0xfd, 0x08, //display config
	0xfe, 5,
	0xfd, 0xc6, //set Vop (0x80)
	0xfe, 5,
	0xfd, 0x41, //set start line
	0xfe, 5,
	0xfd, 0x30, //H[1,0] =================== [0,0]
	0xfe, 5,
	0xfd, 0x05, //PRS
	0xfe, 5,
	0xfd, 0x0c, //D&E
	0xfe, 5,
	0xfd, 0x40, //set Y address of RAM
	0xfe, 5,
	0xff
#endif

#if 0 //ST7579(bak)
	0xfd, 0x38, //Function Set Mx:0,My:0,Pd:0,================H1:0,H0:0
	0xfe, 10,
	0xfd, 0x05, //Set Vlcd range Higt
	0xfe, 10,
	0xfd, 0x0C, //Display control: normal Display
	0xfe, 10,
	0xfd, 0x39, //Function Set Mx:0,My:0,=====================H1:0,H0,1
	0xfe, 10,
//	0xfd, 0x80, //Display configuration LSB is on top
//	0xfe, 100,
	0xfd, 0x14, //System bias 1/36
	0xfe, 10,
	0xfd, 0x45, //Set Start Line :0 (0x45)
	0xfe, 10,
	0xfd, 0xc7, //Set Vop value (0xA0)
	0xfe, 10,
	0xfd, 0x38, //Function Set Mx:0,My:0,Pd:0,================H1:0,H0:0
	0xfe, 10,
	0xff
#endif

#if 0 //ST7565(1)
    //bias selec 1/6
	0xfd,0xae,
	//select(s131-s36)
	0xfd,0xd5,
	//select(com0-com31)
	0xfd,0x80,
	0xfd,0xa8,
	0xfd,0x3f,
	//contrast select
	0xfd,0xd3,
	//set reference voltage register
	0xfd,0x00,
	//delay 
	0xfd,0x40,
	0xfd,0x8d,
	//delay
	0xfd,0x14,
	0xfd,0x20,
	0xfd,0x02,

	0xfd,0xa1,
	0xfd,0xc0,
	0xfd,0xda,
	0xfd,0x12,
	0xfd,0x81,
	0xfd,0x00,
	0xfd,0xd9,
	0xfd,0xf1,
	0xfd,0xdb,
	0xfd,0x40,
	//display on
	0xfd,0xa4,
	0xfd,0xa6,
	0xfd,0x40,

	0xfd,0xb0,
	

	0xfd,0xaf,
	//entire display off
	//   0xfd,0xa4,
    0xff
#endif
};
#pragma userclass(hconst=default)

#pragma renamecode(HARD_INIT)
/************************************************************/
////********************************************************/
///*    void Hdrv_HardInit(void);
//** FUNCTION:     Hdrv_HardInit
//**
//** Description:  初始化LCM的硬件设备
//**
//**  input
//**     none
//**
//**  output
//**     none
//**********************************************************/
void Hdrv_HardInit(void)
{
    uint8 sfr_bak = SFR_BANK;
    SFR_BANK = BANK_CMU_RMU;
	//set mcu clock 24mhz
    MCSR4 |= 1 << 1;
    MCSR4 &= ~1;
	//set external memeory wait 
    ExWait = 0x06; //升频
	//set LCD Bank
	SFR_BANK = 0x06; 
	//set external memeory write/read voltage wait state counter
    EXTMEM_WT = 0x23; //升频
	//片选Chip select
    StoreCE();
/*
	//LCD RESET
    Hdrv_LcdReset(); 
	//write lcd init buf
    Hdrv_LcdInitBuf(); 
	//first clear screen
	Hdrv_ClearScreen();
	//set contrast
	//Hdrv_SetContrast(0x1e);
*/
/*
    //LCD RESET
    Hdrv_LcdReset(); 
	Hdrv_LcdInitBuf();
	//开背光
    Hdrv_LightOnOff(TRUE); 
*/    
	//片选release chip selcet
    RestoreCE(); 
    SFR_BANK = sfr_bak;
    return;
}

/************************************************************/
/*  function:LCD_Reset                                        */
/*  input:               output:                            */
/************************************************************/
void Hdrv_LcdReset(void)
{
	//======================================235案子显示屏为硬件复位，此处代码无意义

    uint8 sfr_bak = SFR_BANK;
    uint8 i = 0;
    SFR_BANK = BANK_GPIO; //set GPIO Bank
    //enable rest gpio
	LCMRST_GIO_EN_REG |= LCMRST_GIO_EN_BIT;
    //set reset gpio high
	LCMRST_GIO_DATA_REG |= LCMRST_SET_BIT;
    Delay_NMS(1);
    //clr reset gpio
	LCMRST_GIO_DATA_REG &= LCMRST_CLR_BIT;
    Delay_NMS(1);
    //set reset gpio high
	LCMRST_GIO_DATA_REG |= LCMRST_SET_BIT;
    Delay_NMS(1);
    SFR_BANK = sfr_bak;
    return;
}

/************************************************************/
//  function:LcdInitBuf command send                        
//  input:               
//  output:                            
/************************************************************/
void Hdrv_LcdInitBuf(void)
{
    uint8 sfr_bak = SFR_BANK;
    uint8 i = 0;

    while (LcdInitBuf[i] != 0xff)
    {
        if (LcdInitBuf[i] == 0xfd)
        {
            i++;
            Write_Command(LcdInitBuf[i]); //写CMD
        }
        else if (LcdInitBuf[i] == 0xfe)
        {
            i++;
            Delay_NMS(LcdInitBuf[i]);
        }
		else
		{
		  ;//do nothing
		}
		i++;
    }
    SFR_BANK = sfr_bak;
    return;
}
/************************************************************/
//    void Hdrv_ClearScreen(void);
//    FUNCTION: Hdrv_ClearScreen 
 
//    Description:clear the screen,insure when power on,
//    there is nothing on the screen  
                       
//    input:  
             
//    output:  
                          
/************************************************************/
void Hdrv_ClearScreen(void)
{
  uint8 pageloop=0; 
  uint8 setcolcmdH,setcolcmdL;
  uint8 begincol=0;
  //clear display buffer
  memset(DisplayBuffer,0x00,DisplayBufLen);
  //for eight page clear
  //Write_Command(0x30);
  //begincol += 0x20;
    #if((LCD_TYPE==3)||(LCD_TYPE==4))
    //begincol += 4;
    #endif
  setcolcmdH = ((begincol&0xf0)>>4)|0x10;
  setcolcmdL = begincol & 0x0f; 
  while(pageloop<8) //8
  {
    //set page munber command
	Write_Command(pageloop|0xb0);
	//set column bits
    Write_Command(setcolcmdH);
   	Write_Command(setcolcmdL);
	//sent data to lcd
	Write_Data(DisplayBuffer,Display_Length);
	pageloop++;
  }
}

