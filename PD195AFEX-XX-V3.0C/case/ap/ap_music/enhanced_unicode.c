/*
 *******************************************************************************
 *                ACTOS AP
 *        enhanced module impletementation
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#include <actos.h>
#include <enhanced.h>
//#include "debug.h"
#include <lang_id.h>
#define  NEWFORMATRES

#pragma name(MW_EH_UICODE)

#ifdef   NEWFORMATRES

const char UniToMBTab[][12] =
{ "FTBL_GB.$$$", /*简体中文codepage table*/
"FTBL_B5.$$$", /*繁体中文codepage table*/
"1252L.TBL", /*英文,西欧语系 codepage table*/
"FTBL_JP.$$$", /*日文 codepage table*/
"FTBL_KR.$$$", /*韩文 codepage table*/
"1257L.TBL", /*波罗地海语系*/
"1251L.TBL", /*俄语语系*/
"1254L.TBL", /*土耳其语系*/
"1255L.TBL", /*希伯莱语*/
"874L.TBL", /*泰语*/
"1250L.TBL", /*北欧，东欧语系*/
"1256L.TBL" /*阿拉伯语系*/
};

#pragma renamecode(MW_EH_UICODE)

bool switchtochar(BYTE *str, int len, unsigned long sd_address, char latin_flag);
/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool unitochar(char *str, int len, unsigned char encode)
{
    SD_FILE *fp;
    unsigned char table_index = 0;
    unsigned char LatinFlag = 1; /*拉丁语系标志符*/
    unsigned long UniToMBTabAddr = 0;
    switch (encode)
    {
        case CHINESE_SIMPLIFIED:
        table_index = 0;
        LatinFlag = 0;
        break;

        case CHINESE_TRADITIONAL:
        table_index = 1;
        LatinFlag = 0;
        break;

        case ENGLISH:	
        case FRENCH:
        case GERMAN:
        case ITALIAN:
        case DUTCH:
        case PORTUGUESE:
        case SPANISH:
        case SWEDISH:
        table_index = 2;
        break;

        case JAPANESE:
        table_index = 3;
        LatinFlag = 0;
        break;

        case KOREAN:
        table_index = 4;
        break;

                case LITHUANIAN:  //white110118#1 for canyon
				case ESTONIAN:    //white110118#1 for canyon
				case LATVIAN:     //white110118#1 for canyon
        case CZECH:
        case POLISH:
        table_index = 5;
        break;

        case RUSSIAN:
        case BULGARIAN:
        case UKRAINIAN:
        table_index = 6;
        break;

        case TURKISH:
        table_index = 7;
        break;

        case HEBREW:
        table_index = 8;
        break;

        case THAI:
        table_index = 9;
        LatinFlag = 0;
        break;

        case DANISH:
        case HUNGARIAN:
        case ROMANIAN:
        case  CROATIA:
        case  SLOVAK:
        case SERBIAN:
        table_index = 10;
        break;

        case ARABIC:
        table_index = 11;
        break;

        default:
        table_index = 0;
        break;
    }
    /*打开unicode to char 转换表*/
    fp = SD_FOpen(UniToMBTab[table_index], MODE_READ);
    if (fp == NULL)
    {
        //ERR(printf("Cannot open UniFontTab\n"));
        return FALSE;
    }
    UniToMBTabAddr = fp->startaddr;
    SD_FClose(fp);
    return switchtochar(str, len, UniToMBTabAddr, (char) LatinFlag);
}

/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   :关键在于unicode与目标字符集的转换表的制作，这里对应的目标字符集在ASCII码都是一个字节的
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool switchtochar(BYTE *str, int len, unsigned long sd_address, char latin_flag)
{
    int s_cnt;
    int d_cnt;
    WORD FontCode;
    SD_RW sdrw;

    sdrw.srampage = 0x08;
    sdrw.sramaddr = (unsigned int) &FontCode;
    for (d_cnt = 0, s_cnt = 0; (s_cnt < len) && ((str[s_cnt] != 0) || (str[s_cnt + 1] != 0));)
    {
        FontCode = *((WORD *) &str[s_cnt]);//字符的值
        //非ASCII码的转换
        if (FontCode > 127)
        {
            sdrw.faddr = sd_address + (((DWORD) FontCode) << 1);
            SD_Read(&sdrw, 2); //SD_Read((unsigned int)&sdrw,2);
            //Z80是小端，KEIL是大端。这里需要注意，如果调试不正常显示，可以修改这里
            //读转换表获得目标字符集的内码表示

            str[d_cnt++] = *((BYTE*) &FontCode); //高位      Z80是小端 ，51是大端
            //非拉丁语系的内码用两个字符来表示
            if ((latin_flag == 0) && (FontCode > 0x7f)) //两字节码编码转换
            {
                str[d_cnt++] = *((BYTE*) &FontCode + 1); //低位
            }
        }
        else
        {
            str[d_cnt++] = str[s_cnt + 1]; //只取低位,获得的是对应字符集的内码
        }
        s_cnt += 2;
    }
    str[d_cnt] = 0;

    return TRUE;
}

#else
typedef struct
{
    word uni_code;
    byte latin_code;
}uni_latin;

