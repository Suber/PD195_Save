
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "actapi.h"
#include "port.h"
#include "driver.h"
#include "sddrv.h"
#include "sdfs.h"
#include "irq.h"
#include "chip_cfg.h"
#include "lang_id.h"
#include "ap_cfg.h"
#include "simulatorpc.h"


#define  COLORREF uint32
//FontID
//#define  FONT_TYPE_TINY           0xff
//#define  FONT_TYPE_TINY_INVERT    0xff
#define  FONT_TYPE_SMALL          0
//#define  FONT_TYPE_SMALL_INVERT   1
#define  FONT_TYPE_LARGE          4
//#define  FONT_TYPE_LARGE_INVERT   0xff
//#define      FONT_TYPE_DEFAULT      FONT_TYPE_SMALL
#define  FONT_TYPE_DEFAULT        FONT_TYPE_SMALL

//#define  DISPLAY_BUFFER_ADDR     0x5800

#define  Color_BLACK_def     0x00000000//黑 
#define  Color_WHITE_def     0x0000ffff//白 
#define  Color_RED_def       0x0000f800//红 
//#define  Color_YELLOW_def    0x0000fff2//黄 
#define  Color_YELLOW_def    0x0000e748//黄 
#define  Color_GREEN_def     0x000007e0//绿 
#define  Color_BULE_def      0x0000001f//蓝 
#define  Color_BULE1_def     0x0000753E//深蓝 
#define  Color_BULE2_def     0x0000335f//浅蓝 
#define  Color_MENU_def      0x0000fcc0//橙 
#define  Color_CYAN_def      0x00008cc1//青 
#define  Color_SAVE_def      0x0000ed80//保存背景 

#define  Color_MUSIC_def    0x00001d1f //0x000014bf//音乐模式下的背景
#define  Color_MUSIC_def1    0x0000092d
#define  Color_MENU_BAR      0x00002a6f
#define  Color_MENU_GRD      0x0000efbf
#define  Color_MENU_GRD1     0x00006c35
#define  Color_WAIT_GRD      0x00000410
#define  Color_TOP_BAR       0x0000A6BD

#define  Color_REC_def       0x00009BA9
#define  Color_REC_def1      0x0000C69C

#define  Color_FM_def1       0x0000D759


//下面根据 windows 画笔列出如下颜色表 
// Color_tab0106_def 中的 01表示表中第1行，06表示第6列 
// 可参照 windows 画笔的色表 
#define  Color_tab0101_def   Color_BLACK_def
#define  Color_tab0102_def   0x00008410
#define  Color_tab0103_def   0x00008000
#define  Color_tab0104_def   0x00008400
#define  Color_tab0105_def   0x00000400
#define  Color_tab0106_def   0x00000410
#define  Color_tab0107_def   0x00000010
#define  Color_tab0108_def   0x00008010
#define  Color_tab0109_def   0x00008408
#define  Color_tab0110_def   0x00000208
#define  Color_tab0111_def   0x0000041f
#define  Color_tab0112_def   0x00000210
#define  Color_tab0113_def   0x0000401f
#define  Color_tab0114_def   0x00008200
#define  Color_tab0201_def   Color_WHITE_def
#define  Color_tab0202_def   0x0000c618
#define  Color_tab0203_def   Color_RED_def
#define  Color_tab0204_def   0x0000ffe0
#define  Color_tab0205_def   Color_GREEN_def
#define  Color_tab0206_def   0x000007ff
#define  Color_tab0207_def   Color_BULE_def
#define  Color_tab0208_def   0x0000f81f
#define  Color_tab0209_def   0x0000fff0
#define  Color_tab0210_def   0x000007f0
#define  Color_tab0211_def   0x000087ff
#define  Color_tab0212_def   0x0000841f
#define  Color_tab0213_def   0x0000f810
#define  Color_tab0214_def   0x0000fc08

/*菜单滚动背景颜色 */
#define Color_BULE_Scroll_def  0x0000001f//深蓝 

#define SCROLL_REGION_X        0 //45    //30
#define SCROLL_REGION_Y       24
#define SCROLL_REGION_WIDTH    128//140   //87
#define SCROLL_REGION_HEIGHT   16
	  
