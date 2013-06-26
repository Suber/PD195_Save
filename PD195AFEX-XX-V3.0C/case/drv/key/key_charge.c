#include "actos.h"
#include "irq.h"
#include "string.h"
#include "ap_cfg.h"

extern void _nop_(void);
uint8 sKY_ChargeSet(uint8 Setting); //充电设置/状态获取
void sKY_ChargeGetandSet(void); //充电检测
void Delay_500ms(void);
void Delay_10ms(void);


void stop_charge_exit(void);

#define 	BATLOW_VEL		0x44//	//3.0V，检测低电
#define		BATNON_VEL		0x5d//	//3.6V,检测无电池
#define 	BATFULL_VEL		0x73//	//4.1V,	检测电池满
#define 	BATFULL_VEL1	0x75//0x77	//4.2V,检测电池满
#define   ChargeCurrent25mA       0x01
#define   ChargeCurrent50mA       0x02
#define   ChargeCurrent100mA      0x03
#define   ChargeCurrent150mA      0x04
#define   ChargeCurrent200mA      0x05
#define   ChargeCurrent250mA      0x06
#define   ChargeCurrent300mA      0x07
#define   ChargeCurrent350mA      0x08
#define   ChargeCurrent400mA      0x09
#define   ChargeCurrent450mA      0x0a
#define   ChargeCurrent500mA      0x0b

#pragma	renamecode(?PR?BAC_CHARGE)

uint8 TheCharge = 0; //充电标识，0,stop；1，start
uint8 ChargeCurrent = 0; //充电电流选择
uint8 TrickleTimeCounter = 0; //触发计数器，30秒计数
uint8 BatteryFullFlag = 0; //充满标识
uint8 LowestCurrentCnt = 0;
uint8 CurrentSetFlag = 0;
uint8 UsbUnStick = 0;
/*
 **************************************************************
 *  char    sKY_ChargeSet(char type)          *
 *  input:   type,  0:get the charge status
 *                       1: start charge battery
 *                       2:stop charge battery
 *   output:
 *              char charge status:  0:no charge
 *                                   1:charge state
 2:batteryfull
 **************************************************************
 */
