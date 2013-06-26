#include "actos.h"
#include "ak211x.h"
#include "Hdrv_S7565.h"
#include "string.h"
//extern void _nop_(void);
#define TALLESTPGNUM  0x07 //0x43//0XB7 large，在有些情况下，这里需要修改，如第一PAGE不是0XB7的时候
#pragma renamecode(HARD_LOGICAL)

//***************************************************************************
//
//** FUNCTION:    Hdrv_UpdateScreen
//** PURPOSE:     put buffer data to Screen
//** PARAMETERS:
//**  input
//**      updateregion 更新范围结构region_t的指针
//**  output
//**      none
//** DESCRIPTION:
//***************************************************************************/

void Hdrv_UpdateScreen(region_t *updateregion)
{
   uint8 pagenum;
   uint8 beginpage;
   uint8 begincol;
   uint8 updatewidth;
   uint16 coloffset;
   //set column heigh bits command
   uint8 setcolcmdL;
   uint8 setcolcmdH;
   //set column low bits command
   //NULL means update all screen 
   if(updateregion == NULL)
   {
   	 //8 pages will be updated 
	 pagenum = DisplaySuppPage;
	 //	beginpage = start page number
	 beginpage = 0;
	 //begincol = start column
	 begincol = 0; 
	 //update width
	 updatewidth = Display_Length;	 
	 //setcolcmdH = ((0x00 & 0x0f)>> 4)  | 0x10;
	 //setcolcmdL = 0 & 0x0f; 
   }
   else
   {
   	 //pagenum = pages will be updated 
	 pagenum = (updateregion->height/8);
	 beginpage = (updateregion->y/8);
	 begincol = updateregion->x;	 
	 updatewidth = updateregion->width;
	 //setcolcmdH = ((begincol&0xf0)>>4)|0x10;
	 //setcolcmdL = begincol & 0x0f;
   }
   coloffset = beginpage*Display_Length + begincol;
   //if width >screen width then use the screen width
   if((begincol+updatewidth)>(Display_Length+1))
   {
   	 updatewidth = Display_Length - begincol;
   }
   //if uddate width =0 do nothing
   if(updatewidth == 0)
   {
   	 return;
   }
   
   //begincol += 0x20;
   #if((LCD_TYPE==3)||(LCD_TYPE==4))
   //begincol += 4;
   #endif
   setcolcmdH = ((begincol&0xf0)>>4)|0x10;
   setcolcmdL = begincol & 0x0f;
   
   StoreCE();
   //Write_Command(0x30);
   while(pagenum)
   { 
	 //set column command 0xb7-0xb0
	 Write_Command((TALLESTPGNUM - beginpage)|0xb0);//0xb5
	 //set column bits 
	 Write_Command(setcolcmdH);
	  //set column low bits 
   	 Write_Command(setcolcmdL);	//
	 //send data to screen
	 Write_Data(&DisplayBuffer[coloffset],updatewidth);
	 //buffer offset add 
	 coloffset += Display_Length;
	 //begin page number add 
	 beginpage++;
	 //update page number dec
	 pagenum--;
   }
   RestoreCE();
}
////*******************************************************************************/
///* void Hdrv_BuffDataTrans ( uint8 *buff , uint16 len);
//** FUNCTION:     Hdrv_BuffDataTrans
//**
//** Description:  把指定 buff ，指定长度的数据送到 显示buffer 中
//**
//**  input
//**     uint8 *buff :数据 buffer 的首址
//**     uint16 len:传送数据的个数   
//**
//**  output
//**     none
//********************************************************************************/
void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len)
{
   uint8 loopcnt;
   uint16 curbufoffset;
   //uint8 line;
   //how many pages will be send to display buffer  
  // pagenum = region.height/8;
   //every page send bytes
   //len = len/pagenum;
   //start postion of display buffer

   curbufoffset = (region.y/8)*Display_Length + region.x;
#if 0   
   //every time send one page
   for(loopcnt = 0;loopcnt<pagenum;loopcnt++)
   {
   	 memcpy((&DisplayBuffer[curbufoffset]),pbuf,len);
	 pbuf += len;
	 curbufoffset += Display_Length;
   }    
#endif   
    if((CharPointBuffer[32] == 0)||TextColumn > Display_Length )
    {
        TextColumn = TextColumn + CharPointBuffer[32];
        return ;
    }
    for(loopcnt=0;loopcnt<AscFontSmall;loopcnt++)
    {
        if(TextColumn + CharPointBuffer[32]<Display_Length)
        {
            len = CharPointBuffer[32];
        }
        else
        {
            len = Display_Length - TextColumn;
        }
        memcpy((&DisplayBuffer[curbufoffset]),pbuf,len);
	    pbuf += CharWidth;
	    curbufoffset += Display_Length;
        
    } 
    TextColumn = TextColumn + CharPointBuffer[32];  
}
////*******************************************************************************/
///* void Hdrv_SetWindow(region_t *region);
//** FUNCTION:     Hdrv_SetWindow
//**
//** Description:  设置输出的窗口
//**
//**  input
//**        region_t *winreg： 要设置的输出窗口的范围
//**
//**  output
//**     none
//********************************************************************************/
void Hdrv_SetWindow(region_t *winreg)
{
    //设窗
    region = *winreg;
    if(CharPointBuffer[32]<9)
    {
        CharWidth = 8;
    }
    else
    {
        CharWidth = 16;
    }
    AscFontSmall = (region.y+region.height)/8 - (region.y/8);
}



