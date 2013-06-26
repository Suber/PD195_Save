#ifndef _PICDEC_H
#define _PICDEC_H

#include "typeext.h"
//#include "display.h"

#include "ak211x.h"

//debug macro
//请根据不同debug需要，打开或关闭特定宏 
//#define NO_DEC_LCM			   //不进行解码和LCD刷屏 
//#define NO_LCM				   //不进行LCD刷屏 
//#define NO_CARD_DEC_LCM		   //不进行读文件,解码和刷屏

 
//for lib compile
#define JPEG_RAM_INDEX     1
#define MURAM1L_RAM_INDEX  2

extern void DMA3Init(uint8 pictype);

extern bool DMA3StartTrans(uint8 dma3_ram);	 //开始DMA3传输，dma3_ram=0, jpeg ram;dma3_ram=1,muram1m. ret=true---success; false---fail

extern void DMA3SetCounter(uint8 row, uint16 col);	 //设置DMA3 counter,注意length不要超过min[DMA3counter， Memory length]
extern void DMA3SetSrcAddr(uint16 src_addr);  //设置memory起始地址 

#define PREVIEW_PLAY 0
#define NORMAL_PLAY	 1


typedef enum
{
    Pic_JPG,
    Pic_BMP,
    Pic_GIF   
}Pic_type_t;

typedef enum
{
    RGB565,
    RGB888  
}Bmp_type_t;

typedef  struct
{
	uint16 x;
    uint16 y;
    uint16 w;
    uint16 h;
}wregion_t;

typedef  struct
{
#ifndef PC
    uint16 fp;
#else
    IntAddr fp;				  
#endif
	wregion_t   *pregion1;	  	  //show region
    wregion_t   *pregion2;       //setwindow region 
    uint16  	offset;	      //file offset
    Bmp_type_t  bmpType;	  //BMP type(RGB565/RGB888)
    uint8 playtype;      	//show type	bit   0: auto rotation; 1: preview 	 ; 0 no; 1 yes
	uint8 reserved;       // reserved for if winh2v
}ColorLcmInfor;

#define PICDEC_OK            0
#define PICDEC_ERROR         -1
#define PICDEC_UNSUPPORTED   -2
#define PICDEC_STRM_ERROR    -3
#define PICDEC_TIMEOUT       -4	

uint8 PicViewInit(ColorLcmInfor *picInfor, Pic_type_t pictype);
void PicViewExit(Pic_type_t pictype);
uint8 PicDec(ColorLcmInfor *picInfor, Pic_type_t pictype);
uint8  JPEGInit(ColorLcmInfor *picInfor);
void JPEGExit(void);
uint8 JPEGDec( ColorLcmInfor *picInfor);
uint8 BMPInit(ColorLcmInfor *picInfor);
void  BMPExit(void);
uint8 BMPDec(ColorLcmInfor *picInfor);
uint8 GIFInit(ColorLcmInfor *picInfor);
void  GIFExit(void);
uint8 GIFDec(ColorLcmInfor *picInfor);

extern uint8 PicPlay(ColorLcmInfor* pPicInfo, Pic_type_t pictype);

//pic_common content
uint8  mygetchar(void);
void get512byte(void);
void fileinit(handle fp);
void FileReset(void);
void  seektodata(uint16  pos);
uint16 readint16le(void);	//little endian
uint16 readint16be(void);	//big endian
uint32 readint32le(void);
uint32 readint32be(void);
uint16 readint16(void);	
uint32 readint32(void);
void get_app1(void);
void storeclk(void);
void restoreclk(void);
//===========================
//for picdata header
#define source_buf 0x2400 
#define dest 0x3000

#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3

#define LZW_MAXCODES 4096
#define LZW_MAXBITS 12
#define NOCODE -1
#define SUCCESS 1
#define DECODEERROR 0
#define LABELERROR 0

extern uint8 zigzag[64];
///////////JPEG DEC////////////////
extern int16  near source_count;
extern uint8* near psrc ;
extern handle near picfp;