//#define FirstSectionCode    0xa1
#define DisplayBufLen       1024//468//384//显示缓冲区的大小 

#define ASCIIDISCODE        0x80
#define FirstSectionCode    0xa1
#define DisplaySuppPage     0x08//0x06//0x08
//lcd use 
#define LCMRST_GIO_EN_REG        GPIOEOUTEN    //GPIO_C0 (Output)
#define LCMRST_GIO_DATA_REG      GPIOEDAT
#define LCMRST_GIO_EN_BIT        0x10
#define LCMRST_SET_BIT           0x10
#define LCMRST_CLR_BIT           0xEF

#define LCMBL_GIO_EN_REG        GPIOCOUTEN        //GPIO_C1 (Output)
#define LCMBL_GIO_DATA_REG      GPIOCDAT
#define LCMBL_GIO_EN_BIT        0x02
#define LCMBL_SET_BIT           0x02
#define LCMBL_CLR_BIT           0xFD  

#define LCMRS_GIO_EN_REG        GPIOBOUTEN        //GPIO_B4 (Output) 
#define LCMRS_GIO_DATA_REG      GPIOBDAT
#define LCMRS_GIO_EN_BIT        0x10
#define LCMRS_GIO_DN_BIT        0xEF
#define LCMRS_SET_BIT           0x10
#define LCMRS_CLR_BIT           0xEF

#define CE_GIO_EN_REG           GPIOAOUTEN        //GPIO_A7 CS pin 
#define CE_GIO_DATA_REG         GPIOADAT
#define CE_EN_BIT               0x80      //ce enable
#define CE_DN_BIT               0x7F
#define CE_SET_BIT              0x80
#define CE_CLR_BIT              0x7F 


//=========================
#ifdef QVGA_LCM
#define Display_Length      242
#define Display_Height      320
#else
#define Display_Length     128//64 //128
#define Display_Height      64//48//64
#endif

#define Lcd_acturalwidth  128//64//128  /*LCD 模组实际的分辨率的实际长度*/
#define Lcd_acturalheight 64//48//64 /*LCD模组分辨率的实际宽度*/

//for not actural size lcd
//Lcd_widthoffset = (Lcd_acturalwidth - Display_Length)/2
//Lcd_heightoffset (Lcd_acturalheight - Display_Height)/2

#define Lcd_widthoffset 2    /*132X162 的LCD居中显示时，相对于LCD模组长度的起始偏移*/
#define Lcd_heightoffset 1   /*132X162 的LCD居中显示时，相对于LCD模组宽度的起始偏移*/

#define	  AuxiliaryBufferLen	   54
#define   RowWordsCountMax	   26

//定义三个刷屏相关buf的RAM标号 
#define LCD_RAM_INDEX      0
#define JPEG_RAM_INDEX     1
#define MURAM1L_RAM_INDEX  2
#define MURAM1L_RAM_USB    3

#define JPEG_BUF_ADDR 	 0x3000
#define LCD_BUF_ADDR  	 0x2400
#define MURAM1L_BUF_ADDR 0x5000

#define JPEG_BUF_LEN  	0x2000	//8K
#define LCD_BUF_LEN   	0x400	//1K

#define DMA_RGB    0
#define DMA_YCBCR  1

//lcd cmd
#define LCD_CMD_SETX     0x2a
#define LCD_CMD_SETY     0x2b
#define LCD_CMD_WRITE    0x2c
#define LCD_CMD_READ     0x2e
#define LCD_CMD_SETMODE  0x36

//display mode

#define RGBFLAG     0x08
#define MXENDFLAG   0x01
#define MYENDFLAG   0x02
#define MVENDFLAG   0x03
//add for less mode 
#define MYREVERSE   0x04

#ifdef __C251__
#include <typeext.h>


typedef struct
{
    COLORREF PenColor;      //画笔和字体颜色 
    COLORREF BackgdColor;   //背景颜色 !!
}DC, *PDC;


