#include "actos.h"
#include "chip_cfg.h"

#define VCCOUT_CARD      1
#define CARDSUPPORT4LINE 1

#pragma renamecode(?PR?KY_CARD)

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

void CardPowerOff(void);
void sKY_CheckCardStatus(void);
void _Delay(void);
extern void _nop_(void);


#if 0 //20110424	因为触摸按键代码长，故屏蔽卡的检测先
/*
 *******************************************************************************
 *                CardPowerOff{RPrefix}:
 *
 * Descriptor:
 *
 * Arguments: VOID
 *
 * Returns:   VOID
 *******************************************************************************
 */
void CardPowerOff(void)
{
    uint8 sfr_bak;

    sfr_bak = SFR_BANK;
    cardpowerflag &= 0xfe;
    SFR_BANK = BANK_GPIO;

    MFPSEL4 &= ~(1 << 2);
    SFR_BANK = BANK_PMU;
    MULTI_USED &= ~1; //SD_VCCOUT_SS disable
    MULTI_USED &= ~(1 << 1); //SD_VCCOUT_EN disable

    SFR_BANK = BANK_GPIO;

    pGPIOF_OUTPUT_EN |= GPIOF_OUTPUT_8LINE_SET;
    pGPIOF_DATA_EN &= GPIOF_DATA_CLR;
    SFR_BANK = sfr_bak;
}

/*
 *******************************************************************************
 *                sKY_CheckCardStatus{RPrefix}:
 *
 * Descriptor:
 *
 * Arguments: VOID
 *
 * Returns:   VOID
 *******************************************************************************
 */

void sKY_CheckCardStatus(void)
{
    // step 1)
    uint8 sfr_bak;
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

    if (SuppCardFlag != 0)
    {
        SFR_BANK = BANK_PMU;
        MULTI_USED &= ~(1 << 2);
        _Delay();
        SFR_BANK = BANK_GPIO;
        MFPSEL4 |= 1 << 3;

        if ((pSDDET_GPIO_INPUT_EN & CARD_CD_SET) == 0)
        {
            //have card
            pSDDET_GPIO_OUTPUT_EN &= CARD_CD_CLR;
            pSDDET_GPIO_INPUT_EN |= CARD_CD_SET; // 2) enable CARD_CD INPUT
        }
        else
        {
            //start detect
            if ((pSDDET_GPIO_INPUT_DAT & CARD_CD_SET) == 0) 
            {
                if (CardDetectState == 0)				          //card detect
                {
                    CardDetectState = 3;
                    WPCardStatue = 0;
                }
                else
                {
                    //_card_detect_over
                    //CardDetectState = 0;
                    WPCardStatue = 0;
                }
            }
            else
            {								// gpio detect no card
                cardpowerflag &= 0xef;

                if ((cardpowerflag & 0x01) == 1)
                {
                    //power off
                    PADPUPD1 &= 0xcf; // disable 50k pull_up sdmmc cmd,data0~7
                    CardPowerOff();
                }

                //not need powerOff
                cardoutflag = 1;
                CardDetectState = 0;
                WPCardStatue = 0;
            }
        }
    }
    else
    {
        CardDetectState = 0;
        WPCardStatue = 0;
    }

	SFR_BANK = BANK_PMU;
	MCSR2 = mcsr2_bak;
	MCSR4 = mcsr4_bak; 
	_nop_();
	_nop_();
	_nop_(); 
    SFR_BANK = sfr_bak;
}


void _Delay(void)
{
    uint8 i;
    for (i = 0; i < 0xe0; i++)
    {
        _nop_();
    }
}
#endif
void _Delay(void)
{ 
}