extern uint8 near DC_TAB0[16] ;
extern uint8 near DC_TAB1[16] ;
extern uint8 near AC_TAB0[16] ;
extern uint8 near AC_TAB1[16] ;
extern uint8   near     WindowModeH2V   ;
extern uint16 near imgHigh ;
extern uint16 near imgWide;
extern uint8  near	inputmode  ;
extern uint8  near qtabs   ;
extern uint8  near dcactab ;
extern uint8  near H_SampFactor[4] ;
extern uint8  near V_SampFactor[4] ;
extern uint8  near downscale;
extern uint16 near decHigh;
extern uint16 near decWide;
extern uint16 near showlength;//for lcd show
extern uint16 near showlines;//
extern uint16 near jpglcd_high;
extern uint16 near jpglcd_wide;
extern uint16 near jpglcd_x;
extern uint16 near jpglcd_y;
extern uint8 near thumbflag;
extern uint8 near decthumbflag;
extern uint16 near thumbwide;
extern uint16 near pic2wide;

extern uint8  near reg_adh_bak;
extern uint8  near reg_aeh_bak;
extern uint8  near reg_afh_bak;
extern uint8  near reg_bfh_bak;//jram123

extern uint8  near reg_b1h_bak;
extern uint8  near reg_b4h_bak;//dram251b
///////////////////////////////////////////
//for bmp decode 
//for data trans 
extern void BMPData_Trans(void);
/////////////BMP DEC/////////////////
extern uint16   near     image_w_out;
extern uint16   near     ccNNNoffset_bits_Img;
extern uint16   near     bits_per_pixel_Img;
extern uint16   near     cNNNbiCompression_Img;//0-none,1-rle 8,2-rle 4,3-bitfields
extern uint16   near     yNNNbiClrUsed_Img;    //number of colors    
extern uint16   near     read_byte_for_head;

extern uint16 near color_size;
extern uint8* near  colormap;
//for region save 
extern wregion_t* bmp_region;

extern uint16 near  offset;
//extern wregion_t near  picregion;
//////////////////////////
extern uint8   near     BackGroundColor ;
extern uint8   near     TextBackGroundColor ;
extern uint8   near     DisposalMethod ;
extern uint8   near     TransparentColor ;//
extern uint8   near     TransparentColorIndex ;    //
extern uint8   near   TextFlag  ;
extern uint8 near     Mode   ;  

extern uint16 near  Window_Row	;
extern uint16 near  Window_Col ;
extern uint16 near  Image_Row  ;
extern uint16 near  Image_Col  ;
extern uint16 near ImageWidth	;
extern uint16 near ImageHeight ;
extern uint16 near ImageTop	  ;
extern uint16 near ImageLeft   ;
extern uint16 near ImageTopOri  ;
extern uint16 near ImageLeftOri ;


extern uint16 near RowCount ;

extern uint16 near ByteCount ;

extern uint16 near Bytes   ;

extern int16 near LastCode ;
extern int16 near EofCode ;


extern int16 near CurrentCode ;

extern int16 near StackIndex ;
extern int16 near TableIndex ;
extern int16 near PrefixCode ;
extern int8 near SuffixCode ;
extern uint8 near Pass ;

extern int8 near GraphicControlFlag ;
extern int16 near outloop ;
	
extern uint8  near * SuffixTable ;//[4096];
extern uint16  near * PrefixTable ;//[4096];

extern uint8   near * StackTable ;//[4096];
extern uint8  near * BlockBuf ;//[256];

extern uint8 near *  ImagePalette  ;//[256*3];//dm
extern uint8  near * GPalette ;//[256*3];


extern uint8  near * LCDBuf ;//[3600];
extern uint8  near * PreLCDBuf ;//[3600];
extern uint8 near *  LineBuf ;//[60];
extern uint8    near     rgbflag ;
extern uint8   near     readfileflag  ;

extern uint8 imflag;//intel or mm
extern uint8 readflag;
extern uint8 actflag;

//================================
//for jpeg dec 
extern uint8 Readjpghead(void);
extern void writehuftoreg(void);
extern void get_fffe(void);
//===============================

//for some special register

//commom reg

#ifndef  PC
sbit AIE_Ecodec	 =AIE^5;
sbit AIE_Elcd	 =AIE^3;