#ifdef QVGA_LCM
typedef struct
{
    uint16 x;         //列位置 
    uint16 y;         //行位置 
    uint16 width;     //宽度	 
    uint16 height;    //高度 
}region_t;
#else
typedef struct
{
    uchar x;         //列位置 
    uchar y;         //行位置 
    uchar width;     //宽度	 
    uchar height;    //高度 
}region_t;
#endif



/***************************UI驱动API接口******************************/
/**********************************************************************/
/**************************1.设置相关*********************************/
/**************************(a) RCODE接口*******************************/
void SetAsciiFont(uint8 FontID);           //设置Ascii字型  FontID见上定义 
void SetTextPos(uint8 x, uint8 y); //设置屏幕坐标  x:0~127  y:0~96
void SetPenColor(COLORREF color);
void SetBackgdColor(COLORREF color);
void SetTextOutMode(uint8 mod);   //前4位表示是否要反相1表示反相,0表示不反相,后4位表示透明度0-15,0表示不透明，
void SetTranMode(uint8 Mode);             //设置传送数据的模式 
void SetDrawMode(uint8 mode);            //设置屏幕的横向还是竖向 
void SetStrEndPosition(uint8 str_endx); 
void SetWindow(region_t *winregion);    //设置显示窗的区域,winregion,窗口定义 

/**************************(b) BANK接口*******************************/
void SetContrast(uint8 ContrastValue);            //设置屏幕对比度 
bool SetLanguage(uint8 lang_id);                  //设置多国语言选项 
//void SetBrightness(char BrightnessValue);      //设置屏幕亮度 
/********************************************************************/

/***********************2.获取状态接口********************************/
/**************************(a) RCODE接口*******************************/
void  GetFontPoint(uint16 code_rename, uint8 *picbuf);      //取字符的点阵. code:Ascii码，GB码，BIG5码 picbuf:点阵buffer
char* GetDisplayBufAddr(void);                          //取得显示Buffer地址。	  
void  GetTextPos(uint8 *x, uint8 *y);   //取屏幕坐标  
void  GetDCStatus(PDC pdc);                            //取得绘图设备的状态，如颜色值，背景色等 
uint8  GetOutPutErr(void);                             //取出错号	
//uint8 GetStringLength(char *str, uint16 str_length, char str_type); 
uint8 GetStringLength(uint8 *str,uint16 CharCount,uint8 str_type);
/********************************************************************/

/***********************3.绘图API接口**********************************/
/**************************(a) RCODE接口*******************************/
uint16  FontCodeMBToUni(uint16 coderename);  //将多国语言内码转换为unicode码 

/**************************(b) BANK接口*******************************/
char* PutUnS(const uint8 *str, int8 strlen);   
char* PutS(const uint8 *str, int8 strlen); 
void  PutSDImage(region_t *imageregion, uint32 sdimageaddr);  //从Flash读图画出来显示  sdimageaddr:Flash地址
//add for trans deal
//void Transdeal(uint16 transcnt,uint16 picwidth,region_t *pImRgn,uint8 *psrcbuf);
void Transdeal(uint8 *psrcbuf,uint16 transcnt);
//==============================================================
void  InvertRegion(region_t *invregion);                   //反白 
void  RegionCopy(region_t *sregion, region_t *tregion);   //区域复制 
//void RegionMove(region_t *sregion, region_t *tregion); //区域移动  
uint8  CharOutRect(uint8 *str, region_t *CRregion, int8 colcnt, uint8 language);
uint8 *PutStringExt(int8 *str, int8 strlen, int8 IsUnicode, int8 mode);
void  FillRect(region_t* region);
void  DrawRectFrame(region_t* region );
void  TextOutRect(region_t* region, uint8 *str);
void  Draw3DRect(region_t* D3region, uint8 type); 
//================================================================
uint8 ThaiPoint(const uint8 * str, uint8 IsUnicode, uint16 sorcecode,uint8* charwidth);
uint8 ThaiCharOutRect(uint8*str, region_t *CRregion, uint8 colcnt,uint8 mode);
uint8 ArabicCopy(uint8 *Dst,uint8 *src,int8 Length,uint8 codeType);
bool GetStringLengthExt(uint8 *str, uint16 str_length, uint8 str_type, uint8 * ResCharCount, uint8 * ResColCount);
/********************************************************************/