void Sdrv_SwapIntByte(uint16 *pword)
{
    uint8 high_byte;
    uint8 low_byte;
    high_byte = (uint8) ((*pword >> 8) & 0xff);
    low_byte = (uint8) (*pword & 0xff);

    *((uint8*) pword + 1) = high_byte;
    *((uint8*) pword) = low_byte;
}

////*******************************************************************************/
///*    void Hdrv_StandbyScr(bool bStandby);
//**
//** FUNCTION:     Hdrv_StandbyScr
//**
//** Description:  进入或退出 standby
//**
//**  input
//**     bool bStandby:  True: 退出Standby;  False: 进入 Standby
//**
//**  output
//**     none
//********************************************************************************/
void Hdrv_StandbyScreen(bool bStandby)
{
   
    if (bStandby == FALSE)
    {      
		//enter standby
		StoreCE();
//        Write_Command(0xa4);
//        Write_Command(0xa6);        
		Write_Command(0xae);   
		RestoreCE();     

		//SetPLL(PLL_6MHZ);
        //SetVDD(VDDSET_1_4V);
    }
    else
    {
	  //exit standby
	  //  Hdrv_HardInit();
	  	StoreCE();
//       Write_Command(0xa4);
//        Write_Command(0xa6);        
		Write_Command(0xaf);   
		RestoreCE();  

        //SetPLL(PLL_24MHZ);
        //SetVDD(VDDSET_1_6V);
    }
    if (bStandby == FALSE)
    {
      Hdrv_LightOnOff(FALSE); 
    }
	else
	{
	  Hdrv_LightOnOff(TRUE); 
	  //   Hdrv_HardInit();
	}
}

////*******************************************************************************/
///*  void Hdrv_SetContrast(char ContrastValue);
//** FUNCTION:     Hdrv_SetContrast
//**
//** Description:  设置对比度
//**
//**  input
//**     char ContrastValue: 要设置的对比度的值0~31
//**
//**  output
//**     none
//**  other
//**     调用间隔不能小于半秒
//********************************************************************************/
void Hdrv_SetContrast(uint8 ContrastValue)
{
#if 1
	StoreCE();
	Write_Command(0x81);
	//Delay_NMS(10);
	Write_Command(ContrastValue*15); //0xB2 + 7 * 3 = 0xC7
	Delay_NMS(10);
	//Write_Command(0x38);
	RestoreCE();
#endif	
}
