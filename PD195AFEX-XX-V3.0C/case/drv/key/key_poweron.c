#include "actos.h"
#include "chip_cfg.h"
//#include <intrins.h>


#define VCCOUT_CARD      1
#define CARDSUPPORT4LINE 1

extern void _nop_(void);
extern uint8 SuppCardFlag;
extern uint8 CardDetectState;
extern uint8 WPCardStatue;
extern uint8 cardpowerflag;
extern uint8 cardoutflag;

sfr pSDCMD_GPIO_REG = SDCMD_GPIO_REG;
sfr pSDCLK_GPIO_REG = SDCLK_GPIO_REG;
sfr pSDVCC_GIO_EN_REG = SDVCC_GIO_EN_REG;
sfr pSDVCC_GIO_DATA_REG = SDVCC_GIO_DATA_REG;
sfr pSDVCC_GIO_INTPUT_REG = SDVCC_GIO_INTPUT_REG;
sfr pGPIOF_OUTPUT_EN = GPIOF_OUTPUT_EN;
sfr pDETECT_INPUT_EN = DETECT_INPUT_EN;
sfr pGPIOF_DATA_EN = GPIOF_DATA_EN;
sfr pSDDET_GPIO_INPUT_DAT = SDDET_GPIO_INPUT_DAT;
sfr pSDDET_GPIO_INPUT_EN = SDDET_GPIO_INPUT_EN;
sfr pSDDET_GPIO_OUTPUT_EN = SDDET_GPIO_OUTPUT_EN;

uint8 sKY_CardPowerOn(uint8 mode);
extern void CardPowerOff(void);
void Wait50ms(void);
void Wait25ms(void);
//tern void _Delay(void);

#pragma renamecode(?PR?BAC_POWERON)



/*
 *******************************************************************************
 *                sKY_CardPowerOn{RPrefix}:
 *
 * Descriptor:
 *
 * Arguments: VOID
 *
 * Returns:   VOID
 *******************************************************************************
 */

uint8 sKY_CardPowerOn(uint8 mode)
{
    uint8 sfr_bak;
    uint8 i;
    uint8 ret;
	uint8 mcsr2_bak, mcsr4_bak;
    sfr_bak = SFR_BANK;
	//mcu clk切到clk24M
	SFR_BANK = BANK_PMU;
	mcsr2_bak = MCSR2;
	mcsr4_bak = MCSR4;
	MCSR2 &= ~(1<<7);	  //CLK DIV=1
	MCSR2 &= ~(1<<6);
	MCSR4 |= 1<<1;		  //SELECT MCU CLK FORM CLK24M
	MCSR4 &= ~1;
	_nop_();
	_nop_();
	_nop_();

    SFR_BANK = BANK_GPIO;

    pSDCMD_GPIO_REG &= SDCMD_GPIO_MASK; // 1) dsiable cmd/clk gpio
    pSDCLK_GPIO_REG &= SDCLK_GPIO_MASK;

    if (mode == 1)
    {
        //powerOn
        if ((cardpowerflag & 0x01) == 0)
        {
            //need powerOn
            cardpowerflag |= 1;
            pSDVCC_GIO_DATA_REG &= SDVCC_CLR_BIT; // 4)
            pSDVCC_GIO_EN_REG &= SDVCC_CLR_BIT; // 2) close GPIO out
            pSDVCC_GIO_INTPUT_REG &= SDVCC_CLR_BIT;

            MFPSEL4 &= ~(1 << 2); // 5)
            Wait50ms();

            SFR_BANK = BANK_PMU;
            MULTI_USED |= 1;

            for (i = 0; i < 48; i++)
            {
                MULTI_USED |= 1 << 1;
                _nop_();
                MULTI_USED &= ~(1 << 1);
            }

            MULTI_USED |= 1 << 1;
            SFR_BANK = BANK_GPIO;

			//_Delay
			for (i = 0; i < 0xe0; i++)
		    {
		        _nop_();
		    }            

			pGPIOF_OUTPUT_EN &= GPIOF_OUTPUT_8LINE_CLR; // 6)
        }
    }
    else
    {
#if 0 //20110424	因为触摸按键代码长，故屏蔽卡的检测先
        //powerOff
        CardPowerOff();
#endif
    }

#ifdef CARDSUPPORT8LINE
    ret = 0x08;
#endif

#ifdef CARDSUPPORT4LINE
    ret = 0x04;
#endif

#ifdef CARDSUPPORT1LINE
    ret = 0x01;
#endif

#ifdef  _CARD_CMD_DETECT
    ret |= 0x80;
#endif

#ifdef  VCCOUT_CARD
    ret |= 0x10; // clk1 as MS/SD clk
#endif

	SFR_BANK = BANK_PMU;
	MCSR2 = mcsr2_bak;
	MCSR4 = mcsr4_bak; 
	_nop_();
	_nop_();
	_nop_();
    SFR_BANK = sfr_bak;
    return ret;
}

void Wait50ms(void)
{
    Wait25ms();
    Wait25ms();
}

void Wait25ms(void)
{
    uint16 i;
    for (i = 0; i < 0xdfff; i++)
    {
        _nop_();
    }
}