/***************************4.其他接口********************************/
/**************************(a) RCODE接口*******************************/
void BuffDataTrans(uint8 *buff, uint16 datacnt);    //把BUFF中数据送到LCM，buff：数据BUFF的首地址，datacnt：数据个数 
void DispBufSelect(uint8 rampage);      //选择buffer在zram哪个Page。USB专用函数 
void DMA3Init(uint8 pictype);

//#ifdef QVGA_LCM
	bool DMA3StartTrans(uint8 dma3_ram);	 //开始DMA3传输，dma3_ram=0, jpeg ram;dma3_ram=1,muram1m. ret=true---success; false---fail
//#else
	//bool DMA3StartTrans(void);				//开始DMA3传输，true---success; false---fail
//#endif

void DMA3SetCounter(uint8 row, uint16 col);	 //设置DMA3 counter,注意length不要超过min[DMA3counter， Memory length]
void DMA3SetSrcAddr(uint16 src_addr);  //设置memory起始地址 
void DMA3SetClock();	//选择DMA3 clock      
/**************************(b) BANK接口*******************************/
void StandbyScreen(bool bStandby);                 //开关显示屏  bStandby=0:关显示屏 bStandby=1:开显示屏
void ClearScreen(region_t *clrregion);            //清屏 clrregion:清屏区域 clrregion=NULL表示清整个屏幕
void UpdateScreen(const region_t *region);       //更新屏幕  region:更新区域  region=NULL表示更新整个屏幕
void LightOnOff(bool OnOff);                    //开关背光（true: 开, false:关） 
/********************************************************************/
/********************************************************************/
/********************************************************************/
//extern uint8 sysverflag;
//extern  uint32 FontLibAddr;
//extern  uint32 AsciiFontLibAddr;
//extern  DLL_IMP uint8 DisplayBuffer[];
//extern  uint32 MBToUniTabAddr;

//extern uint8 LcdYDataStartAddr[];
//extern uint8 LcdCCDataStartAddr[];

#endif/*__C251__*/

/**************************出错消息********************************/
#define  RESULT_PUTCHAR_OK            0//正常输出 
#define  RESULT_PUTCHAR_X_ERR         1//X轴超出 
#define  RESULT_PUTCHAR_Y_ERR         2//Y轴超出 
#define  RESULT_PUTCHAR_OTHER_ERR     3//其他错 

/**************************数据传送模式定义********************************/ 
#define  NORTransMode_def  0    //用普通的传送模式 
#define  DMATransMode_def  1   //用DMA传送模式 

/**************************standby mode 定义********************************/  
#define  StandbyOFF_def   0     //关显示屏 
#define  StandbyON_def    1    //开显示屏 
#define  UNICODELANGUAGE  0x7f    //字符资源为unicode编码模式  

#define UNICODEARABICDONTJOIN 3
#define ISNARABICDONTJOIN  4

//for arabic ligature
#define  ARABICHASJOIN    -2

/**************************用图方向模式定义********************************/ 
#define  DrawModeH_def      0  //横向优先 
#define  DrawModeV_def      1  //纵向优先   
#define  DrawModeHPic_def   2  //看图横向模式 
#define  DrawModeVPic_def   3  //看图纵向模式 
#define  DrawModeAvi_def    4  //AVI模式 
#define  DrawModeJpg_def    5  
#define  DrawModeBMP_def    6

/**************************滚屏模式的定义********************************/ 
//#define  SCMFull_def    0    //全屏滚模式 
//#define  SCMUp_def      1    //上屏滚模式 
//#define  SCMDown_def    2     //下屏滚模式 
//#define  SCMMid_def     3    //中间滚模式 

/**************************字符的点阵模式********************************/ 
//#ifdef   UNICODE_SUPPORT
#define  UnicodeMode      0    //16*16点阵模式 
#define  PicDisMode       1    //图形显示模式 
/*#else
#define  CharMode0608   0    //06*08点阵模式 
#define  CharMode0816   1    //08*16点阵模式 
#define  CharMode1616   2    //16*16点阵模式 
#define  PicDisMode     3    //图形显示模式  
#endif*/