sbit MemBankCtl_MenBankEn=MemBankCtl^0;
sbit MemBankCtl_IntVectPage=MemBankCtl^2;
sbit MemBankCtl_PCMMU2=MemBankCtl^3;
//bank 0x01  RMU CMU REG
//sfr MRCR1 =0x88;
//sfr MRCR2 =0x89;
//sfr MCSR1 =0xa9;
//sfr MCSR2 =0xaa;
//sfr MCSR3 =0xab; //bit54  video codec clock select 10 hosc or 11 pll
//sfr MCSR4 =0xac; //
//sfr MCSR5 =0xad; //bit43  jram3 clock  01 video clock or 10 mcu clock
//sfr MCSR6 =0xae; //bit10  jram1 clock  01 video clock or 10 mcu clock
//sfr MCSR7 =0xaf;// bit10  jram2 clock  01 video clock or 10 mcu clock
//sfr MCSR8 =0xb0;// MURAM1M L  for amv
//sfr MCSR9 =0xb1;// bit20  dram251b   011 video clock or 010 mcu clock or 100 dma3
//sfr MCSR10=0xb2;//  dram251a 
sbit MRCR2_VideoReset= MRCR2^6;
sbit MCSR3_VideoCLK1=MCSR3^5;
sbit MCSR3_VideoCLK0=MCSR3^4;
sbit MCSR4_MCUCLK1=MCSR4^1;	 //mcu clk 
sbit MCSR4_MCUCLK0=MCSR4^0;
sbit MCSR5_JRAM3CLK1=MCSR5^4;	  //jram3 clk
sbit MCSR5_JRAM3CLK0=MCSR5^3;
sbit MCSR6_JRAM1CLK1=MCSR6^1;	 //jram1 clk 
sbit MCSR6_JRAM1CLK0=MCSR6^0;
sbit MCSR7_JRAM2CLK1=MCSR7^1;	 //jram2 clk 
sbit MCSR7_JRAM2CLK0=MCSR7^0;
sbit MCSR7_B1CLK2=MCSR7^7;	 //b1 clk 
sbit MCSR7_B1CLK1=MCSR7^6;
sbit MCSR7_B1CLK0=MCSR7^5;	  

sbit MCSR8_MURAM1MCLK2=MCSR8^7;	 //muram1m
sbit MCSR8_MURAM1MCLK1=MCSR8^6;
sbit MCSR8_MURAM1MCLK0=MCSR8^5;
sbit MCSR8_MURAM1LCLK2=MCSR8^4;	  //muram1l
sbit MCSR8_MURAM1LCLK1=MCSR8^3;
sbit MCSR8_MURAM1LCLK0=MCSR8^2;
sbit MCSR9_DRAM251BCLK2=MCSR9^2;	  //dram251b
sbit MCSR9_DRAM251BCLK1=MCSR9^1;
sbit MCSR9_DRAM251BCLK0=MCSR9^0;

sbit MCSR10_DRAM251ACLK2=MCSR10^2;	  //dram251a
sbit MCSR10_DRAM251ACLK1=MCSR10^1;
sbit MCSR10_DRAM251ACLK0=MCSR10^0;

sbit MCSR11_D251ACLKEN=MCSR11^2;
sbit MCSR11_D251BCLKEN=MCSR11^3;
sbit MCSR12_JRAM1CLKEN=MCSR12^2;
sbit MCSR12_JRAM2CLKEN=MCSR12^3;
sbit MCSR12_JRAM3CLKEN=MCSR12^4;


//bank 0x0b;  AMV JPG REG
/*  BYTE Registers  */
   
   
/*  BIT Registers  */
sbit  AMVCTL_end=AMVCTL^0;
sbit AMVEN_TimeOutInt =AMVEN^2;
sbit AMVEN_IntDis =AMVEN^1;
sbit AMVEN_DecStart =AMVEN^0;
sbit AMVIE_Timeout=AMVIE^5;
sbit AMVIE_SliceOK=AMVIE^4;
sbit AMVIE_StreamError=AMVIE^3;
sbit AMVIE_BufferEmpty=AMVIE^2;
sbit AMVIE_DecReady=AMVIE^1;
sbit AMVIE_IntBit=AMVIE^0;
sbit JPGMemMapReg_JRAM12Map=MemMapReg^0;
sbit JPGMemMapReg_dismap0= MemMapReg^1;
sbit JPGMemMapReg_dismap1= MemMapReg^2;



sbit MRCR1_AudReset  =MRCR1^7;//wakeup auip
sbit MRCR2_DACReset =MRCR2^4; //
sbit MCSR1_AudCLK1 =MCSR1^7;
sbit MCSR1_AudCLK0 =MCSR1^6;//10 CK24M
sbit MCSR3_I2SRCLK  =MCSR3^2;

