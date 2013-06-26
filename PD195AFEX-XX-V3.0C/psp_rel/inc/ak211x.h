// ak211x.h
// revision history
// 090804 initial revision
// 090901 add RMU CMU SFR BANK NUMBER
// 090903 change sfr MRCR2R address
// 090903 add MCU PLL register and UMODE control register
// 090915 the CPUINFO register has been changed to ExWait register, and it is all bank now.
// 090917 add DMA0/1/2 SFR BANK NUMBER
// 090917 change the order of DMA0/1/2 registers
// 090917 change AUIP_BANK to BANK_AUIP
// 090917 change the order of amv audio decoding registers
// 090918 add EXTINT register at sfr page 06
// 090918 change the name of amv audio decoding registers
// 090923 add MRCR3 register at sfr page 01
// 090924 add MCUCKDIV register
// 090927 add CLKDEBUG register at at sfr page 01
// 091007 add debug registers of the audio codec
// 091010 add new register to CMU
// 091014 add BankMissEntryH and BankMissEntryL
// 091014 change the description of mp3 header synchronization register 
// 091014 delete SRSCfg5
// 091018 add interrupt flag debug register
// 091019 change the page of interrupt flag debug register
// 091019 change the address of BankMissEntryH and BankMissEntryL
// 091024 change the address of eqcof and syscof registers
// 091104 change the address of UMCTL to 0a7h
// 091105 MCUPLL changed to MCUPLL_CTL
// 091105 the address of BankMissEntry is changed to 0F5H
// 091105 CD_CTL0 changed to CLD_CTL0
// 091106 ADC_CTL_xx1 changed to PMUADC_CTL
// 091106 add the description of PSW&PSW1 bit registers
// 091106 CARD_DEBUG_LEVEL == 1 fpga card 3.0  and CARD_DEBUG_LEVEL == 0 fpga card 2.0
// 091106 GPIO_DEBUG_LEVEL == 1 gpio new  and GPIO_DEBUG_LEVEL == 0 gpio old
// 091106 change the address of sfr SDBLKSZH/SDBLKSZL/DMA4_STAH/DMA4_STAl , 
// important !! card 2.0 is different from card 3.0
// 091108 change the name of the register Index to AmvAuIndex
// 091108 change the name of the register ADC_FITOCTL1 to ADC_FIFOCTL1
// 091112 add ChipVersion and BondingOption register
// 091117 add Multifunction select 6 Register
// 091119 del page06 0x8d reserved and  change VDD_CURRENT_CTL to BAK0
// 091119 del LBNMI register
// 091124 add BIST controller register
// 091125 change MFPSEL5 bank and DBGBOE,DBGACOE,DBGDOE,DBGDIE,DBGFOE register
// 091125 add IMCSR7~IMCSR12, ICCTL, DMASTARTH, DMASTARTL, DMALENH, DMALENL
// 091211 add BANK NUMBER OF EACH MODULE
// 091215 add 3 register : RTCrdm_2,RTCrdm_3 and RTCrdm_4
// 091221 change the order of Hcout1maxpckh Hcout1maxpckl and Hcout3maxpck
////////////////////////////////////////////////////////////////////////////////////////////////
//  prepared  for fpga simulation
#ifndef __AK211X_H__
#define __AK211X_H__

#include "simulatorpc.h"
#define CARD_DEBUG_LEVEL 1
#define GPIO_DEBUG_LEVEL 1
////////////////////////////////////////////////////////////////////////////////////////////////
// BANK NUMBER OF EACH MODULE
#define BANK_BASIC           0
#define BANK_CMU_RMU         1
#define BANK_DMA0            1
#define BANK_DMA1            2
#define BANK_DMA2            3
#define BANK_AUIP            4 
#define BANK_PMU             5
#if GPIO_DEBUG_LEVEL == 0
#define BANK_GPIO            0ffh
#else
#define BANK_GPIO            6
#endif
#define BANK_LCD             6
#define BANK_USB             7
#define BANK_NAND            8
#if CARD_DEBUG_LEVEL == 0
#define BANK_CARD            0feh
#else
#define BANK_CARD            9
#endif
#define BANK_I2C             10
#define BANK_SPI             10
#define BANK_UART            10
#define BANK_IR              10
#define BANK_JPEG            11
#define BANK_RTC             12 
#define BANK_ACC             BANK_UART

////////////////////////////////////////////////////////////////////////////////////////////////