/**************************定义3d效果********************************/ 
#define  D3Normalframe_def  0 //只加边框 
#define  D3KeyUp_def        1 //突出效果 
#define  D3KeyDown_def      2 //凹进效果 

/**************************定义3d图标效果颜色********************************/ 
#define  FaceLightColor_def  Color_tab0202_def   //向光边的颜色 
#define  BackLightColor_def  Color_tab0102_def   //背光边的颜色 
#define  SelectedColor_def   Color_BLACK_def     //被选中图片的边框颜色 

//#ifdef  UNICODE_SUPPORT
/**************************定义UNICODE字符 SIZE*****************************/
#define UNICODE_HEIGHT_DEF     16
#define UNICODE_WIDTH_DEF      16
//#else
/**************************定义ascii字符及汉字点阵的 SIZE********************/
/*#define ASCII_SMALL_HEIGHT_DEF  8
#define ASCII_SMALL_WIDTH_DEF   6
#define ASCII_LARGE_HEIGHT_DEF  16
#define ASCII_LARGE_WIDTH_DEF   8
#define CHINESE_HEIGHT_DEF      12
#define CHINESE_WIDTH_DEF       12
#define CHINESE_HEIGHT_DEF1     16
#define CHINESE_WIDTH_DEF1      16
#endif*/


#ifdef __A251__

//RCODE
#define API_UpdateScreen        0x01
//#define API_SetTextOutMode      0x02
#define API_SetAsciiFont        0x02
#define API_SetStrEndPosition   0x03
#define API_BuffDataTrans       0x04
#define API_SetWindow           0x05
#define API_BackLightOnOff      0x06
#define API_SetContrast         0x07
#define API_StandbyScreen       0x08

//BANKA
//BANKA *****************************************************
// 请注意，这里不能随意修改，如修改要与 DRV 中的 .xcl 文件中相应段对应
#define API_ClearScreen         0x0000 + API_UI_BaseNoA
#define API_PutSDImage          0x0001 + API_UI_BaseNoA
#define API_FillRect            0x1002 + API_UI_BaseNoA
#define API_GUI_BIDI_IsNSM      0x0003 + API_UI_BaseNoA
//#define API_ThaiOverlap			0x0004 + API_UI_BaseNoA
//#define API_FontCodeMBToUni		0x1004 + API_UI_BaseNoA
#define API_TurnArabicToUn      0x0005 + API_UI_BaseNoA
#define API_MsgBox              0x0006 + API_UI_BaseNoA


//BANKA *****************************************************

//BANKB *****************************************************
#define API_LanguageSet         0x2000 + API_UI_BaseNoB
#define API_Hdrv_HardInit       0x3000 + API_UI_BaseNoB
#define API_ShowLogo            0x0001 + API_UI_BaseNoB
#define API_RegionCopy          0x0002 + API_UI_BaseNoB
#define API_CharOutRect         0x1002 + API_UI_BaseNoB
#define API_PutChar             0x0003 + API_UI_BaseNoB
#define API_PutS                0x1003 + API_UI_BaseNoB
#define API_PutUnS              0x2003 + API_UI_BaseNoB
#define API_GetFontPoint        0x3003 + API_UI_BaseNoB
#define API_PutStringExt       	0x4003 + API_UI_BaseNoB
#define API_SetTextPos          0x5003 + API_UI_BaseNoB
#define API_GetTextPos          0x6003 + API_UI_BaseNoB

#define API_InvertRegion        0x0004 + API_UI_BaseNoB

#define API_GetStringLength     0x0005 + API_UI_BaseNoB
#define API_UnicodeTOArabic     0x0006 + API_UI_BaseNoB
#define API_CopyStr      		0x0007 + API_UI_BaseNoB
#define API_GetStringLengthExt  0x0008 + API_UI_BaseNoB

#define API_ThaiOverlap			0x0009 + API_UI_BaseNoB
#define API_FontCodeMBToUni		0x1009 + API_UI_BaseNoB
//BANKB



 
#endif/*__A251__*/
#endif/*_DISPLAY_H*/