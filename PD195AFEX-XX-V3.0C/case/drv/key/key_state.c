#include "actos.h"
#include "key.h"

extern uint8 const key_map[];

extern uint32 step_counter;		//20110524
extern uint32 step_second;
extern uint32 step_counter_swim;		//20110524	已运动步数
extern uint32 step_second_swim;
extern uint16 gsensor_xadc1;
extern uint16 gsensor_yadc1;
extern uint16 gsensor_zadc1;
extern uint32 gf_tmp32;
extern uint8 gsensor_workflag;
extern uint8 gsensor_workflag_swim;
extern uint8 gsensor_period;
extern uint8 gsensor_got;
extern uint16 gsensor_gap;
extern uint16 gsensor_gap_last;


#pragma renamecode(?PR?BAC_STATE)

uint8 *sKY_GetKeyTabAddress(void)
{
    return key_map;
}

bool sKY_HoldCheck(void)
{
    uint8 sfr_bak;
    uint8 key_val;
    uint8 ret;

    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_PMU;
    key_val = LRADC1_DATA & 0x3f;
    if (   key_val < 60)
    {
        ret = 0; //unhold
    }
    else
    {
        //ret = 1; //hold
        ret = 0; //SC903
    }

    SFR_BANK = sfr_bak;
    return ret;
}

uint8 sKY_GetEarState(void)
{
    return 1;
    /*
	uint8 sfr_bak;
	uint8 ear_state;
	uint16 i;

    ear_state = 0;
	sfr_bak = SFR_BANK;
	SFR_BANK = BANK_GPIO; //关闭高频限幅功能
	if((GPIOCDAT & 0x01)==0)
	{
		
		for(i=0;i<5000;i++)
		{
		   ;
		}
		if((GPIOCDAT & 0x01)==0)
		{			
			ear_state = 1;
		}
	}

	SFR_BANK = sfr_bak;
	return ear_state;	
	*/
}


//20110524
//para		0:停止	1:工作	2.清零  3:數據已處理    4:清零gap值    5:ReadGSensor    6:寫step    7:寫second
void sKY_SetGsensor(uint8 para, uint32 step, uint32 second, uint8 mode)
{
	uint8 sfr_bak;
	sfr_bak = SFR_BANK;

	if(para==0)				//停止
	{
	    sKY_GsensorOff();
	    if(mode)
	        gsensor_workflag_swim = 0;
	    else
		    gsensor_workflag = 0;	//扫描加速度传感
	}
    else if(para==1)			//计数
	{
		sKY_GsensorOn();
	    if(mode)
	        gsensor_workflag_swim = 1;
	    else
		    gsensor_workflag = 1;	//扫描加速度传感
		gsensor_period = 0;
	}
	else if(para==2)					//清零
	{
	    if(mode)
        {
            step_counter_swim = 0;
    		step_second_swim = 0;
        }
	    else
	    {
    		step_counter = 0;
    		step_second = 0;
		}
	}
	else if(para==3)
	{
	    gsensor_got = 0;
	}
	else if(para==4)
	{
	    gsensor_gap = 0;
	}
	else if(para==5)
	{
	    sKY_ReadGSensor();
	}    
	else if(para==6)
	{
	    if(mode)
        {
            step_counter_swim = step;
        }
        else
        {
	        step_counter = step;
	    }
	}    
	else if(para==7)
	{
	    if(mode)
        {
            step_second_swim = second;
        }
        else
        {
	        step_second = second;
	    }
	}    
}

void sKY_BankGsensorGetStep(uint32 *pStep, uint32 *pSecond, uint8 mode)
{
    if(mode)
    {
        *pStep = step_counter_swim;
	    *pSecond = step_second_swim;
    }
    else
    {
	    *pStep = step_counter;
	    *pSecond = step_second;
	}
}

//para		1:xadc  2:yadc  3:zadc  4:數據待處理    5:返回gap值 6:工作標誌
uint16 sKY_BankGsensorGetADC(uint8 para, uint8 mode)
{
	if(para==1)
		return gsensor_xadc1;
	else if(para==2)
		return gsensor_yadc1;
	else if(para==3)
		return gsensor_zadc1;
	else if(para==4)
	{
	    if(mode)
            return (gsensor_workflag_swim && gsensor_got);
        else
            return (gsensor_workflag && gsensor_got);
    }
    else if(para==5)
        return gsensor_gap;
    else if(para==6)
    {
        if(mode)
            return gsensor_workflag_swim;
        else
            return gsensor_workflag;
    }
    else
        return 0;
}