//  BYTE Registers  
// Mnemonic               address       bank number           Description
////////////////////////////////////////////////////////////////////////////////////////////////
// mcu core register        
sfr SPH                   =  0xbe;      // all bank     SPH Stack Pointer high byte
sfr SPL                   =  0x81;      // all bank     SPL Stack Pointer low byte
sfr SP                    =  0x81;      // all bank     SPL Stack Pointer low byte  另外一种写法
sfr DPXL                  =  0x84;      // all bank     DPXL    Data Pointer extended byte
sfr DPH                   =  0x83;      // all bank     DPH Data Pointer high byte
sfr DPL                   =  0x82;      // all bank     DPL Data Pointer low byte
sfr PSW                   =  0xd0;      // all bank     PSW Program Status Word
sfr PSW1                  =  0xd1;      // all bank     PSW1    Program Status Word1
sfr ACC                   =  0xe0;      // all bank     ACC Accumulator
sfr B                     =  0xf0;      // all bank     B   B Register
// psw & psw1 bit register
#ifndef __DEC
sbit CY                   =  0xD7;
#endif
sbit AC                   =  0xD6;
sbit F0                   =  0xD5;
sbit RS1                  =  0xD4;
sbit RS0                  =  0xD3;
sbit OV                   =  0xD2;
sbit UD                   =  0xD1;
sbit P                    =  0xD0;
//////////////////////////////////////////////////////////////////////////////////////////////// 
// external bus access wait state cycle count
sfr ExWait                =  0x96;      // all bank     ExWait  External bus access wait cycle register
//////////////////////////////////////////////////////////////////////////////////////////////// 
// debug register
sfr CCMCON                =  0x8e;      // all bank     CCMCON  Communication Control Register
sfr CCMVAL                =  0x8f;      // all bank     CCMVAL  Communication Value Register
sfr BIRDINFO              =  0x85;      // all bank     BIRDINFO    Bird information
//////////////////////////////////////////////////////////////////////////////////////////////// 
// SFR Bank control
sfr SFR_BANK              =  0x80;      // all bank     SFR_BANK    SFR Banking control Register
//////////////////////////////////////////////////////////////////////////////////////////////// 
// Indirect Addressing
sfr P2                    =  0xa0;      // all bank     P2  Port2 Register which is not used of IO access
sfr MPAGE                 =  0xa1;      // all bank     MPAGE   same as P2 register
//////////////////////////////////////////////////////////////////////////////////////////////// 
// Interrupt control
sfr IE0                   =  0xa8;      // all bank     IE0 Interrupt Enable register 0
// sbit EA     			  = IE0^7;
#ifndef PC
sbit EA     			  = IE0^7;
#else
sbit EA     			  = 0xAF;
#endif
sfr AIE                   =  0xe8;      // all bank     AIE Additional Interrupt Enable register
sfr IPH0                  =  0xb7;      // 0x00         Interrupt Priority High register 0         
sfr IPL0                  =  0xb8;      // 0x00         Interrupt Priority Low register 0          
sfr AIPH                  =  0xf7;      // 0x00         Additional Interrupt Priority High register
sfr AIPL                  =  0xf8;      // 0x00         Additional Interrupt Priority Low register 
sfr IF0                   =  0x88;      // all bank     Interrupt Flag register 0                  
sfr AIF                   =  0xc0;      // all bank     Additional interrupt flag register         
sfr EXTINT                =  0xd8;      // 0x06         External Interrupt Control                 
sfr IFDebug0              =  0xf5;      // 0x01         Interrupt Flag Debug register 0            
sfr IFDebug1              =  0xf6;      // 0x01         Interrupt Flag Debug register 0                       
//////////////////////////////////////////////////////////////////////////////////////////////// 
// banking control registers
sfr MemBankCtl            =  0xf9;      // all bank     Memory banking control register  
sfr NewPageAddr           =  0xfa;      // all bank     New page address register        
sfr PageAddr              =  0xfb;      // all bank     Current Page Address register    
sfr BankMissEntryH        =  0xf5;      // 0x00         Bank Miss Entry Address High byte
sfr BankMissEntryL        =  0xf6;      // 0x00         Bank Miss Entry Address Low byte 
#ifdef __A251__
sfr BankMissEntry         =  0xf5;      // 0x00         Bank Miss Entry Address 16 bit mode access
#endif
////////////////////////////////////////////////////////////////////////////////////////////////  
// test counter registers
sfr TestCounterByte0      =  0xfc;      // 0x00         Test Counter byte register 0 
sfr TestCounterByte1      =  0xfd;      // 0x00         Test Counter byte register 1
sfr TestCounterByte2      =  0xfe;      // 0x00         Test Counter byte register 2
sfr TestCounterByte3      =  0xff;      // 0x00         Test Counter byte register 3
sfr TestCounterByte4      =  0xf1;      // 0x00         Test Counter byte register 4
//////////////////////////////////////////////////////////////////////////////////////////////// 
// DMA0/1/2 register
sfr DMA0CTL0              =  0x90;      // 0x01         DMA0 Control Register 0            
sfr DMA0SADDR1            =  0x91;      // 0x01         DMA0 Source Address 1 Register     
sfr DMA0SADDR0            =  0x92;      // 0x01         DMA0 Source Address 0 Register     
sfr DMA0DADDR1            =  0x93;      // 0x01         DMA0 Destination Address 1 Register
sfr DMA0DADDR0            =  0x94;      // 0x01         DMA0 Destination Address 0 Register
sfr DMA0CTL1              =  0x95;      // 0x01         DMA0 Control Register 1            
sfr DMA0SZH               =  0x97;      // 0x01         DMA0 Transfer Size High Register   
sfr DMA0SZL               =  0x98;      // 0x01         DMA0 Transfer Size Low Register    
sfr DMA0IP                =  0x99;      // 0x01         DMA0 Interrupt Pending register    
sfr DMA1CTL0              =  0x90;      // 0x02         DMA1 Control Register 0            
sfr DMA1SADDR1            =  0x91;      // 0x02         DMA1 Source Address 1 Register     
sfr DMA1SADDR0            =  0x92;      // 0x02         DMA1 Source Address 0 Register     
sfr DMA1DADDR1            =  0x93;      // 0x02         DMA1 Destination Address 1 Register
sfr DMA1DADDR0            =  0x94;      // 0x02         DMA1 Destination Address 0 Register
sfr DMA1CTL1              =  0x95;      // 0x02         DMA1 Control Register 1            
sfr DMA1SZH               =  0x97;      // 0x02         DMA1 Transfer Size High Register   
sfr DMA1SZL               =  0x98;      // 0x02         DMA1 Transfer Size Low Register    
sfr DMA1IP                =  0x99;      // 0x02         DMA1 Interrupt Pending register    
sfr DMA2CTL0              =  0x90;      // 0x03         DMA2 Control Register 0            
sfr DMA2SADDR1            =  0x91;      // 0x03         DMA2 Source Address 1 Register     
sfr DMA2SADDR0            =  0x92;      // 0x03         DMA2 Source Address 0 Register     
sfr DMA2DADDR1            =  0x93;      // 0x03         DMA2 Destination Address 1 Register
sfr DMA2DADDR0            =  0x94;      // 0x03         DMA2 Destination Address 0 Register
sfr DMA2CTL1              =  0x95;      // 0x03         DMA2 Control Register 1            
sfr DMA2SZH               =  0x97;      // 0x03         DMA2 Transfer Size High Register   
sfr DMA2SZL               =  0x98;      // 0x03         DMA2 Transfer Size Low Register    
sfr DMA2IP                =  0x99;      // 0x03         DMA2 Interrupt Pending register
#ifdef __A251__
// common register of DMA0/1/2
// 16 bit mode access
sfr DMAxSADDR             =  0x91;      // bank 1~3     DMAx Source Address Register 16 bit mode access
sfr DMAxDADDR             =  0x93;      // bank 1~3     DMAx Destination Address Register 16 bit mode access
sfr DMAxSZ                =  0x97;      // bank 1~3     DMAx Transfer Size Register 16 bit mode access
#endif
// 8 bit mode access
sfr DMAxCTL0              =  0x90;      // bank 1~3     DMAx Control Register 0
sfr DMAxCTL1              =  0x95;      // bank 1~3     DMAx Control Register 1
sfr DMAxIP                =  0x99;      // bank 1~3     DMAx Interrupt Pending register
//////////////////////////////////////////////////////////////////////////////////////////////// 
// Audio Codec TOP
sfr AuCodecCtl            =  0x90;      // 0x04         Audio Codec Control Register     
sfr AuDebugLength         =  0x91;      // 0x04         Audio Codec Debug Length Register
sfr AuCodecDebug          =  0x92;      // 0x04         Audio Codec Debug Register       
sfr AuCodecFIFOCtl        =  0x93;      // 0x04         Audio Codec FIFO control Register
sfr AuCodecFIFOData       =  0x94;      // 0x04         Audio Codec FIFO Data Register   
sfr AuCodecDebug2         =  0x95;      // 0x04         Audio Codec Debug Register 2     
sfr AuCodecDecStateH      =  0x97;      // 0x04         Decoding State Register High byte
sfr AuCodecDecStateL      =  0x98;      // 0x04         Decoding State Register Low byte 
// MP3 Decoder
sfr MP3IE                 =  0xa9;      // 0x04         MP3 Decoder Interrupt Enable Register      
sfr MP3IP                 =  0xaa;      // 0x04         MP3 Decoder Interrupt Pending Register     
sfr MP3Ctl                =  0xab;      // 0x04         MP3 Decoder Control Register               
sfr MP3HeaderInfo         =  0xac;      // 0x04         Header information register                
sfr MP3BitRateH           =  0xad;      // 0x04         High byte of bit rate index register       
sfr MP3BitRateL           =  0xae;      // 0x04         Low byte of bit rate index register        
sfr MP3TTimeH             =  0xaf;      // 0x04         Total time hours register                  
sfr MP3TTimeM             =  0xb0;      // 0x04         Total time minutes register                
sfr MP3TTimeS             =  0xb1;      // 0x04         Total time seconds register                
sfr MP3CTimeH             =  0xb2;      // 0x04         Current time hours register                
sfr MP3CTimeM             =  0xb3;      // 0x04         Current time minutes register              
sfr MP3CTimeS             =  0xb4;      // 0x04         Current time seconds register              
sfr MP3BlockNumH          =  0xb5;      // 0x04         High byte of Block Number register         
sfr MP3BlockNumM          =  0xb6;      // 0x04         Middle byte of Block Number register       
sfr MP3BlockNumL          =  0xb7;      // 0x04         Low byte of Block Number register          
sfr MP3FileLen3           =  0xb8;      // 0x04         Byte 3 of File Length register             
sfr MP3FileLen2           =  0xb9;      // 0x04         Byte 2 of File Length register             
sfr MP3FileLen1           =  0xba;      // 0x04         Byte 1 of File Length register             
sfr MP3HeaderSynCtl       =  0xbb;      // 0x04         MP3 header synchronization control register
sfr SynHeaderData1        =  0xbc;      // 0x04         MP3 header synchronization Data register 1 
sfr SynHeaderData2        =  0xbd;      // 0x04         MP3 header synchronization Data register 2 
// WMA Decoder
sfr WMAIE                 =  0xa9;      // 0x04         WMA Decoder Interrupt Enable Register  
sfr WMAIP                 =  0xaa;      // 0x04         WMA Decoder Interrupt Pending Register
sfr WMACtl                =  0xab;      // 0x04         WMA Decoder Control Register          
sfr WMAHeaderInfo         =  0xac;      // 0x04         Header information register           
sfr WMABitRateH           =  0xad;      // 0x04         High byte of bit rate index register  
sfr WMABitRateL           =  0xae;      // 0x04         Low byte of bit rate index register   
sfr WMATTimeH             =  0xaf;      // 0x04         Total time hours register             
sfr WMATTimeM             =  0xb0;      // 0x04         Total time minutes register           
sfr WMATTimeL             =  0xb1;      // 0x04         Total time seconds register           
sfr WMACTimeH             =  0xb2;      // 0x04         Current time hours register           
sfr WMACTimeM             =  0xb3;      // 0x04         Current time minutes register         
sfr WMACTimeL             =  0xb4;      // 0x04         Current time seconds register         
sfr WMAPackNumH           =  0xb5;      // 0x04         High byte of Packet Number register   
sfr WMAPackNumM           =  0xb6;      // 0x04         Middle byte of Packet Number register 
sfr WMAPackNumL           =  0xb7;      // 0x04         Low byte of Packet Number register    
sfr WMAPackSizeH          =  0xb8;      // 0x04         High byte of Packet Size register     
sfr WMAPackSizeM          =  0xb9;      // 0x04         Middle byte of Packet Size register   
sfr WMAPackSizeL          =  0xba;      // 0x04         Low byte of Packet Size register      
// WAV Decoder 
sfr WAVIE                 =  0xa9;      // 0x04         WAV Decoder Interrupt Enable Register 
sfr WAVIP                 =  0xaa;      // 0x04         WAV Decoder Interrupt Pending Register
sfr WAVCtl                =  0xab;      // 0x04         WAV Decoder Control Register          
sfr WAVHeaderInfo         =  0xac;      // 0x04         Header information register           
sfr WAVBitRateH           =  0xad;      // 0x04         High byte of bit rate index register  
sfr WAVBitRateL           =  0xae;      // 0x04         Low byte of bit rate index register   
sfr WAVTTimeH             =  0xaf;      // 0x04         Total time hours register             
sfr WAVTTimeM             =  0xb0;      // 0x04         Total time minutes register           
sfr WAVTTimeL             =  0xb1;      // 0x04         Total time seconds register           
sfr WAVCTimeH             =  0xb2;      // 0x04         Current time hours register           
sfr WAVCTimeM             =  0xb3;      // 0x04         Current time minutes register         
sfr WAVCTimeS             =  0xb4;      // 0x04         Current time seconds register         
sfr WAVBlockNumH          =  0xb5;      // 0x04         High byte of Block Number register    
sfr WAVBlockNumM          =  0xb6;      // 0x04         Middle byte of Block Number register  
sfr WAVBlockNumL          =  0xb7;      // 0x04         Low byte of Block Number register     
// WAV Encoder
sfr WAVEncIE              =  0xa9;      // 0x04         WAV Encoder Interrupt Enable Register 
sfr WAVEncIP              =  0xaa;      // 0x04         WAV Encoder Interrupt Pending Register
sfr WAVEncCtl             =  0xab;      // 0x04         WAV Encoder Control Register          
// Amv Audio Decoder
sfr AmvAudioIP            =  0xaa;      // 0x04         Amv Audio Decoder Pending Register 
sfr AmvAudioCtl           =  0xab;      // 0x04         Amv Audio Decoder Control Register 
sfr PrevDataH             =  0xb5;      // 0x04         High byte of previous data register
sfr PrevDataL             =  0xb6;      // 0x04         Low byte of previous data register 
sfr AmvAuIndex            =  0xb7;      // 0x04         Index register                     
sfr FrameLenH             =  0xb8;      // 0x04         High byte of frame length register 
sfr FrameLenL             =  0xb9;      // 0x04         Low byte of frame length register  
#ifdef __A251__
// 16 bit mode access
sfr AmvAuFrameLen         =  0xb8;      // bank 04h     frame length register, 16 bit mod access
sfr AmvAuPrevData         =  0xb5;      // bank 04h     predicted data register, 16 bit mod access
#endif
// Back End Processor
sfr BEPCtl1               =  0xc2;      // 0x04         Back End Processor Control Register 1
sfr BEPCtl2               =  0xc3;      // 0x04         Back End Processor Control Register 2
sfr BEPCtl3               =  0xc4;      // 0x04         Back End Processor Control Register 3
sfr BEPCtl4               =  0xc5;      // 0x04         Back End Processor Control Register 4
sfr BEPCtl5               =  0xc6;      // 0x04         Back End Processor Control Register 5
sfr GlobalGainH           =  0xc7;      // 0x04         Global Gain High Byte Register       
sfr GlobalGainM           =  0xc8;      // 0x04         Global Gain Middle Byte Register     
sfr GlobalGainL           =  0xc9;      // 0x04         Global Gain Low Byte Register        
// sfr Reserved              =  0xca;      // 0x04                                         
// sfr Reserved              =  0xcb;      // 0x04                                         
// EQ Processor
sfr EQCoe0                =  0xcc;      // 0x04         NO.0 Equalizer Coefficients Register 
sfr EQCoe1                =  0xcd;      // 0x04         NO.1 Equalizer Coefficients Register
sfr EQCoe2                =  0xce;      // 0x04         NO.2 Equalizer Coefficients Register
sfr EQCoe3                =  0xcf;      // 0x04         NO.3 Equalizer Coefficients Register
sfr EQCoe4                =  0xd2;      // 0x04         NO.4 Equalizer Coefficients Register
sfr EQCoe5                =  0xd3;      // 0x04         NO.5 Equalizer Coefficients Register
sfr EQCoe6                =  0xd4;      // 0x04         NO.6 Equalizer Coefficients Register
// SRS Processor 
sfr SRSCtl                =  0xcc;      // 0x04         SRS Control Register 
sfr SRSCfg1               =  0xcd;      // 0x04         SRS config register 1
sfr SRSCfg2               =  0xce;      // 0x04         SRS config register 2
sfr SRSCfg3               =  0xcf;      // 0x04         SRS config register 3
sfr SRSCfg4               =  0xd2;      // 0x04         SRS config register 4
// sfr Reserved              =  0xd3;      // 0x04         reserved register    
// audio ip common registers
sfr AuCodecIE             =  0xa9;      // bank 04h     Audio Codec Interrupt Enable Register 
sfr AuCodecIP             =  0xaa;      // bank 04h     Audio Codec Interrupt Pending Register
sfr AuDecEncCtl           =  0xab;      // bank 04h     Audio decoding or encoding Control Register          
sfr AuCodecHeaderInfo     =  0xac;      // bank 04h     Header information register           
sfr AuCodecBirRateH       =  0xad;      // bank 04h     High byte of bit rate index register  
sfr AuCodecBirRateL       =  0xae;      // bank 04h     Low byte of bit rate index register   
sfr AuCodecTTimeH         =  0xaf;      // bank 04h     Total time hours register             
sfr AuCodecTTimeM         =  0xb0;      // bank 04h     Total time minutes register           
sfr AuCodecTTimeS         =  0xb1;      // bank 04h     Total time seconds register           
sfr AuCodecCTimeH         =  0xb2;      // bank 04h     Current time hours register           
sfr AuCodecCTimeM         =  0xb3;      // bank 04h     Current time minutes register         
sfr AuCodecCTimeS         =  0xb4;      // bank 04h     Current time seconds register
//////////////////////////////////////////////////////////////////////////////////////////////// 
// DAC_PA
sfr DAC_CTL0              =  0xd5;      // 0x04         DAC Control Register 0         
sfr DAC_CTL1              =  0xd6;      // 0x04         DAC Control Register 1         
sfr DAC_CTL2              =  0xd7;      // 0x04         DAC Control Register 2         
sfr DAC_CTL3              =  0xd8;      // 0x04         DAC Control Register 3         
sfr DAC_CTL4              =  0xd9;      // 0x04         Reserved                       
sfr DAC_SR                =  0xda;      // 0x04         DAC Sample Rate select Register
sfr DAC_PCML              =  0xdb;      // 0x04         DAC PCM DATA Low byte          
sfr DAC_PCMM              =  0xdc;      // 0x04         DAC PCM DATA Middle byte       
sfr DAC_PCMH              =  0xdd;      // 0x04         DAC PCM DATA High byte         
sfr DAC_ANALOG            =  0xde;      // 0x04         DAC Analog Control Register    
sfr PA_CTL                =  0xdf;      // 0x04         PA Control Register            
sfr DAC_TUNE0             =  0xe1;      // 0x04         DAC tuning Control Register 0  
sfr DAC_TUNE1             =  0xe2;      // 0x04         DAC tuning Control Register 1  
sfr PA_APCTL              =  0xe3;      // 0x04         PA anti-pop Control Register   
sfr DDV_CTL0              =  0xe4;      // 0x04         Direct drive Control Register 0
sfr DDV_CTL1              =  0xe5;      // 0x04         Direct drive Control Register 1
sfr KT_CTL                =  0xe6;      // 0x04         Key-tone Control Register      
sfr KT_VAL                =  0xe7;      // 0x04         Key-tone Value Register        
//////////////////////////////////////////////////////////////////////////////////////////////// 
// ADC
sfr ADC_CTL               =  0xe9;      // 0x04         ADC Control Register                   
sfr MICOP_CTL             =  0xea;      // 0x04         Mic OP Control Register              
sfr FMOP_CTL              =  0xeb;      // 0x04         FM OP Control Register               
sfr ADC_GCTL              =  0xec;      // 0x04         ADC gain Control Register            
sfr ADC_TUNE0             =  0xed;      // 0x04         ADC tuning control Register 0        
sfr ADC_TUNE1             =  0xee;      // 0x04         ADC tuning control Register 1        
sfr ADC_DAT               =  0xef;      // 0x04         ADC data register                    
sfr ADC_FIFOCTL0          =  0xf1;      // 0x04         ADC FIFO control register 0          
sfr ADC_FIFOCTL1          =  0xf2;      // 0x04         ADC FIFO control register 1          
sfr MTR_DBG               =  0xf3;      // 0x04         Mic OP offset trimming debug register
////////////////////////////////////////////////////////////////////////////////////////////////
// CLASSD
sfr CLD_CTL0               =  0xf4;      // 0x04         ClassD Control Register 0
sfr CLD_CTL1               =  0xf5;      // 0x04         ClassD Control Register 1
sfr CLD_CTL2               =  0xf6;      // 0x04         ClassD Control Register 2
sfr CLD_CTL3              =  0xf7;      // 0x04         ClassD Control Register 3
////////////////////////////////////////////////////////////////////////////////////////////////
// PMU                                                                   
sfr VOUT_CTL              =  0x90;      // 0x05         VCC/VDD voltage set Register                                   
sfr VDD_DCDC_CTL          =  0x91;      // 0x05         DCDC Modulation and frequency set Register                     
sfr VCC_CURRENT_CTL       =  0x92;      // 0x05         VCC DCDC and LDO current limit set Register                    
sfr FS_CTL                =  0x93;      // 0x05         FSOURCE control Register                    
sfr CHG_CTL               =  0x94;      // 0x05         Charge current and temperature set Register                    
sfr CHG_DET               =  0x95;      // 0x05         Charge current and status detect Register                      
sfr CHG_ASSISTANT         =  0x97;      // 0x05         Charge terminate voltage and temperature set Register          
sfr DIODE_CTL             =  0x98;      // 0x05         Idea diode control Register                                    
sfr BDG_CTL               =  0x99;      // 0x05         Bandgap enable and voltage set Register                        
sfr EFUSE_CTL             =  0x9a;      // 0x05         Efuse enable and efuse bounding option Register                
sfr PMUADC_CTL            =  0x9b;      // 0x05         ADC frequency and enable Register                              
sfr BATADC_DATA           =  0x9c;      // 0x05         BATADC data Register                                           
sfr LRADC2_DATA           =  0x9d;      // 0x05         LRADC2 data Register                                           
sfr LRADC1_DATA           =  0x9e;      // 0x05         LRADC1 data Register                                           
sfr LRADC3_DATA           =  0x9f;      // 0x05         LRADC3 data Register                                           
sfr LRADC4_DATA           =  0xa2;      // 0x05         LRADC4 data Register                                           
sfr LRADC5_DATA           =  0xa3;      // 0x05         LRADC5 data Register                                           
sfr MULTI_USED            =  0xa4;      // 0x05         GPIO multi-used set Register                                   
sfr SYSTEM_VOL            =  0xa5;      // 0x05         System Voltage detect or set Register                      
sfr SYSTEM_CTL_RTCVDD     =  0xa7;      // 0x05         System on/off and play/pause time set & LB voltage set Register
sfr PRESS_DISPLAY         =  0xa9;      // 0x05         SYSON key-press status display Register                        
sfr STANDBY_DEBUG         =  0xaa;      // 0x05         Standby Debug Register                                         
sfr RTCVDD_BAK1           =  0xab;      // 0x05                                                                   
sfr RTCVDD_BAK2           =  0xac;      // 0x05                                                                   
sfr RTCVDD_BAK3           =  0xad;      // 0x05                                                                   
sfr ID_STATUS             =  0xae;      // 0x05         TEFUSE ID status Register                                                                      
////////////////////////////////////////////////////////////////////////////////////////////////
// LCD_DMA3
// sfr Reserved              =  0x94;      // 0x06                                            
// sfr Reserved              =  0x95;      // 0x06                                       
sfr LCD_IF_PCS            =  0x97;      // 0x06         LCD Interface Parity Sum Register  
sfr LCD_CTL               =  0x98;      // 0x06         LCD Control Register               
sfr LCD_IF_CLK            =  0x99;      // 0x06         LCD Interface Clock Register       
sfr DMA3_CTL              =  0x9a;      // 0x06         DMA3 Control Register              
sfr DMA3_SRCADDRH         =  0x9b;      // 0x06         DMA3 SRC Address High Register     
sfr DMA3_SRCADDRL         =  0x9c;      // 0x06         DMA3 SRC Address Low Register      
sfr DMA3_CCNTH            =  0x9d;      // 0x06         DMA3 Column Higher Counter Register
sfr DMA3_CCNTL            =  0x9e;      // 0x06         DMA3 Column Lower Counter Register 
sfr DMA3_RCNT             =  0x9f;      // 0x06         DMA3 Row Counter Register          
////////////////////////////////////////////////////////////////////////////////////////////////
// External memory access
sfr EXTMEM_CTL            =  0xfc;      // 6,8          External memory control Register
sfr EXTMEM_WT             =  0xfd;      // 6,8          External memory wait state      
sfr EXTMEM_DH             =  0xfe;      // 0x06         External memory high byte       
sfr EXTMEM_DL             =  0xff;      // 6,8          External memory low byte        
////////////////////////////////////////////////////////////////////////////////////////////////
// GPIO
sfr GPIOAOUTEN            =  0xa2;      // 0x06         General Purpose Input Output Group A Output Enable
sfr GPIOAINEN             =  0xa3;      // 0x06         General Purpose Input Output Group A Input Enable 
sfr GPIOADAT              =  0xa4;      // 0x06         General Purpose Input Output Group A Data         
sfr GPIOBOUTEN            =  0xa5;      // 0x06         General Purpose Input Output Group B Output Enable
sfr GPIOBINEN             =  0xa6;      // 0x06         General Purpose Input Output Group B Input Enable 
sfr GPIOBDAT              =  0xa7;      // 0x06         General Purpose Input Output Group B Data         
sfr GPIOCOUTEN            =  0xa9;      // 0x06         General Purpose Input Output Group C Output Enable
sfr GPIOCINEN             =  0xaa;      // 0x06         General Purpose Input Output Group C Input Enable 
sfr GPIOCDAT              =  0xab;      // 0x06         General Purpose Input Output Group C Data         
sfr GPIODOUTEN            =  0xac;      // 0x06         General Purpose Input Output Group D Output Enable
sfr GPIODINEN             =  0xad;      // 0x06         General Purpose Input Output Group D Input Enable 
sfr GPIODDAT              =  0xae;      // 0x06         General Purpose Input Output Group D Data         
sfr GPIOEOUTEN            =  0xaf;      // 0x06         General Purpose Input Output Group E Output Enable
sfr GPIOEINEN             =  0xb0;      // 0x06         General Purpose Input Output Group E Input Enable 
sfr GPIOEDAT              =  0xb1;      // 0x06         General Purpose Input Output Group E Data         
sfr GPIOFOUTEN            =  0xb2;      // 0x06         General Purpose Input Output Group F Output Enable
sfr GPIOFINEN             =  0xb3;      // 0x06         General Purpose Input Output Group F Input Enable 
sfr GPIOFDAT              =  0xb4;      // 0x06         General Purpose Input Output Group F Data         
sfr GPIOGOUTEN            =  0xb5;      // 0x06         General Purpose Input Output Group G Output Enable
sfr GPIOGINEN             =  0xb6;      // 0x06         General Purpose Input Output Group G Input Enable 
sfr GPIOGDAT              =  0xb7;      // 0x06         General Purpose Input Output Group G Data         
// sfr Reserved              =  0xb8;      // 0x06         Reserved Register                                 
sfr BOOTSEL               =  0xb9;      // 0x06         Boot Mode Select Register                         
sfr PADDRV0               =  0xba;      // 0x06         Pad Drive Control 0 Register                      
sfr PADDRV1               =  0xbb;      // 0x06         Pad Drive Control 1 Register                      
sfr PADDRV2               =  0xbc;      // 0x06         Pad Drive Control 2 Register                      
// sfr Reserved              =  0xbd;      // 0x06         Pad Drive Reserved Register                       
// sfr Reserved              =  0xc2;      // 0x06         Pad Drive Reserved Register                       
sfr PADPUPD0              =  0xc3;      // 0x06         Pad PU and PD Control 0 Register                  
sfr PADPUPD1              =  0xc4;      // 0x06         Pad PU and PD Control 1Register                   
// sfr Reserved              =  0xc5;      // 0x06         Pad PU and PD Control Reserved Register           
sfr PWMDUTY               =  0xc6;      // 0x06         PWM Control Register                              
sfr PWMDIV                =  0xc7;      // 0x06         PWM Clock Divide Register                         
// sfr Reserved              =  0xc8;      // 0x06         PWM Reserved Register                             
// sfr Reserved              =  0xc9;      // 0x06         PWM Reserved Register                             
sfr MFPSEL0               =  0xca;      // 6,8          Multifunction select 0 Register                   
sfr MFPSEL1               =  0xcb;      // 6,8          Multifunction select 1 Register                   
sfr MFPSEL2               =  0xcc;      // 0x06         Multifunction select 2 Register                   
sfr MFPSEL3               =  0xcd;      // 0x06         Multifunction select 3 Register                   
sfr MFPSEL4               =  0xce;      // 0x06         Multifunction select 4 Register                   
sfr MFPSEL5                   =  0xcf;      // 6,8          Multifunction select 5 Register
sfr MFPSEL6                   =  0xd2;      // 0x06         Multifunction select 6 Register
//  Reserved              =  0xd3;      // 0x06         Multifunction Reserved Register
//  Reserved              =  0xd4;      // 0x06         Debug Reserved Register
sfr DBGSEL                    =  0xd5;      // 0x06         Debug Select Register
sfr DBGBOE                    =  0xd6;      // 0x06         DebugB[7:0] Output Enable
sfr DBGACOE                   =  0xd7;      // 0x06         DebugAC Output Enable
sfr DBGDOE                    =  0xd9;      // 0x06         DebugD[7:0] Output Enable
sfr DBGDIE                    =  0xda;      // 0x06         DebugD[7:0] Input Enable
sfr DBGFOE                    =  0xdb;      // 0x06         DebugF[7:0] Output Enable
//  Reserved              =  0xdc;     // 0x06          Debug Reserved Register
sfr ChipVersion               =  0xde;      // 0x06         Chip Version Register
sfr BDOP                      =  0xdf;      // 0x06         Bonding Option Register                   
////////////////////////////////////////////////////////////////////////////////////////////////
// USB
sfr UDMACTRL              =  0x86;      // 0x07         UDMA control register.                        
sfr UDMAADDR              =  0x89;      // 0x07         Select UDMA address and endpoint.             
sfr UMODELENH             =  0x8a;      // 0x07         The length to transfer in U mode. n*512 bytes 
sfr UMODELENL             =  0x8b;      // 0x07         The length to transfer in U mode. n*512 bytes 
sfr NORMALLENH            =  0x8c;      // 0x07         UDMA transfer length high in normal mode      
sfr NORMALLENL            =  0x8d;      // 0x07         UDMA transfer length low in normal mode       
sfr OUTPCKCNTH            =  0x90;      // 0x07         Out transaction packet counter high           
sfr OUTPCKCNTL            =  0x91;      // 0x07         Out transaction packet counter low            
sfr IDVBUSCTRL            =  0x92;      // 0x07         ID&VBUS control                               
sfr USBSTATUS             =  0x93;      // 0x07         USB status                                    
sfr DPDMCTRL              =  0x94;      // 0x07         DP DM control register                        
sfr USB_PHYCTRL           =  0x95;      // 0x07         PHY control register                          
sfr Out0bc_hcin0bc        =  0x97;      // 0x07         Endpoint 0 OUT Byte Count                     
sfr In0bc_hcout0bc        =  0x98;      // 0x07         Endpoint 0 IN Byte Count                      
sfr Ep0cs_hcep0cs         =  0x99;      // 0x07         Endpoint 0 Control and Status                 
sfr In1bch_hcout1bch      =  0x9a;      // 0x07         Endpoint 1 IN Byte Count High                 
sfr In1bcl_hcout1bcl      =  0x9b;      // 0x07         Endpoint 1 IN Byte Count Low                  
sfr In1ctrl_hcout1ctrl    =  0x9c;      // 0x07         Endpoint 1 IN Control                         
sfr In1cs_hcout1cs        =  0x9d;      // 0x07         Endpoint 1 IN Control And Status              
sfr Out2bch_hcin2bch      =  0x9e;      // 0x07         Endpoint 2 OUT Byte Count High                
sfr Out2bcl_hcin2bcl      =  0x9f;      // 0x07         Endpoint 2 OUT Byte Count Low                 
sfr Out2ctrl_hcin2ctrl    =  0xa2;      // 0x07         Endpoint 2 OUT Control                        
sfr Out2cs_hcin2cs        =  0xa3;      // 0x07         Endpoint 2 OUT Control And Status             
sfr In3bc_hcout3bc        =  0xa4;      // 0x07         Endpoint 3 IN Byte Count                      
sfr In3ctrl_hcout3ctrl    =  0xa5;      // 0x07         Endpoint 3 IN Control                         
sfr In3cs_hcout3cs        =  0xa6;      // 0x07         Endpoint 3 IN Control And Status              
sfr Fifo1dat              =  0xa7;      // 0x07         FIFO 1 Data                                   
sfr Fifo2dat              =  0xa9;      // 0x07         FIFO 2 Data                                   
sfr Fifo3dat              =  0xaa;      // 0x07         FIFO 3 Data                                   
sfr Ep0indata             =  0xab;      // 0x07         EP0 IN DATA                                   
sfr Ep0outdata            =  0xac;      // 0x07         EP0 OUT DATA                                  
sfr Usbirq_hcusbirq       =  0xad;      // 0x07         USB Interrupt                                 
sfr Usbien_hcusbien       =  0xae;      // 0x07         USB interrupt enable                          
sfr SHORTPCKIRQ           =  0xaf;      // 0x07         Short packets Interrupt request and enable    
sfr Hcep0ctrl             =  0xb0;      // 0x07         Endpoint 0 Control                            
sfr Hcout0err             =  0xb1;      // 0x07         Endpoint 0 HC OUT Error                       
sfr Hcin0err              =  0xb2;      // 0x07         Endpoint 0 HC IN Error                        
sfr Hcout1ctrl            =  0xb3;      // 0x07         Endpoint 1 HC OUT Control                     
sfr Hcout1err             =  0xb4;      // 0x07         Endpoint 1 HC OUT Error                       
sfr Hcin2ctrl             =  0xb5;      // 0x07         Endpoint 2 HC IN Control                      
sfr Hcin2err              =  0xb6;      // 0x07         Endpoint 2 HC OUT Error                       
sfr Hcout3ctrl            =  0xb9;      // 0x07         Endpoint 3 HC OUT Control                     
sfr Hcout3err             =  0xba;      // 0x07         Endpoint 3 HC OUT Error                       
sfr Setupdat0             =  0xbb;      // 0x07         SETUP DATA0                                   
sfr Setupdat1             =  0xbc;      // 0x07         SETUP DATA1                                   
sfr Setupdat2             =  0xbd;      // 0x07         SETUP DATA2                                   
sfr Setupdat3             =  0xbf;      // 0x07         SETUP DATA3                                   
sfr Setupdat4             =  0xc1;      // 0x07         SETUP DATA4                                   
sfr Setupdat5             =  0xc2;      // 0x07         SETUP DATA5                                   
sfr Setupdat6             =  0xc3;      // 0x07         SETUP DATA6                                   
sfr Setupdat7             =  0xc4;      // 0x07         SETUP DATA7                                   
sfr Ep03irq               =  0xc5;      // 0x07         Endpoint 0 to 3 Interrupt Request             
sfr Ep03ien               =  0xc6;      // 0x07         Endpoint 0 to 3 Interrupt Enables             
sfr Ep03tokirq            =  0xc7;      // 0x07         Endpoint 0 to 3 Token Interrupt Request       
sfr Ep03tokien            =  0xc8;      // 0x07         Endpoint 0 to 3 Token Interrupt Request Enable
sfr IVECT                 =  0xc9;      // 0x07         Interrupt Vector                              
sfr EPRST                 =  0xca;      // 0x07         Endpoint Reset                                
sfr UsbCTRL_STUS          =  0xcb;      // 0x07         USB Control And Status                        
sfr FrmCNTH               =  0xcc;      // 0x07         USB Frame Counter HIGH                        
sfr FrmCNTL               =  0xcd;      // 0x07         USB Frame Counter Low                         
sfr Fnaddr                =  0xce;      // 0x07         Function Address                              
sfr Clkgate               =  0xcf;      // 0x07         Clock Gate                                    
sfr Fifoctrl              =  0xd2;      // 0x07         FIFO Control                                  
sfr Hcportctrl            =  0xd3;      // 0x07         HC Port Control                               
sfr Hcfrmnh               =  0xd4;      // 0x07         HC Frame Number high                          
sfr Hcfrmnl               =  0xd5;      // 0x07         HC Frame Number low                           
sfr Hcfrmremainh          =  0xd6;      // 0x07         HC Frame Remain high                          
sfr Hcfrmremainl          =  0xd7;      // 0x07         HC Frame Remain Low                           
sfr Hcep03errirq          =  0xd8;      // 0x07         HC 0 to 3 Error Interrupt Request             
sfr Hcep03errien          =  0xd9;      // 0x07         HC 0 to 3 Error Interrupt Enable              
sfr Otgirq                =  0xda;      // 0x07         OTG Interrupt Request                         
sfr Otgstate              =  0xdb;      // 0x07         The OTG FSM State                             
sfr Otgctrl               =  0xdc;      // 0x07         OTG Control                                   
sfr Otgstatus             =  0xdd;      // 0x07         OTG Status                                    
sfr Otgien                =  0xde;      // 0x07         OTG Interrupt Enable                          
sfr TaidlbdisTimer        =  0xdf;      // 0x07         OTG ta_aidl_bdis Timer                        
sfr TawaitbconTimer       =  0xe1;      // 0x07         OTG ta_wait_bcon Timer                        
sfr TbvbusplsTimer        =  0xe2;      // 0x07         OTG tb_vbus_pls Timer                         
sfr TbvbusdisplsTimer     =  0xe3;      // 0x07         OTG tb_vbus_dischrg_pls Timer                 
sfr Hcin0maxpck           =  0xe4;      // 0x07         HC IN 0 Max Packet Size                       
sfr Hcin2maxpckh          =  0xe5;      // 0x07         HC IN 2 max packet high                       
sfr Hcin2maxpckl          =  0xe6;      // 0x07         HC IN 2 max packet low                        
sfr Hcout1maxpckh         =  0xe9;      // 0x07         HC OUT 1 max packet low                       
sfr Hcout1maxpckl         =  0xea;      // 0x07         HC OUT 1 max packet high                      
sfr Hcout3maxpck          =  0xe7;      // 0x07         HC OUT 3 max packet                           
sfr USBEIRQ               =  0xeb;      // 0x07         USB external Interrupt request                
sfr AUTONAKCTRL           =  0xec;      // 0x07         auto nak control                              
sfr HCINCTRL              =  0xed;      // 0x07         host in control                               
sfr DBGMODE               =  0xee;      // 0x07         debug mode                                    
sfr VDCTRL                =  0xef;      // 0x07         USB PHY vendor control                        
sfr VDSTAT                =  0xf1;      // 0x07         USB PHY vendor status                         
sfr USBfifodir            =  0xf2;      // 0x07         memory mode select                            
sfr BKDOOR                =  0xf3;      // 0x07         Test back door                                
sfr SRPCTRL               =  0xf4;      // 0x07         SRP control                                   
sfr OTGTRIEN              =  0xf5;      // 0x07         OTR status machine interrupt enable           
sfr OTGTRIRQ              =  0xf6;      // 0x07         OTR status machine interrupt request          
sfr USB_Efuse_Ref         =  0xf7;      // 0x07         Usb Access Efuse_Ref register                 
sfr FSMPRESTATE           =  0xfc;      // 0x07         FSM pre-state register                        
sfr HCIN2CNTH             =  0xfd;      // 0x07         hcin2 packet counter high                     
sfr HCIN2CNTL             =  0xfe;      // 0x07         hcin2 packet counter low                      
////////////////////////////////////////////////////////////////////////////////////////////////
// NAND 
sfr NAND_EN               =  0x90;      // 0x08         NAND flash enable controller register            
sfr NAND_CTL              =  0x91;      // 0x08         NAND flash controller register                   
sfr NAND_STATUS           =  0x92;      // 0x08         NAND flash status register                       
sfr NAND_IARASS           =  0x93;      // 0x08         NAND flash internal access address register      
sfr NAND_SCOLADDR1        =  0x94;      // 0x08         NAND flash spare area column address 1 register  
sfr NAND_SCOLADDR0        =  0x95;      // 0x08         NAND flash spare area column address 0 register  
sfr NAND_MCOLADDR1        =  0x97;      // 0x08         NAND flash main area column address 1 register   
sfr NAND_MCOLADDR0        =  0x98;      // 0x08         NAND flash main area column address 0 register   
sfr NAND_ROWADDR4         =  0x99;      // 0x08         NAND flash row address 4 register                
sfr NAND_ROWADDR3         =  0x9a;      // 0x08         NAND flash row address3 register                 
sfr NAND_ROWADDR2         =  0x9b;      // 0x08         NAND flash row address 2 register                
sfr NAND_ROWADDR1         =  0x9c;      // 0x08         NAND flash row address 1 register                
sfr NAND_ROWADDR0         =  0x9d;      // 0x08         NAND flash row address 0 register                
sfr NAND_DMA5S_DAddr1     =  0x9e;      // 0x08         DMA5 source/destination address 1 register       
sfr NAND_DMA5S_DAddr0     =  0x9f;      // 0x08         DMA5 source/destination address 0 register       
// sfr Reserved              =  0xa2;      // 0x08                                                     
sfr NAND_UDATA0           =  0xa3;      // 0x08         User data 0 register                             
sfr NAND_UDATA1           =  0xa4;      // 0x08         User data 1 register                             
sfr NAND_UDATA2           =  0xa5;      // 0x08         User data 2 register                             
sfr NAND_UDATA3           =  0xa6;      // 0x08         User data 3 register                             
sfr NAND_UDATA4           =  0xa7;      // 0x08         User data 4 register                             
sfr NAND_UDATA5           =  0xa9;      // 0x08         User data 5 register                             
sfr NAND_UDATA6           =  0xaa;      // 0x08         User data 6 register                             
sfr NAND_UDATA7           =  0xab;      // 0x08         User data 7 register                             
sfr NAND_UDATA8           =  0xac;      // 0x08         User data 8 register                             
sfr NAND_UDATA9           =  0xad;      // 0x08         User data 9 register                             
// sfr -                     =  0xae;      // 0x08          Reserved                                        
sfr NAND_FSMCTL           =  0xaf;      // 0x08         NAND flash State machine control Register        
sfr NAND_UCECTL           =  0xb0;      // 0x08         NAND flash User data count & ECC control register
sfr NAND_FSM0_CMD0        =  0xb1;      // 0x08         NAND flash FSM0 Command register_0               
sfr NAND_FSM0_CMD1        =  0xb2;      // 0x08         NAND flash FSM0 Command register_1               
sfr NAND_FSM0_CMD2        =  0xb3;      // 0x08         NAND flash FSM0 Command register_2               
sfr NAND_FSM0_CMD3        =  0xb4;      // 0x08         NAND flash FSM0 Command register_3               
sfr NAND_FSM0_CTL0        =  0xb5;      // 0x08         NAND flash FSM0 controller register0_0           
sfr NAND_FSM0_CTL1        =  0xb6;      // 0x08         NAND flash FSM0 controller register0_1           
sfr NAND_FSM1_CMD0        =  0xb7;      // 0x08         NAND flash FSM1 command register_0               
sfr NAND_FSM1_CMD1        =  0xb8;      // 0x08         NAND flash FSM1 Command register_1               
sfr NAND_FSM1_CMD2        =  0xb9;      // 0x08         NAND flash FSM1 Command register_2               
sfr NAND_FSM1_CMD3        =  0xba;      // 0x08         NAND flash FSM1 Command register_3               
sfr NAND_FSM1_CTL0        =  0xbb;      // 0x08         NAND flash FSM1 controller register0             
sfr NAND_FSM1_CTL1        =  0xbc;      // 0x08         NAND flash FSM1  controller register1            
sfr NAND_FSM2_CMD0        =  0xbd;      // 0x08         NAND flash FSM2 Command register2_0              
sfr NAND_FSM2_CMD1        =  0xbf;      // 0x08         NAND flash FSM2 Command register2_1              
sfr NAND_FSM2_CMD2        =  0xc1;      // 0x08         NAND flash FSM2 Command register2_2              
sfr NAND_FSM2_CMD3        =  0xc2;      // 0x08         NAND flash FSM2 Command register2_3              
sfr NAND_FSM2_CTL0        =  0xc3;      // 0x08         NAND flash FSM2  controller register2_0          
sfr NAND_FSM2_CTL1        =  0xc4;      // 0x08         NAND flash FSM2  controller register2_1          
sfr NAND_ECCS0            =  0xc5;      // 0x08         NAND flash ECC0 Status register                  
sfr NAND_ECCS1            =  0xc6;      // 0x08         NAND flash ECC1 Status register                  
// sfr Reserved              =  0xc7;      // 0x08                                                          
// sfr Reserved              =  0xc8;      // 0x08                                                     
// sfr Reserved              =  0xc9;      // 0x08                                                     
// sfr Reserved              =  0xcc;      // 0x08                                                     
// sfr Reserved              =  0xcd;      // 0x08                                                     
// sfr Reserved              =  0xce;      // 0x08                                                    
// sfr Reserved              =  0xd2;      // 0x08                                                     
////////////////////////////////////////////////////////////////////////////////////////////////
// file system  accelerator
// sfr Reserved              =  0xe9;      // 08~0a                                                                                                
sfr ACC_DMA5S_Addr1       =  0xea;      // 08~0a        DMA5 source Address1 Register(for accelerator)                                          
sfr ACC_DMA5S_Addr0       =  0xeb;      // 08~0a        DMA5 source Address0 Register(for accelerator)                                          
sfr ACC_CTRL              =  0xec;      // 08~0a        Control Register                                                                        
sfr ACC_BLKLEN_LENHI      =  0xed;      // 08~0a        Search block length or the low byte of data length of unsigned accumulation unit number 
sfr ACC_BLKNUM_LENLO      =  0xee;      // 08~0a        Search block number or the high byte of data length of unsigned accumulation unit number
// sfr Reserved              =  0xef;      // 08~0a                                                                                           
sfr ACC_CMPDATA0_SUMHIE   =  0xf1;      // 08~0a        The first byte of data searched or the higher byte of unsigned accelerator’s result    
sfr ACC_CMPDATA1_SUMHI    =  0xf2;      // 08~0a        The second byte of data searched or the high byte of unsigned accelerator’s result     
sfr ACC_CMPDATA2_SUMLO    =  0xf3;      // 08~0a        The third byte of data searched or the low byte of unsigned accelerator’s result       
sfr ACC_CMPDATA3_SUMLOE   =  0xf4;      // 08~0a        The fourth byte of data searched or the lower byte of unsigned accelerator’s result    
sfr ACC_MATCNTHI          =  0xf5;      // 08~0a        The high byte of bitmap                                                                 
sfr ACC_MATCNTLO          =  0xf6;      // 08~0a        The low byte of match number                                                            
sfr ACC_BITMAPHI          =  0xf7;      // 08~0a        The high byte of bitmap                                                                 
sfr ACC_BITMAPLO          =  0xf8;      // 08~0a        The low byte of bitmap                                                                  
// sfr Reserved              =  0xfe;      // 08~0a                                                                                           
////////////////////////////////////////////////////////////////////////////////////////////////
// CARD controller
#if CARD_DEBUG_LEVEL == 1
sfr SDCMD                 =  0x90;      // 0x09         SD/MMC CMD Register                
sfr SDARGRSP              =  0x91;      // 0x09         SD/MMC Argument or RSP Register    
sfr SDRSPPOIN             =  0x92;      // 0x09         SD/MMC RSP Point Register          
sfr SDTFCTL               =  0x93;      // 0x09         SD/MMC control register            
sfr SDSTAT                =  0x94;      // 0x09         MMC/SD status Register             
sfr SDBLKSZH              =  0x97;      // 0x09         SD/MMC High Block size Register    
sfr SDBLKSZL              =  0x98;      // 0x09         SD/MMC Low Block size Register     
sfr SDBLKNUM              =  0x99;      // 0x09         SD/MMC BLOCK number Register       
sfr SDCLKCTL              =  0x9a;      // 0x09         SD/MMC Clock Control Register      
sfr SDPADCTL              =  0x9b;      // 0x09         SD/MMC PAD CONTROL Register        
sfr SDINTCTL              =  0x9c;      // 0x09         SD/MMC INTERRUPT Control Register  
sfr SDDATFIFO             =  0x9d;      // 0x09         SD/MMC Data FIFO Register          
sfr SD_TIMEOUT_CNT        =  0x9e;      // 0x09         Data Timeout Counter Register      
sfr SDPOWERCTL            =  0x9f;      // 0x09         SDVCC and PAD POWER VOLTAGE CONTROL
sfr MS_EN                 =  0xa2;      // 0x09         MSC module and PAD enable          
sfr MS_DAT_FIFO           =  0xa3;      // 0x09         MS Data or FIFO Port Register      
sfr MS_CTL0               =  0xa4;      // 0x09         MS Control Register0               
sfr MS_CTL1               =  0xa5;      // 0x09         MS Control Register1               
sfr MS_FIFO_CTL           =  0xa6;      // 0x09         MS FIFO Control Register           
sfr MS_STAT               =  0xa7;      // 0x09         MS Status Register                 
sfr MS_CNT1               =  0xa9;      // 0x09         Byte Counter Register1             
sfr MS_CNT0               =  0xaa;      // 0x09         Byte Counter Register0             
sfr MS_BLK_CNT            =  0xab;      // 0x09         MS BLOCK Counter Register          
sfr INTTimeoutCounter     =  0xac;      // 0x09         INT Timeout Counter Register       
sfr TimingCTL             =  0xad;      // 0x09         CARD interface timing control      
sfr DMA4_CONFIG           =  0xae;      // 0x09         DMA4 Configure Register            
sfr DMA4_CLK              =  0xaf;      // 0x09         DMA4 Clock Register                
sfr DMA4_ADDRH            =  0xb0;      // 0x09         DMA4 Starting High Address Register
sfr DMA4_ADDRL            =  0xb1;      // 0x09         DMA4 Starting Low Address Register 
sfr DMA4_CTL              =  0xb2;      // 0x09         DMA4 Control Register              
// sfr Reserved              =  0xb3;      // 0x09         -                                  
// sfr Reserved              =  0xb4;      // 0x09         - 
#endif                                 
////////////////////////////////////////////////////////////////////////////////////////////////
//SPI
sfr SPI_CTL               =  0x99;      // 0x0a         SPI Control Register              
sfr SPI_DRQ               =  0x9a;      // 0x0a         SPI DMA/IRQ control Register.    
sfr SPI_STA               =  0x9b;      // 0x0a         SPI Status Register              
sfr SPI_CLKDIV            =  0x9c;      // 0x0a         SPI Clock Divide Control Register
sfr SPI_TXDAT             =  0x9d;      // 0x0a         SPI TX FIFO register             
sfr SPI_RXDAT             =  0x9e;      // 0x0a         SPI RX FIFO register             
sfr SPI_BCL               =  0x9f;      // 0x0a         SPI Bytes Count Low Register     
sfr SPI_BCH               =  0xa2;      // 0x0a         SPI Bytes Count high Register    
sfr SPI_DEBUG             =  0xa3;      // 0x0a         SPI debug register               
// sfr Reserved              =  0xa4;      // 0x0a                                  
////////////////////////////////////////////////////////////////////////////////////////////////
// I2C     
sfr I2C_CTL               =  0x89;      // 0x0a         I2C control register
sfr I2C_STA               =  0x8a;      // 0x0a         I2C status register 
sfr I2C_DAT               =  0x8b;      // 0x0a         I2C data register   
sfr I2C_ADDR              =  0x8c;      // 0x0a         I2C address register
sfr I2C_DEBUG             =  0x8d;      // 0x0a         I2C debug register  
////////////////////////////////////////////////////////////////////////////////////////////////
// UART
sfr UART_BR               =  0x90;      // 0x0a         UART BAUDRATE Register.  
sfr UART_MODE             =  0x91;      // 0x0a         UART mode setup Register.
sfr UART_CTL              =  0x92;      // 0x0a         UART Control Register.   
sfr UART_DRQ              =  0x93;      // 0x0a         UART DRQ/IRQ register    
sfr UART_STA              =  0x94;      // 0x0a         UART Status Register     
sfr UART_TXDAT            =  0x95;      // 0x0a         UART TX FIFO register    
sfr UART_RXDAT            =  0x97;      // 0x0a         UART RX FIFO register    
sfr UART_DEBUG            =  0x98;      // 0x0a         UART debug register.     
////////////////////////////////////////////////////////////////////////////////////////////////
// IR
sfr IR_CTL                =  0xa5;      // 0x0a         IR Control Register        
sfr IR_STA                =  0xa6;      // 0x0a         IR Status Register         
sfr IR_LUC                =  0xa7;      // 0x0a         IR low user code register. 
sfr IR_HUC                =  0xa9;      // 0x0a         IR high user code register.
sfr IR_KDC                =  0xaa;      // 0x0a         IR key data code register. 
sfr IR_DEBUG              =  0xab;      // 0x0a         IR debug register.         
// sfr Reserved              =  0xac;      // 0x0a                               
// sfr Reserved              =  0xad;      // 0x0a                               
////////////////////////////////////////////////////////////////////////////////////////////////
// AMV JPEG
sfr AMVEN                 =  0x90;      // 0x0b         Decoder Enable register        
sfr AMVIE                 =  0x91;      // 0x0b         Decoder Interrupt register     
sfr AMVDecWidth           =  0x92;      // 0x0b         Decoder width register         
sfr AMVDecHeight          =  0x93;      // 0x0b         Decoder height register        
sfr AMVCTL                =  0x94;      // 0x0b         Decoder control register       
sfr AMV_START             =  0x95;      // 0x0b         start bit register             
sfr AMV_HufTab            =  0x97;      // 0x0b         huffman table select register  
sfr AMV_StreamLen         =  0x98;      // 0x0b         Stream length register         
sfr JPEG_DownScale        =  0x99;      // 0x0b         JPEG downscale register        
sfr SliceHeight           =  0x9a;      // 0x0b         Slice height register          
sfr JPEG_AcVlcCode1       =  0x9b;      // 0x0b         JPEG AC VLC code part1         
sfr JPEG_AcVlcCode2       =  0x9c;      // 0x0b         JPEG AC VLC code part2         
sfr JPEG_AcVlcCode3       =  0x9d;      // 0x0b         JPEG AC VLC code part3         
sfr JPEG_AcVlcCode4       =  0x9e;      // 0x0b         JPEG AC VLC code part4         
sfr JPEG_AcVlcCode5       =  0x9f;      // 0x0b         JPEG AC VLC code part5         
sfr JPEG_AcVlcCode6       =  0xa2;      // 0x0b         JPEG AC VLC code part6         
sfr JPEG_AcVlcCode7       =  0xa3;      // 0x0b         JPEG AC VLC code part7         
sfr JPEG_AcVlcCode8       =  0xa4;      // 0x0b         JPEG AC VLC code part8         
sfr JPEG_AcVlcCode9       =  0xa5;      // 0x0b         JPEG AC VLC code part9         
sfr JPEG_AcVlcCode10      =  0xa6;      // 0x0b         JPEG AC VLC code part10        
sfr JPEG_AcVlcCode11      =  0xa7;      // 0x0b         JPEG AC VLC code part11        
sfr JPEG_AcVlcCode12      =  0xa9;      // 0x0b         JPEG AC VLC code part12        
sfr JPEG_AcVlcCode13      =  0xaa;      // 0x0b         JPEG AC VLC code part13        
sfr JPEG_AcVlcCode14      =  0xab;      // 0x0b         JPEG AC VLC code part14        
sfr JPEG_AcVlcCode15      =  0xac;      // 0x0b         JPEG AC VLC code part15        
sfr JPEG_AcVlcCode16      =  0xad;      // 0x0b         JPEG AC VLC code part16        
sfr JPEG_AcVlcCode17      =  0xae;      // 0x0b         JPEG AC VLC code part17        
sfr JPEG_AcVlcCode18      =  0xaf;      // 0x0b         JPEG AC VLC code part18        
sfr JPEG_AcVlcCode19      =  0xb0;      // 0x0b         JPEG AC VLC code part19        
sfr JPEG_AcVlcCode20      =  0xb1;      // 0x0b         JPEG AC VLC code part20        
sfr JPEG_AcVlcCode21      =  0xb2;      // 0x0b         JPEG AC VLC code part21        
sfr JPEG_AcVlcCode22      =  0xb3;      // 0x0b         JPEG AC VLC code part22        
sfr JPEG_AcVlcCode23      =  0xb4;      // 0x0b         JPEG AC VLC code part23        
sfr JPEG_AcVlcCode24      =  0xb5;      // 0x0b         JPEG AC VLC code part24        
sfr JPEG_AcVlcCode25      =  0xb6;      // 0x0b         JPEG AC VLC code part25        
sfr JPEG_AcVlcCode26      =  0xb7;      // 0x0b         JPEG AC VLC code part26        
sfr JPEG_AcVlcCode27      =  0xb8;      // 0x0b         JPEG AC VLC code part27        
sfr JPEG_AcVlcCode28      =  0xb9;      // 0x0b         JPEG AC VLC code part28        
sfr JPEG_AcVlcCode29      =  0xba;      // 0x0b         JPEG AC VLC code part29        
sfr JPEG_AcVlcCode30      =  0xbb;      // 0x0b         JPEG AC VLC code part30        
sfr JPEG_AcVlcCode31      =  0xbc;      // 0x0b         JPEG AC VLC code part31        
sfr JPEG_AcVlcCode32      =  0xbd;      // 0x0b         JPEG AC VLC code part32        
sfr JPEG_DcVlcCode1       =  0xbf;      // 0x0b         JPEG DC VLC code part1         
sfr JPEG_DcVlcCode2       =  0xc1;      // 0x0b         JPEG DC VLC code part2         
sfr JPEG_DcVlcCode3       =  0xc2;      // 0x0b         JPEG DC VLC code part3         
sfr JPEG_DcVlcCode4       =  0xc3;      // 0x0b         JPEG DC VLC code part4         
sfr JPEG_DcVlcCode5       =  0xc4;      // 0x0b         JPEG DC VLC code part5         
sfr JPEG_DcVlcCode6       =  0xc5;      // 0x0b         JPEG DC VLC code part6         
sfr JPEG_DcVlcCode7       =  0xc6;      // 0x0b         JPEG DC VLC code part7         
sfr JPEG_DcVlcCode8       =  0xc7;      // 0x0b         JPEG DC VLC code part8         
sfr JPEG_DcVlcCode9       =  0xc8;      // 0x0b         JPEG DC VLC code part9         
sfr JPEG_DcVlcCode10      =  0xc9;      // 0x0b         JPEG DC VLC code part10        
sfr JPEG_DcVlcCode11      =  0xca;      // 0x0b         JPEG DC VLC code part11        
sfr JPEG_DcVlcCode12      =  0xcb;      // 0x0b         JPEG DC VLC code part12        
sfr JPEG_DcVlcCode13      =  0xcc;      // 0x0b         JPEG DC VLC code part13        
sfr JPEG_DcVlcCode14      =  0xcd;      // 0x0b         JPEG DC VLC code part14        
sfr JPEG_DcVlcCode15      =  0xce;      // 0x0b         JPEG DC VLC code part15        
sfr JPEG_DcVlcCode16      =  0xcf;      // 0x0b         JPEG DC VLC code part16        
sfr MemMapReg             =  0xd2;      // 0x0b         DECODER memory mapping register
sfr DecClkDiv             =  0xd3;      // 0x0b         Decoder clock divide register  
sfr Debug                 =  0xd4;      // 0x0b         Debug reigster                 
// sfr Reserved              =  0xd5;      // 0x0b                                   
////////////////////////////////////////////////////////////////////////////////////////////////
// RTC/CTC
sfr RTC_CTL0              =  0xa9;      // 0x0c         RTC Control 0 register         
sfr RTCTimeS              =  0xaa;      // 0x0c         RTC Time Second Register       
sfr RTCTimeMin            =  0xab;      // 0x0c         RTC Time Minute Register       
sfr RTCTimeH              =  0xac;      // 0x0c         RTC Time Hour Register         
sfr RTCTimeD              =  0xad;      // 0x0c         RTC Time Day Register          
sfr RTCTimeMon            =  0xae;      // 0x0c         RTC Time Month Register        
sfr RTCTimeYear           =  0xaf;      // 0x0c         RTC Time Year Register         
sfr RTCAlmS               =  0xb0;      // 0x0c         RTC Alarm Second Register      
sfr RTCALMM               =  0xb1;      // 0x0c         RTC Alarm Minute Register      
sfr RTCALMH               =  0xb2;      // 0x0c         RTC Alarm Hour Register        
sfr ADJCtrl               =  0xb3;      // 0x0c         adjust circuit control Register
sfr RTCrdm                =  0xb4;      // 0x0c         RTC Random access Register     
sfr RTCREGUPDATE          =  0xb5;      // 0x0c         RTC Register update Register   
sfr RTC_CTL1              =  0xb6;      // 0x0c         RTC Control 1 register         
sfr TimerLB               =  0xb7;      // 0x0c         Timer low Byte                 
sfr TimerMB               =  0xb8;      // 0x0c         Timer middle Byte              
sfr TimerHB               =  0xb9;      // 0x0c         Timer high Byte                
sfr WDCtl                 =  0xba;      // 0x0c         watch dog control register     
 
