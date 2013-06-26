#include "ak211x.h"
//#include <intrins.h>
#include "typeext.h"
//#include "lcd.h"
#include "string.h"
#include "display.h"
//#pragma src
extern void _nop_(void);
extern void Delay_NMS(uint8 delay_ms);
extern void StoreCE(void);
extern void RestoreCE(void);
extern void Write_Command(uint8 cmd);
extern void Write_Data(uint8* mydata, uint16 len);
extern void Write_WordData(uint8 mydata1, uint8 mydata2);
extern void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len);
extern uint32 sGetBuffInfo(void);
extern void sGetLogoData(uint8 cnt, uint16 addr);

#pragma userclass(far=HD_LCDData)
int8  far reent_flag =0;
uint8 far LCDDrawMode=0;
uint8 far PicMode = 0;
uint8 far cesel=0;
#pragma userclass(far=default)

#pragma name(HD_LCD)
#pragma renamecode(HD_LCD)
//8 BITS LCD
const char far LcdInitBuf[] =
{
#if(LCD_TYPE==1)    //SSD1303
    0xfe,100,
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
	0xfd, 0xff, //set Vop (0x80)
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
	0xfd,0x48,
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
	0xfd,0x48,
	0xfd,0xd9,
	0xfd,0xf1,
	0xfd,0xdb,
	0xfd,0x48,
	//display on
	0xfd,0xa4,
	0xfd,0xa6,
	0xfd,0x48,

	0xfd,0xb0,
	

	0xfd,0xaf,
	//entire display off
	//   0xfd,0xa4,
    0xff
#endif

#if 0 //ST7565(0)
	0xfd,0xae,
	//select(s131-s36)
	0xfd,0xa1,
	//select(com0-com31)
	0xfd,0xda,
	0xfd,0x12,
	0xfd,0xc8,
	//contrast select
	0xfd,0xa8,
	//set reference voltage register
	0xfd,0x3f,
	//delay 
	0xfd,0xd5,
	0xfd,0x80,
	//delay
	0xfd,0x81,
	0xfd,0x50,
	0xfd,0xd9,

	0xfd,0x21,
	0xfd,0x20,
	0xfd,0xdb,
	0xfd,0x30,
	0xfd,0xad,
	0xfd,0x00,
	0xfd,0xa4,
	0xfd,0xa6,
	0xfd,0xaf,
	//entire display off
	//   0xfd,0xa4,
    0xff
#endif    
};

void Hdrv_LcdReset(void)
{

//======================================235案子显示屏为硬件复位，此处代码无意义
    uint8 sfr_bak = SFR_BANK;
    uint8 i = 0;
    SFR_BANK = BANK_GPIO; //set GPIO Bank
    //ENABLE GPIOC.0
	LCMRST_GIO_EN_REG |= LCMRST_GIO_EN_BIT;
    //clr GPIOC0.0
	LCMRST_GIO_DATA_REG &= LCMRST_CLR_BIT;
    Delay_NMS(20);
    //SETB GPIOC0.0
	LCMRST_GIO_DATA_REG |= LCMRST_SET_BIT;
    Delay_NMS(60);
    SFR_BANK = sfr_bak;
    return;
}
void Hdrv_LcdInitBuf(void)
{
    uint8 sfr_bak = SFR_BANK;
    uint8 i = 0;
    SFR_BANK = BANK_LCD; //set LCD Bank
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
            //Write_Data(&LcdInitBuf[i],1); //写data
            ; //do nothing
        }
        i++;
    }
    SFR_BANK = sfr_bak;
}