uint8 sKY_ChargeSet(uint8 Setting)
{
    if (Setting == 0)
    {
        //get charge status
        if (TheCharge == 1)
        {
            return 1;
        }
        else
        {
            if (BatteryFullFlag == 1)
            {
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }
    else if (Setting == 1)
    {
        //start charge,set charge flag
        TheCharge = 1;
    }
    else if (Setting == 2)
    {
        //stop charge, clr charge flag
        TheCharge = 0;
    }
    else
    {
        ;//none
    }
    return 0;
}

/*
 **************************************************************************
 *  void    sKY_ChargeGetandSet(char void)          *
 *  input:   NONE
 *  output: NONE
 *  description: This function is used for controlling the charging process
 **************************************************************************
 */
void sKY_ChargeGetandSet(void)
{
    uint8 sfr_bak;
    uint8 tmp1;

    sfr_bak = SFR_BANK;
    Delay_10ms();

    SFR_BANK = BANK_PMU;
    #if(MACHINE_MODEL==0)
    if((SYSTEM_VOL & 0x80) != 0) //使用SYSTEM_VOL判别USB连接
    #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    if((SYSTEM_VOL & 0x80) || (CHG_CTL & 0x80))
    #endif
    {
        UsbUnStick = 0;
        //usb connect
        if ((TheCharge == 0) || (TheCharge == 2))
        {
            stop_charge_exit();
            goto exit;
        }
        else if (TheCharge == 1)
        {
            //3.检测电池电压是否小于3.0V,判断是否需要检测有无电池
            if (BATADC_DATA < BATLOW_VEL)
            {
                //3.1若小于3.0V，屏蔽涓流，打开充电电流50ma充电，delay 500ms
                CHG_CTL &= ~1;
                CHG_CTL |= 1 << 1;
                CHG_CTL &= ~(1 << 2); //设置充电电流：50mA
                CHG_CTL |= 1 << 4; //屏蔽涓流,硬件做反，写1为屏蔽

                SYSTEM_VOL |= 1 << 6; //设置41us检测时间
                CHG_CTL |= 1 << 5; //使能充电
                Delay_500ms(); //延迟500ms

                //3.1.1检测电池电压是否小于3.6V
                if (BATADC_DATA < BATNON_VEL)
                {
                    //have battery,wait next check
                    goto exit;
                }
                else
                {
                    //no battery
                    stop_charge_exit();
                    goto exit;
                }
            }
            else
            {
                //no need check battery exist
                //3.2若电池电压大于3.0V，判断是否已设定用户充电电流
                if (CurrentSetFlag == 1)
                {
                    //3.2.1若已设定用户电流，检测status是否为0
                    if ((CHG_DET & 0x01) == 0)
                    {
                        //3.2.1.1 status=0，判断充电电流是否大于25ma
                        if ((CHG_CTL & 0x0f) > ChargeCurrent25mA)
                        {
                            //3.2.1.1.1 电流>25mA，说明负载能力不足，逐级减小50ma充电电流
                            LowestCurrentCnt = 0; //低电计数器清0
                            tmp1 = (CHG_CTL & 0x0f) - 1;
                            CHG_CTL = (CHG_CTL & 0xf0) | tmp1; //电流减50mA

                            //3.2.1.1.1.1 检测电压(BATADC)是否>4.2V
             BatFullCheck: if (BATADC_DATA > BATFULL_VEL1)
                            {
                                //3.2.1.1.1.1.1 大于4.2V，判断是否已加充10分钟
                                if (TrickleTimeCounter < 20) //30s进一次chargeSetGet
                                {
                                    //3.2.1.1.1.1.1.1 未加充，加充计时器累加1，退出
                                    TrickleTimeCounter++;		  
                                    goto exit;
                                }
                                else
                                {
                                    //3.2.1.1.1.1.1.2 已加充，则断开内部充电电路（操作REG[CHG_EN]），延时500ms
                                    CHG_CTL &= ~(1 << 5);
                                    Delay_500ms();

                                    //3.2.1.1.1.1.1.2.1 检测电池电压是否>4.2V
                                    if (BATADC_DATA > BATFULL_VEL1)
                                    {
                                        //3.2.1.1.1.1.1.2.1.1 是，则置充电已满标志,停止充电，初始化充电相关参数，退出
                                        BatteryFullFlag = 1;
                                        stop_charge_exit();	
                                        goto exit;
                                    }
                                    else
                                    {
                                        //3.2.1.1.1.1.1.2.1.1 否，则打开充电电路，初始化加充定时器，退出
                                        CHG_CTL |= 1 << 5;
                                        TrickleTimeCounter = 0;
                                        goto exit;
                                    }
                                }
                            }
                            else
                            {
                                //电池电压<4.2V，退出，等待下次检测
                                goto exit;
                            }
                        }
                        else

                        {
                            //3.2.1.1.2	电流<25mA，低电计数器加一
                            LowestCurrentCnt++;

                            //3.2.1.1.2.1 判断低电计数器是否>3
                            if (LowestCurrentCnt > 3)
                            {
                                //计数器>3，表明电池过充被保护或者负载能力不足
                                //置充电已满，停止，退出
                                BatteryFullFlag = 1;
                                stop_charge_exit();
                                goto exit;
                            }
                            else
                            {
                                //计数器<3
                                //转到3.2.1.1.1.1，检测电池电压>4.2V
                                goto BatFullCheck;
                            }
                        }
                    }
                    else
                    {
                        //3.2.1.2 status!=0,跳转3.2.1.1.1.1，检测电压(BATADC)是否>4.2V
                        goto BatFullCheck;
                    }
                }
                else
                {
                    //3.2.2若未设定用户电流，检测电池电压是否<4.2V
                    if (BATADC_DATA < BATFULL_VEL1)
                    {
                        //屏蔽涓流，以100mA使能充电，等待0.5s
                        CHG_CTL |= 1;
                        CHG_CTL |= 1 << 1;
                        CHG_CTL &= ~(1 << 2);
                        CHG_CTL &= ~(1 << 3);
                        CHG_CTL |= 1 << 4;
                        CHG_CTL |= 1 << 5;

                        Delay_500ms();

                        //设置用户充电电流，退出
                        #if(MACHINE_MODEL==1)
                        ChargeCurrent = ChargeCurrent150mA; //用户电流设为250mA
                        #elif(MACHINE_MODEL == 2)
                    ChargeCurrent = ChargeCurrent50mA; //用户电流设为250mA
                        #endif

                        tmp1 = (CHG_CTL & 0xf0) | 0x0f;
                        CHG_CTL = (ChargeCurrent | 0xf0) & tmp1; //当前current存到CHG_CTL
                        CurrentSetFlag = 1; //设置用户电流设置标识
                        goto exit;
                    }
                    else
                    {
                        //3.2.2.2 电压>4.2V，说明电池已满,置充电已满标志
                        BatteryFullFlag = 1;
                        stop_charge_exit();
                        goto exit;
                    }
                }
            }
        }
    }
    else
    {
        //no usb cable connect
        //goto exit;
		if(!UsbUnStick)
		{
			stop_charge_exit();
		}
		UsbUnStick = 1;
    }

    exit: SFR_BANK = sfr_bak;
    return;
}

void stop_charge_exit(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

    SFR_BANK = BANK_PMU;
    CHG_CTL &= ~(1 << 5); //停止充电
    TheCharge = 0;
    CurrentSetFlag = 0;
    LowestCurrentCnt = 0;

    SFR_BANK = sfr_bak;
}


void Delay_10ms(void) //24Mhz，延时10ms
{
    uint8 i, j;
    for (i = 0; i < 255; i++)
    {
        for (j = 0; j < 255; j++)
        {
            _nop_();
            _nop_();
        }
    }
}

void Delay_500ms(void) //24Mhz，延时500ms
{
    uint8 i;
    for (i = 0; i < 50; i++)
    {
        Delay_10ms();
    }
}