#pragma userclass(near = MOC_EH_UNI)

const uni_latin ConvertTab[27]= //"27" meaning:0x20-5

{   0x20ac,0x80, 0x201a,0x82,0x0192,0x83,0x201e,0x84,0x2026,0x85,0x2020,0x86,0x2021,0x87,
    0x02c6,0x88,0x2030,0x89,0x0160,0x8a,0x2039,0x8b,0x0152,0x8c, 0x017d,0x8e,
    0x2018,0x91,0x2019,0x92,0x201c,0x93,0x201d,0x94,0x2022,0x95,0x2013,0x96,0x2014,0x97,
    0x02dc,0x98,0x2122,0x99,0x0161,0x9a,0x203a,0x9b,0x0153,0x9c, 0x017e,0x9e,0x0178,0x9f
};
#pragma userclass(near = default)

#define FindTimes (sizeof(ConvertTab)/sizeof(ConvertTab[0]))   //也可以直接赋"27"
#pragma renamecode(MO_EH_UNI)

/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   : inchar 输入的字符串首址   codeoffsetaddr 对照表中的首址
 *
 * Returns     : outchar 输出的字符串首址
 *                       返回输出中的有效字节数，为0表示没字符输出
 *
 * Notes       :
 *
 ********************************************************************************
 */
WORD OneUniToChar(WORD unicode, const SD_FILE *fp)
{
    WORD code;
    ulong unicode1;

    unicode1=((ulong)unicode)*2;
    if(unicode>127)
    {
        if(!SD_FSeek(fp,SEEK_SET,unicode1))
        return 0x20;
        if(SD_FRead(fp,&code,2))
        return code;
    }
    else //是ASCII

    {
        return unicode;
    }
    return 0;
}

/*
 ********************************************************************************
 *
 * Description : 函数名:OneUniToLatin1  功能:将一个Latin1字符的unicode转换成对应的Latin1内码
 *
 * Arguments   : 输入:unicode 转换的Latin1字符unicode值
 *
 * Returns     : word: 转换后得到的Latin内码.其中:若输入的unicode非Latin字符时,则返回0x0020!
 *
 *
 * Notes       : for Latin1 unicode convert! by ccm
 *
 ********************************************************************************
 */

word OneUniToLatin1(unsigned int unicode)
{
    byte i;

    if ((unicode<=0x007f) || ((unicode>=0x00a0)&&(unicode<=0x00ff))) //不能写成:(0x00a0<=unicode<=0x00ff)! by ccm!
    return unicode;
    else
    {
        for(i=0;i<FindTimes;i++)
        {
            if (unicode==ConvertTab[i].uni_code)
            {
                return (word)ConvertTab[i].latin_code; //找到,返回对应的Latin1内码!
            }
        }
        if (i==FindTimes)
        return unicode; //若输入的unicode在转换表中查找不到(非Latin字符),则返回0x0020!
    }
}

/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool unitochar(char *str,int len,unsigned char encode)
{
    int s_cnt=0;
    int d_cnt=0;
    WORD code;
    SD_FILE *fp;

    const char ftbl_B5[]="FTBL_B5.$$$";
    const char ftbl_GB[]="FTBL_GB.$$$";
    const char ftbl_JP[]="FTBL_JP.$$$";
    const char ftbl_KR[]="FTBL_KR.$$$";

    switch(encode)
    {
        case CP_CHS: //中文简体
        fp=SD_FOpen(ftbl_GB,MODE_READ);
        break;

        case CP_CHT: //中文繁体
        fp=SD_FOpen(ftbl_B5,MODE_READ);
        break;
        case 3: //日文
        fp=SD_FOpen(ftbl_JP,MODE_READ);
        break;
        case 4: //韩文
        fp=SD_FOpen(ftbl_KR,MODE_READ);
        break;

        default: //ASCII和拉丁文
        // add by ccm for Latin1 unicode convert.
        //Latin1与ASCII使用同一个字库ASCII.BIN和驱动UI_L0724.DRV!
        for(d_cnt=0,s_cnt=0;(s_cnt<len)&&((str[s_cnt]!=0)||(str[s_cnt+1]!=0)); )
        {
            code=OneUniToLatin1(str[s_cnt]+str[s_cnt+1]*0x100);
            str[d_cnt++]=code;
            s_cnt+=2;
        }
        str[d_cnt]=0; //结尾添0
        return 1; //converted complete !   by ccm
    }

    if(fp==NULL)

    {
        SD_FClose(fp);
        return 0;
    }

    for(d_cnt=0,s_cnt=0; (s_cnt<len)&&( (str[s_cnt]!=0) || (str[s_cnt+1]!=0) ); ) // quinn 070621 modify

    {
        code=OneUniToChar(str[s_cnt]+str[s_cnt+1]*0x100, fp);
        str[d_cnt++]=code;//放了低位
        if(code>127)
        {
            str[d_cnt++]=code>>8;//把高位移到低位去
        }
        s_cnt+=2;

    }
    str[d_cnt]=0; //Rcmai 04-1-31 20:37
    SD_FClose(fp);

    return 1;
}
#endif

