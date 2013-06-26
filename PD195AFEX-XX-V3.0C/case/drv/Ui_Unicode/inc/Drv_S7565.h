#ifndef _Drv_S7565_H
#define _Drv_S7565_H

#include "display.h"

#include "sysdef.h"
//#include "Hdrv_S7565.h"

#ifdef __A251__
#endif /*__251_SYSTEMS_ASM*/

#ifdef __C251__

// 函数表
void Drv_Entrance(void);

bool Sdrv_Init(uint8 mode);
void Sdrv_Exit(void);

//for clear screen
void Sdrv_ClearScreen(region_t *clrregion);

void Sdrv_SetContrast(uint8 ContrastValue);

//for font size
void Sdrv_SetAsciiFont(uint8 FontID);


uint8 BAPI_UnicodeTOArabic(uint8 *str,uint8 length);
//for put out char
int8 * Sdrv_PutS(int8 *str, int8 strlen);
void ConvertEnglish(int8 *str,int8 CharCount);  
int8 * Sdrv_PutUnS(int8 *str, int8 strlen);
uint8 Sdrv_PutChar(uint8 *str, uint8 IsUnicode, uint16 ucode);
//for picture display
void Sdrv_PutSDImage(region_t *pRgnImage, uint32 sdimageaddr);
bool BAPI_GUI_BIDI_IsNSM(uint16 c);
//add for trans deal
//void Sdrv_GetFontPoint(uint16 code_rename, uint8 *picbuf);
void Sdrv_GetFontPoint(uint16 ucode,uint16 picbuf);
void Sdrv_AsciiGetFontPoint(uint16 code_num,uint8* databuf);
extern void Hdrv_SetWindow(region_t *winreg);
extern void Hdrv_UpdateScreen(region_t *updateregion);
extern void Write_Command(uint8 cmd);
extern void Write_Data(uint8* mydata,uint16 len);
bool Sdrv_LanguageSet(uint8 lang_id);
uint16 Sdrv_FontCodeMBToUni(uint16 code_rename);
uint8 Sdrv_CharOutRect(uint8 *str, region_t *CRregion, int8 colcnt, uint8 language);
void Sdrv_SetTextPos(uint8 x, uint8 y);

void Sdrv_GetTextPos(uint8 *x,uint8 *y);
void Hdrv_BuffDataTrans(uint8* pbuf, uint16 len); 

int8 BAPI_CopyStr(int8 *Dst,int8 *src,int8 CharCount,uint8 IsSwap);
int8 CopyStr(int8 *Dst,int8 *src,int8 CharCount,uint8 IsSwap);
int8 TurnArabicToUn(int8* charbuf, int8 IsUnicode, int8 CharCount);
int8 BAPI_TurnArabicToUn(int8* charbuf, int8 IsUnicode, int8 CharCount); 
//uint8 Sdrv_GetStringLength(uint8 *str,int8 CharCount,uint8 str_type);
uint8 Sdrv_GetStringLength(uint8 *str,uint16 CharCount,uint8 str_type);
void Sdrv_SwapIntByte(uint16 * pword);

uint16 Sdrv_UniToChar(uint16 unichar);
uint8* Sdrv_MultiLanDeal(uint8* srcstr,uint8 strtype,uint16* fontcode);
void Sdrv_MultiGetFontPoint(uint16 code_rename, uint8 *picbuf);
//add for thai language
//uint8 Sdrv_ThaiPoint(const uint8 * str, uint8 IsUnicode, uint16 sorcecode);

//add for arabic language
uint8* Sdrv_SpecialLanDeal(const uint8 *lanstr, int8 length,uint8* chartype);

uint8 UnicodeTOArabic(uint8 *str,uint8 length);
bool Arabic_IsNSM(uint16 c);

 
int8 BAPI_ThaiOverlap(int8 * str, int8 IsUnicode);
int8 Sdrv_ThaiOverlap(int8 * str, int8 IsUnicode);
void BAPI_GetFontPoint(uint16 ucode,uint16 picbuf);
uint16 BAPI_FontCodeMBToUni(uint16 code_rename);
 
uint8 Sdrv_AraCharToUni(uint8 *str,uint8* deststr,uint8 length);
//uint8 Sdrv_ArabicCopy(uint8 *Dst,uint8 *src,int8 Length,uint8 codeType);
bool CheckDisplaySpace(uint8 codemod,uint8 fontwidth);

//以下为全局变量
#ifndef PC
extern uint8 DisplayBuffer[];
#else
extern DLL_IMP uint8 DisplayBuffer[]; //10-3-5 LG
#endif
extern int8 CharPointBuffer[];

extern region_t region;//记录区域坐标
extern region_t region_win;//设定的屏幕窗口

extern uint8 StrHeight;//当前行字符的高度
extern uint8 PutCharErrFlag;//保存字符输出时的出错类型

extern uint8 Thai_lan;
extern uint8 TwoByteLan;

extern int16 PrevAffectsNextJoining;

//extern uint8 backup_endstrx;
extern uint8 backup_curstrx;

extern uint8 Language_ID;

extern uint8 UniBuffer[];
extern uint8 ArabicBuf[];

#ifdef WELCOME
extern int8 far reent_flag; //store CE flag
extern uint8 far cesel;
#else
extern int8 reent_flag; //store CE flag
extern uint8 cesel;
#endif
extern SD_FILE * fontlibfp;
extern SD_FILE * multifontlibfp;
//for change table
extern SD_FILE * mbtounifp;

extern uint8 fontlibtype;
extern uint8 AsciiSelect;
extern uint8 StrEndX;
extern uint8  TextColumn;
extern uint8  TextPage;
extern uint8 IsArabicOrHebrew;
extern uint32 MBToUniTabAddr;
extern int8 AuxiliaryBuffer[];
extern uint32 FontLibAddr;
extern uint32 AsciiFontLibAddr;
extern uint8 MBCodeTwoFlag;
extern uint8 AscFontSmall;
extern uint8 CharWidth;
//宏函数
//阿拉伯和希伯莱这些从右到左阅读方向的语言的特殊处理
//Flag TRUE--map  FALSE--remap
#define RegionMapping(region, justwidth)	\
	do{\
		if(IsArabicOrHebrew)\
		{\
			if((justwidth)==-1)\
			{\
				(region)->x = Display_Length - (region)->x - (region)->width;\
			}\
			else\
			{\
				(region)->x = Display_Length - (region)->x - (justwidth);\
				TextColumn = (region)->x;\
			}\
		}\
	}while(0)


#endif //__C251__
#endif /* _Drv_S7565_H */