sfr CTCCtl                =  0xbb;      // 0x0c         CTC control register           
sfr CTCCNTL               =  0xbc;      // 0x0c         CTC counter low register      
sfr CTCCNTH               =  0xbd;      // 0x0c         CTC counter high register
sfr RTCrdm_2              =  0xc8;      // 0x0c         RTC Random access Register2  
sfr RTCrdm_3              =  0xc9;      // 0x0c         RTC Random access Register3
sfr RTCrdm_4              =  0xca;      // 0x0c         RTC Random access Register4       
////////////////////////////////////////////////////////////////////////////////////////////////
// RMU
sfr MRCR1                 =  0x86;      // 0x01         Module Reset Control Register 1
sfr PCON                  =  0x87;      // all bank     Power Control                  
sfr MRCR2                 =  0x89;      // 0x01         Module Reset Control Register 2
sfr MRCR3                 =  0x9f;      // 0x01         Module Reset Control Register 3
////////////////////////////////////////////////////////////////////////////////////////////////
// CMU
sfr MCSR1                 =  0xa9;      // 0x01         Module Clock Select Register 1  
sfr MCSR2                 =  0xaa;      // 0x01         Module Clock Select Register 2  
sfr MCSR3                 =  0xab;      // 0x01         Module Clock Select Register 3  
sfr MCSR4                 =  0xac;      // 0x01         Module Clock Select Register 4  
sfr MCSR5                 =  0xad;      // 0x01         Module Clock Select Register 5  
sfr MCSR6                 =  0xae;      // 0x01         Module Clock Select Register 6  
sfr MCSR7                 =  0xaf;      // 0x01         Module Clock Select Register 7  
sfr MCSR8                 =  0xb0;      // 0x01         Module Clock Select Register 8  
sfr MCSR9                 =  0xb1;      // 0x01         Module Clock Select Register 9  
sfr MCSR10                =  0xb2;      // 0x01         Module Clock Select Register 10 
sfr MCSR11                =  0xb4;      // 0x01         Module Clock Select Register 11 
sfr LOSC1_CTL             =  0xb6;      // 0x01         Single pin LOSC Control Register
sfr LOSC2_CTL             =  0xb7;      // 0x01         Dual pin LOSC Control Register  
sfr LFPLL_CTL             =  0xb8;      // 0x01         LFPLL Control Register          
sfr LFPLL_STATUS          =  0xb9;      // 0x01         LFPLL Status Register           
sfr HOSC_CTL              =  0xba;      // 0x01         HOSC Control Register           
sfr AUDIOPLL_CTL          =  0xbb;      // 0x01         AudioPLL Control Register       
sfr CK48M_CTL             =  0xbc;      // 0x01         CK48M Control Register          
sfr Reserved0             =  0xbd;      // 0x01                                         
sfr MCSR12                =  0xbf;      // 0x01         Module Clock Select Register 12 
sfr Reserved1             =  0xc1;      // 0x01 
sfr MCUPLL_CTL            =  0xb3;      // 0x01         MCU PLL Register                               
////////////////////////////////////////////////////////////////////////////////////////////////
// B1/B2 controller             
sfr UMCTL                 =  0xa7;      // 0x01         UDISK Mode Control Register
sfr IMCSR7                =  0xc2;      // 0x01         Image register of Module Clock Select Register 7
sfr IMCSR8                =  0xc3;      // 0x01         Image register of Module Clock Select Register 8
sfr IMCSR9                =  0xc4;      // 0x01         Image register of Module Clock Select Register 9
sfr IMCSR10               =  0xc5;      // 0x01         Image register of Module Clock Select Register 10
sfr IMCSR11               =  0xc6;      // 0x01         Image register of Module Clock Select Register 11
sfr IMCSR12               =  0xc7;      // 0x01         Image register of Module Clock Select Register 12
sfr ICCTL                 =  0xc8;      // 0x01         Image clock control register
sfr DMASTARTH             =  0xc9;      // 0x01         DMA 4 or DMA5 transfer start address high byte
sfr DMASTARTL             =  0xca;      // 0x01         DMA 4 or DMA5 transfer start address low byte
sfr DMALENH               =  0xcb;      // 0x01         DMA 4 or DMA5 transfer length high byte
sfr DMALENL               =  0xcc;      // 0x01         DMA 4 or DMA5 transfer length low byte
////////////////////////////////////////////////////////////////////////////////////////////////
// BIST controller
sfr bist_en1              =  0xe1;      // 0x01         Bist enable register1
sfr bist_en2              =  0xe2;      // 0x01         Bist enable register2
sfr bist_en3              =  0xe3;      // 0x01         Bist enable register3
sfr bist_fin1             =  0xe4;      // 0x01         Bist finish flag register1
sfr bist_fin2             =  0xe5;      // 0x01         Bist finish flag register2
sfr bist_fin3             =  0xe6;      // 0x01         Bist finish flag register3
sfr bist_info1            =  0xe7;      // 0x01         Bist information register1
sfr bist_info2            =  0xe8;      // 0x01         Bist information register2
sfr bist_info3            =  0xea;      // 0x01         Bist information register3
////////DATA////////////////////////////////////////////////////////////////////////////////////////
// fpga verify sd controller  
#if CARD_DEBUG_LEVEL == 0
sfr SDCMD                 =  0x90;      // 0xfe         SD/MMC CMD Register                 
sfr SDARGRSP              =  0x91;      // 0xfe         SD/MMC Argument or RSP Register    
sfr SDRSPPOIN             =  0x92;      // 0xfe         SD/MMC RSP Point Register          
sfr SDCTL                 =  0x93;      // 0xfe         SD/MMC control register            
sfr SDSTAT                =  0x94;      // 0xfe         MMC/SD status Register             
sfr SDBLKSZH              =  0x98;      // 0xfe         SD/MMC High Block size Register    
sfr SDBLKSZL              =  0x97;      // 0xfe         SD/MMC Low Block size Register     
sfr SDBLKNUM              =  0x99;      // 0xfe         SD/MMC BLOCK number Register       
sfr SDCLKCTL              =  0x9a;      // 0xfe         SD/MMC Clock Control Register      
sfr SDEN                  =  0x9b;      // 0xfe         SD/MMC Module Enable Register      
sfr SDFIFOCTL             =  0x9c;      // 0xfe         SD/MMC FIFO Control Register       
sfr SDDATFIFO             =  0x9d;      // 0xfe         SD/MMC Data FIFO Register          
sfr SD_TIMEOUT_CNT        =  0x9e;      // 0xfe         SD Data Timeout Counter Register   
sfr MS_EN                 =  0x9f;      // 0xfe         MS Card EN control register        
sfr MS_DAT_FIFO           =  0xa2;      // 0xfe         MS Data or FIFO Port Register      
sfr MS_CTL0               =  0xa3;      // 0xfe         MS Control Register0               
sfr MS_CTL1               =  0xa4;      // 0xfe         MS Control Register1               
sfr MS_FIFO_CTL           =  0xa5;      // 0xfe         MS FIFO Control Register           
sfr MS_CNT1               =  0xa6;      // 0xfe         Byte Counter Register1             
sfr MS_CNT0               =  0xa7;      // 0xfe         Byte Counter Register0             
sfr MS_STAT               =  0xa9;      // 0xfe         MS Status Register                 
sfr MS_BLK_CNT            =  0xaa;      // 0xfe         MS BLOCK Counter Register          
sfr INTTimeoutCounter     =  0xab;      // 0xfe         MS INT Timeout Counter Register    
sfr CARD_TimingCTL        =  0xac;      // 0xfe         CARD Interface Timing CTL          
sfr DMA4_CONFIG           =  0xad;      // 0xfe         DMA4 Configure control Register    
sfr DMA4_CLK              =  0xae;      // 0xfe         DMA4 Clock Register                
sfr DMA4_STAH             =  0xb0;      // 0xfe         DMA4 Starting High Address Register
sfr DMA4_STAL             =  0xaf;      // 0xfe         DMA4 Starting Low Address Register 
sfr DMA4_CTL              =  0xb1;      // 0xfe         SDVCC  AND DMA4 CTL                
// sfr Reserved              =  0xb2;      // 0xfe         -                                  
// sfr Reserved              =  0xb3;      // 0xfe         -                                  
// sfr Reserved              =  0xb4;      // 0xfe         -   
#endif                              
////////////////////////////////////////////////////////////////////////////////////////////////
// fpga verify GPIO    
//#if GPIO_DEBUG_LEVEL == 0
//sfr GPIOAOUTEN            =  0xa2;      // 0xff         General Purpose Input Output Group A Output Enable
//sfr GPIOAINEN             =  0xa3;      // 0xff         General Purpose Input Output Group A Input Enable 
//sfr GPIOADAT              =  0xa4;      // 0xff         General Purpose Input Output Group A Data         
//sfr GPIOBOUTEN            =  0xa5;      // 0xff         General Purpose Input Output Group B Output Enable
//sfr GPIOBINEN             =  0xa6;      // 0xff         General Purpose Input Output Group B Input Enable 
//sfr GPIOBDAT              =  0xa7;      // 0xff         General Purpose Input Output Group B Data         
//#endif
#endif

















































































































































































































































































































































































































































































































































































































































