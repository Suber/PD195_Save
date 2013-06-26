#ifndef _VIDEODATA_H
#define _VIDEODATA_H
// Edit by Weimh 2010.05.19
#ifdef PC
#include "sim_video_dec.h"
#else	/* #ifdef PC */
#include "ak211x.h"
#include "typeext.h"
#include "codec_video.h"


uint8 AMV_INIT(VideoDataParam_t *pVideoDataParam);
uint8 AMV_Play(void);
uint8 AMV_FrontStep(int8 step);
uint8 AMV_BackStep(int8 step);
uint8 AMV_Step(int8 step);
uint8 AMV_Stop(void);
void  AMV_EXIT(void);
uint8 AVI_INIT(VideoDataParam_t *pVideoDataParam);
uint8 AVI_Play(void);
uint8 AVI_FrontStep(int8 step);
uint8 AVI_BackStep(int8 step);
uint8 AVI_Step(int8 step);
uint8 AVI_Stop(void);
void  AVI_EXIT(void);

extern uint16 readint16le(void);
extern uint8 mygetchar(void);
extern void get512byte(void);
extern void get512bytefb(void);
extern void BreakPointProcess(void);
extern void CalBInofr(void);
extern void aviCalBInofr(void);
extern uint32 readint32le(void);
extern void seektodata(int16 pos);


uint8  mygetchar(void);
uint8  mygetcharfb(void);
void  fileinit(void);
void  seektodata(int16 pos);
//uint8 *mygetjram(uint16  *streamlen);
//uint8 *mygetjram1(uint16 *streamlen,uint16 *startpos);
void modifypsrc(int16 datalen);
void BreakPointProcess(void);



//video dec data
extern uint16 near imgHigh   ;
extern uint16 near imgWide   ;
extern uint32 near chunktype  ;
extern uint32 near chunklen  ;
extern uint32  near bpblockcnt;
extern uint16  near bpblockbyte;
extern uint32  near bpframecnt;
extern uint8 near alltime_sec  ;
extern uint8 near alltime_min  ;
extern uint8 near alltime_hour  ;
extern uint8 near time_sec  ;
extern uint8 near time_min  ;
extern uint8 near time_hour  ;
extern uint8 near reserved  ;
extern uint32 near alltime;
extern uint32 near curtime;
extern uint32 near frmaepersec  ;
extern uint32 near samplespersec  ;
extern uint32 near allframes   ; 
extern uint16 near decHigh ;
extern uint16 near decWide ;
extern uint8  near showlines;
extern uint16 near showlength;
extern int16  near  source_count  ;
extern uint8 near * near psrc ;
extern uint32  near blockcnt ;
extern uint32  near file_sec_cnt ;
extern handle near  vidfp;
extern uint8 near downscale;
extern uint8 near RGBScale;//RGB 565   
extern uint8 near FadeoutPara;// 0,无淡出; 1，1S淡出; 2,2S淡出
extern uint8 near ShowProgressBar;//进度条显示    
extern uint8 near USetW_spare1;//软音量
 
extern uint8   near reg_adh_bak;//mcsr5
extern uint8   near reg_aeh_bak;//mcsr6
extern uint8   near reg_afh_bak;//mcsr7
extern uint8   near reg_b0h_bak;//mcsr8
extern uint8   near reg_b1h_bak;//mcsr9
extern uint8   near reg_b4h_bak;//mcsr11
extern uint8   near reg_bfh_bak;//mcsr12
extern VideoDataParam_t near*VideoDataParam;
extern uint8  near audendflag;
extern uint8  near showflag;
extern uint8  near fadeTime;
extern uint8  near readfileflag;

//reg declare
#define DAC_D_DEBUG

//jpg  reg
sbit AIE_Ecodec	 =AIE^5;
sbit AIE_Elcd	 =AIE^3;

sbit MemBankCtl_MenBankEn=MemBankCtl^0;
sbit MemBankCtl_IntVectPage=MemBankCtl^2;
sbit MemBankCtl_PCMMU2=MemBankCtl^3;

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

sbit AMVCTL_end=AMVCTL^0;
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

//aud reg

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


sbit  MRCR2_DMA012Reset   =MRCR2^0 ;//  0:reset  1:normal
sbit  MCSR4_DMA012CLK   =MCSR4^5     ; //enable DMA0/1/2 clock   0:dc ; 1:mcuclk 
sbit  DMAxCTL0_Abort  =DMAxCTL0^6    ; //abort DMA transfer  1:abort; 0:ena
sbit  AIPH_DMA012    = AIPH^6   ; //set dma 0/1/2 interrupt to lowest level
sbit  AIPL_DMA012   =AIPL^6   ;
sbit  AIE_DMA012    =AIE^6     ; //enable dma 0/1/2 interrupt
sbit  DMAxCTL1_Start =DMAxCTL1^0    ;// start dma
sbit  DMAxIP_Tcirq   = DMAxIP^2;  



sbit MCSR11_PCMRAMCLKEN=MCSR11^0;
sbit MCSR11_MURAM2CLKEN=MCSR11^1;
sbit MCSR11_MURAM1HCLKEN=MCSR11^6;
sbit MCSR12_JRAM1CLKEN=MCSR12^2;
sbit MCSR12_JRAM2CLKEN=MCSR12^3;

sbit MemMapReg_dis1  =MemMapReg^2;

sbit MemMapReg_dis0 =MemMapReg^1;


sbit BEPCtl1_Fadeinena=BEPCtl1^4;

sbit AuDecEncCtl_Fadeoutena=AuDecEncCtl^6;
sbit BEPCtl1_Fadeoutena=BEPCtl1^3;
sbit BEPCtl1_Fadeoutp=BEPCtl1^1;
sbit BEPCtl2_PCMOUT = BEPCtl2^0; 
sbit AUDIOPLL_CTL_EN = AUDIOPLL_CTL^0;
sbit DAC_CTL2_02=DAC_CTL2^2;
#endif		/* end of #ifdef PC */	
#endif