sbit MCSR5_PCMRAMCLK1 =MCSR5^2;
sbit MCSR5_PCMRAMCLK0 =MCSR5^1;//01 aud codec clk

sbit MCSR6_MURAM2CLK1 =MCSR6^3;
sbit MCSR6_MURAM2CLK0 =MCSR6^2;//10 mcu


sbit MCSR7_MURAM1HCLK2 =MCSR7^4;
sbit MCSR7_MURAM1HCLK1 =MCSR7^3;
sbit MCSR7_MURAM1HCLK0 =MCSR7^2;//001


sbit MCSR11_PCMRAMCLKEN=MCSR11^0;
sbit MCSR11_MURAM2CLKEN=MCSR11^1;
sbit MCSR11_MURAM1HCLKEN=MCSR11^6;
sbit MCSR11_MURAM1MCLKEN=MCSR11^5;
sbit MCSR11_MURAM1LCLKEN=MCSR11^4;

sbit MCSR11_D251BCLKEN =MCSR11^3;
sbit MCSR12_JRAM3CLKEN=MCSR12^4;

sbit AuCodecFIFOCtl_AlmostEmpty	= AuCodecFIFOCtl^6;
sbit AuCodecFIFOCtl_empty=AuCodecFIFOCtl^7;
sbit AuDecEncCtl_NewFrame=AuDecEncCtl^0;
sbit AuCodecIP_FrameRdyIP=AuCodecIP^3;
sbit AuCodecDebug_State =AuCodecDebug^6;
sbit DAC_CTL3_DADDBG=DAC_CTL3^5;

#else

sbit MCSR11_D251BCLKEN;
sbit MCSR9_DRAM251BCLK2;  //dram251b from mcu
sbit MCSR9_DRAM251BCLK1;
sbit MCSR9_DRAM251BCLK0;
sbit MCSR11_MURAM1MCLKEN;
sbit MCSR8_MURAM1MCLK2;                 // Muram1M clock from mcu
sbit MCSR8_MURAM1MCLK1;
sbit MCSR8_MURAM1MCLK0;
sbit MCSR11_MURAM1MCLKEN;
sbit MCSR11_MURAM1LCLKEN;
sbit MCSR8_MURAM1LCLK2;                 // Muram1L clock from mcu
sbit MCSR8_MURAM1LCLK1;
sbit MCSR8_MURAM1LCLK0;
sbit MCSR11_MURAM1LCLKEN;

sbit MCSR11_MURAM1HCLKEN;
sbit MCSR7_MURAM1HCLK2;                  //Muram1H clock from mcu
sbit MCSR7_MURAM1HCLK1;
sbit MCSR7_MURAM1HCLK0;

sbit MCSR11_MURAM2CLKEN;		//muram2 mcu
sbit MCSR6_MURAM2CLK1;
sbit MCSR6_MURAM2CLK0;
sbit MCSR11_MURAM2CLKEN;
sbit MCSR11_PCMRAMCLKEN;	  //pcmram hl	 mcu
sbit MCSR5_PCMRAMCLK1;
sbit MCSR5_PCMRAMCLK0;

sbit MCSR5_JRAM3CLK1;	  //jram3 clk
sbit MCSR5_JRAM3CLK0;
sbit MCSR12_JRAM1CLKEN;
sbit MCSR6_JRAM1CLK1;//jram1 mcu clk
sbit MCSR6_JRAM1CLK0;
sbit JPGMemMapReg_JRAM12Map;
sbit MCSR7_JRAM2CLK1; //jram2 mcu clk
sbit MCSR7_JRAM2CLK0;
sbit MCSR12_JRAM2CLKEN;

sbit AMVEN_DecStart;	 
sbit AMVIE_BufferEmpty;
sbit  AMVCTL_end;
sbit AMVEN_TimeOutInt;
sbit AMVEN_IntDis;
sbit AMVEN_DecStart;
sbit AMVIE_Timeout;
sbit AMVIE_SliceOK;
sbit AMVIE_StreamError;
sbit AMVIE_BufferEmpty;
sbit AMVIE_DecReady;
sbit AMVIE_IntBit;

sbit MCSR3_VideoCLK1;//ck24m
sbit MCSR3_VideoCLK0;
sbit MCSR12_JRAM3CLKEN;
sbit MRCR2_VideoReset;
#endif



//=========================


#endif