//clear 162*132
void Hdrv_ClearScreen(void)
{
    uint8* databuf = (uint8*) 0x3000;
	//databuf max 0x1000, so everytime sent cnt must less than 0x1000
    uint8 loopcnt = 8; //8
	uint16 everycnt;
	uint8 pageloop = 0;
	uint8 setcolcmdH,setcolcmdL;
	uint8 begincol=0;
	everycnt = Lcd_acturalwidth*Lcd_acturalheight/loopcnt;	
    memset((void*) databuf, 0xff, everycnt);
	//begincol += 0x20;
    //#if((LCD_TYPE==3)||(LCD_TYPE==4))
    //begincol += 4;
    //#endif
	setcolcmdH = ((begincol&0xf0)>>4)|0x10;
	setcolcmdL = begincol & 0x0f;
	//Write_Command(0x30);
    while (pageloop < 8) //8
    {
        //set page munber command
    	Write_Command(pageloop|0xb0);
    	//set column bits
      	Write_Command(setcolcmdH);
    	Write_Command(setcolcmdL);
    	//sent data to lcd
    	Write_Data(databuf,Display_Length);
    	pageloop++;
    }
}
/********************************************************************************
 * Description : 显示图片，图片来源于welcome.res
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void Show_welcome_logo()
{
    
    uint8 sfr_bak = SFR_BANK;
    uint16 sec_cnt = 0;
    uint8 *buffer_addr = (uint8*) 0x3000;
    uint16 pageloop = 0;
    //uint8 tmp_cesel;
    uint8 setcolcmdH,setcolcmdL;
    uint8 begincol=0;
    SFR_BANK = BANK_CMU_RMU;
    MCSR4 |= 1 << 1;
    MCSR4 &= ~1;

    /*******EMIF Write Command********/
    ExWait = 0x06; //升频
    SFR_BANK = BANK_LCD; //set LCD Bank
    EXTMEM_WT = 0x23; //升频

    //StoreCE(&tmp_cesel); //片选CE5
     
    sGetBuffInfo();
    sGetLogoData(0x10, sec_cnt);
    //begincol += 0x20;
    //#if((LCD_TYPE==3)||(LCD_TYPE==4))
    //begincol += 4;
    //#endif
    setcolcmdH = ((begincol&0xf0)>>4)|0x10;
    setcolcmdL = begincol & 0x0f; 
    StoreCE();
	//Write_Command(0x30);
    while (pageloop < 8)    //8
    {
        //set page munber command
    	Write_Command((7-pageloop)|0xb0);//6
    	//set high column bits
      	Write_Command(setcolcmdH);
    	//set low colmun bits 
    	Write_Command(setcolcmdL);
    	//sent data to lcd
    	Write_Data(buffer_addr+ 10 + pageloop*Display_Length,Display_Length);
    	pageloop++;
    }
    //Write_Command(0xaf);
	RestoreCE(); 
    SFR_BANK = sfr_bak;
}


void Hdrv_LCDInit(void)
{
    uint8 sfr_bak = SFR_BANK;
    //uint8 tmp_cesel;

    SFR_BANK = BANK_CMU_RMU;
    MCSR4 |= 1 << 1;
    MCSR4 &= ~1;

    /*******EMIF Write Command********/
    ExWait = 0x06; //升频
    SFR_BANK = BANK_LCD; //set LCD Bank
    EXTMEM_WT = 0x23; //升频

    //StoreCE(&tmp_cesel); //片选CE5
    StoreCE();
    Hdrv_LcdReset(); //LCD RESET    //SC903
    Hdrv_LcdInitBuf(); //write lcd init buf

    /**********test data*************
     Write_Command(LCD_CMD_WRITE);
     Write_Display_Ram(); //RS=1

     SFR_BANK = LCD_PAGE_P;
     for(i=0; i<100; i++)
     {
     for(j=0; j<50; j++)
     {
     //EXTMEM_DL = 0;
     //EXTMEM_DL = 0x1f;
     Write_Data(0);
     Write_Data(0x1f);
     }
     }
     */
//    Hdrv_ClearScreen();
    //RestoreCE(tmp_cesel); //片选CE5
	RestoreCE(); 
    SFR_BANK = sfr_bak;
}
