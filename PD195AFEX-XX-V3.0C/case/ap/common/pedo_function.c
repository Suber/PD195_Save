/********************************************************************************************
 Airoha Technology FM module driver(I2C DRIVER)  for ATJ2097
 last update 2007/11/02
 Version:V0.5
 ********************************************************************************************/
#include "actos.h" 
#include <ap_cfg.h>
#include <ap_common.h>

#define SAMPLE_TIME 0x000001	//G-sensor
#define HPF_RC      0x00000F
#define LPF_RC      0x000001

#define SENS_WALK   0x000800		// Memsic Referece:  0.1G
#define SENS_PEDO_UP	100
#define SENS_PEDO_DOWN	100     //0.1G

#define SENS_JOGG	0x000A00		// Memsic Referece:  1G
#define SENS_RUNN	0x000C00		// Memsic Referece:  2G
#define SENS_JMAX	0x000000		//
#define SENS_RMAX	0x000000		//
#define SENS_WMAX	0x000000		//

#define STRI_WALK	0x000050	
#define STRI_JOGG	0x000080	
#define STRI_RUNN	0x0000A0

#pragma name(PEDO_FUNC)

//uint8 gfGSStart=0;     //计步开始、暂停标志			
int32 stc_iAcce1_LPFcount1=40832;
int32 stc_iAcce0_LPFcount1=40832;
int32 stc_iAcce1_HPFcount1=32866;
int32 stc_iAcce0_HPFcount1=32866;
int32 stc_iAcceFilter1=2048;          //配置滤波涉及参数初始值
int32 stc_iG_CountUP=0x800;
int32 stc_iG_CountDOWN=0x800;
uint8 stc_bStatUp=0; 
uint8 stc_bStatDown=0;         //计步参数初始化
uint16 step_gap;

int32 AccLPF(int32 iX0, int32 iY1)
{
	iY1 = iY1 * LPF_RC;					//RC = 0x000003;
	iY1+=((iX0*SAMPLE_TIME)+(LPF_RC+SAMPLE_TIME)/2);
	iY1/=(LPF_RC+SAMPLE_TIME);
	return iY1;
}

int32 AccHPF(int32 iX0, int32 iX1, int32 iY1)
{
	iY1+=(iX0-iX1);
	iY1*=HPF_RC;
	iY1+=0x008000+(HPF_RC+SAMPLE_TIME)/2;					//RC = 0x000003;
	iY1/=(HPF_RC+SAMPLE_TIME);							//	0x000004;		T+RC   T=Timer*100,  
	return iY1;
}

uint32 sqrt_16(uint32 M)  
{  
    uint16 N, i;  
    uint32 tmp, ttp;   // 结果、循环计数  
    if (M == 0)               // 被开方数，开方结果也为0  
        return 0;  

    N = 0;  

    tmp = (M >> 30);          // 获取最高位：B[m-1]  
    M <<= 2;  
    if (tmp > 1)              // 最高位为1  
    {  
        N ++;                 // 结果当前位为1，否则为默认的0  
        tmp -= N;  
    }  

    for (i=15; i>0; i--)      // 求剩余的15位  
    {  
        N <<= 1;              // 左移一位  

        tmp <<= 2;  
        tmp += (M >> 30);     // 假设  

        ttp = N;  
        ttp = (ttp<<1)+1;  

        M <<= 2;  
        if (tmp >= ttp)       // 假设成立  
        {  
            tmp -= ttp;  
            N ++;  
        }  

    }  

    return N;  
}


void   StepLoops(void)
{
    uint16 gap,temp;
    int32 iAcce_Sample;
    #if (PETOMETER_METHOD == 2)
    int32 iG_CountPEAK,iAcce1_LPFcount0,iAcce0_LPFcount0,iAcce1_HPFcount0,iAcce0_HPFcount0,iAcceFilter0;  
    #endif
	//=========calculate=======masda20110111=====================
    int32 XDataTemp,YDataTemp,ZDataTemp;
    int32 GDataTemp;
	//=============================end===========================
    //GS_XYRead();           //读G-sensor  X、Y轴的值
    sKY_SetGsensor(5,0,0,Pedo_Swim_flag);
    XDataTemp = sKY_BankGsensorGetADC(1,Pedo_Swim_flag);
    YDataTemp = sKY_BankGsensorGetADC(2,Pedo_Swim_flag);
    ZDataTemp = sKY_BankGsensorGetADC(3,Pedo_Swim_flag);
    sKY_SetGsensor(3,0,0,Pedo_Swim_flag);
    //==============calculate======masda20110111=======================================
    XDataTemp-=2048;         //2048为轴上1倍重力的值    //求两轴合力的值(无符号正值)
    YDataTemp-=2048;
    ZDataTemp-=2048;
    GDataTemp=(uint32)((XDataTemp*XDataTemp)+(YDataTemp*YDataTemp)+(ZDataTemp*ZDataTemp));
    iAcce_Sample=(uint32)(sqrt_16(GDataTemp));	   //sqrt_16为开平方运算
    //iAcce_Sample = YDataTemp;
    //Pedo_total_step = iAcce_Sample;
    //if((XDataTemp+YDataTemp+ZDataTemp) < 0) iAcce_Sample = -iAcce_Sample; 
    //=================================end=============================================
    
  #if (PETOMETER_METHOD == 1)
    //由于2048是零点，所以实际得到的值要-2048，
	//合ADC=sqrt((xadc-2048)*(xadc-2048)+(yadc-2048)*(yadc-2048))	/*sqrt为开平方根*/
	//由于再开平方根需要增加代码，现不做开平方根，直接用(合ADC*合ADC)作比较

	/* 一些测试数据:
	静止	Xadc	Yadc	X*X+Y*Y	开方
	
	方向1	2511	2027	214810	463.4759972
	方向2	2361	1703	216994	465.8261478
	方向3	2037	1565	233410	483.1252426
	方向4	1715	1690	239053	488.9304654
	方向5	1591	2003	210874	459.2101915
	方向6	1765	2416	215513	464.2337773
	方向7	2088	2512	216896	465.7209465
	方向8	2361	2401	222578	471.7817292
	说明平时静止状态的(合ADC)为460左右，那(合ADC*合ADC)为211600，
	现选(合ADC)比较值为550，那(合ADC*合ADC)为302500，

	
	模拟测试4组数据:
	运动	Xadc	Yadc	X*X+Y*Y	开方
	
	动作1 	2292	1170	830420	911.2738337
	动作2	1619	527		2497482	1580.342368
	动作3	1756	3000	991568	995.775075
	动作4	1625	2844	812545	901.41278
	说明平时运动状态的(合ADC)为大于900左右，
	现选(合ADC)比较值为700，那(合ADC*合ADC)为490000，


	1. Length满足一定阀值，比如 –Threshold < Length < Threshold，0.1g
　　2. 两个连续踩点的时间间隔必须在Timespan （200~2000毫秒）之内

	*/

    #define g_sensor_max        ((uint32)(1024+100))   // 1.1g
    #define g_sensor_min        ((uint32)(1024-100))   // 0.9g
    #define g_senser_debounce   1

    if(gsensor_posflag==0)			        //之前状态为大值
	{
		if	(iAcce_Sample < g_sensor_min)		//判断是否到小值	
		{
			g_debounce++;
			if(g_debounce>=g_senser_debounce)
			{
				gsensor_posflag=1;		    //是的话就将之前状态设为小值，起點
				g_debounce=0;
			}	
		}		
		else
		{
			g_debounce=0;
		}
	}
	else							        //之前状态为小值
	{
		if	(iAcce_Sample > g_sensor_max)		//判断是否到大值
		{
			g_debounce++;
			if(g_debounce>=g_senser_debounce)
			{
				gsensor_posflag=0;		    //是的话就将之前状态设为大值，踩點
				g_debounce=0;

				gap = sKY_BankGsensorGetADC(5,Pedo_Swim_flag);
                temp = gap - step_gap;
                if ((temp >= 6) && (temp <= 67))      //200ms~2000ms
                {
                    if(Pedo_Swim_flag)
                        Swim_total_step++; //步数加1   
                    else
                        Pedo_total_step++; //步数加1   
                    sKY_SetGsensor(4,0,0,Pedo_Swim_flag);
                    if(Pedo_Swim_flag)
                        sKY_SetGsensor(6,Swim_total_step,0,Pedo_Swim_flag);
                    else
                        sKY_SetGsensor(6,Pedo_total_step,0,Pedo_Swim_flag);
                    //sKY_SetGsensor(7,0,Pedo_total_sec);
                    gap = 0;
                }
                step_gap = gap;
			}	
		}	
		else
		{
			g_debounce=0;
		}
	}

  #else
  
    iAcce_Sample+=2048; 
	iAcce_Sample*=0x10;
    iAcce1_LPFcount0=AccLPF(iAcce_Sample,stc_iAcce1_LPFcount1);
    iAcce0_LPFcount0=AccLPF(iAcce1_LPFcount0,stc_iAcce0_LPFcount1);   
    iAcce1_HPFcount0=AccHPF(iAcce0_LPFcount0,stc_iAcce0_LPFcount1,stc_iAcce1_HPFcount1);
    iAcce0_HPFcount0=AccHPF(iAcce1_HPFcount0,stc_iAcce1_HPFcount1,stc_iAcce0_HPFcount1);   
    iAcceFilter0=iAcce0_HPFcount0/0x10;         //滤波处理，输出iAcceFilter0与iAcce_Sample大小无关，iAcce_Sample稳态是iAcceFilter0为2048，iAcce_Sample增大时iAcceFilter0增大，反之减小
		                                        //即iAcceFilter0反应的是iAcce_Sample的变化

    if ((iAcceFilter0>0x000800) && (iAcceFilter0>stc_iG_CountUP))
		stc_iG_CountUP=iAcceFilter0;
    if ((iAcceFilter0<0x000800) && (iAcceFilter0<stc_iG_CountDOWN))
		stc_iG_CountDOWN=iAcceFilter0;
    if ((iAcceFilter0>=0x000800) && (stc_iAcceFilter1<0x000800))
    {
        stc_iG_CountDOWN = 0x000800 - stc_iG_CountDOWN;
        iG_CountPEAK = stc_iG_CountDOWN;
        stc_iG_CountDOWN = 0x000800;
        if(iG_CountPEAK > SENS_PEDO_UP)      //SENS_PEDO为计步灵敏度，大小可调
            stc_bStatUp = 1;
	   	else
	   	    stc_bStatUp = 0;
    }
    if ((iAcceFilter0<=0x000800) && (stc_iAcceFilter1>0x000800))
    {
        stc_iG_CountUP=stc_iG_CountUP-0x000800;
        iG_CountPEAK=stc_iG_CountUP;
        stc_iG_CountUP=0x000800;
        if (iG_CountPEAK > SENS_PEDO_DOWN)     
			stc_bStatDown=1;
		else
			stc_bStatDown=0;
    }
    
    //if ( stc_bStatUp )
    //{
    //  	test_up++;
    //}
    //if ( stc_bStatDown )
    //{
    //  	test_down++;
    //}

    if ( stc_bStatUp && stc_bStatDown )      //stc_bStatUp 和 stc_bStatDown分别代表行走一步的人体的上升和下降
    {
       stc_bStatUp = 0;
       stc_bStatDown = 0;
       gap = sKY_BankGsensorGetADC(5,Pedo_Swim_flag);
       temp = gap - step_gap;
       if ((temp >= 6) && (temp <= 67))      //200ms~2000ms
       {
            if(Pedo_Swim_flag)
                Swim_total_step++; //步数加1   
            else
                Pedo_total_step++; //步数加1   
            sKY_SetGsensor(4,0,0,Pedo_Swim_flag);
            if(Pedo_Swim_flag)
                sKY_SetGsensor(6,Swim_total_step,0,Pedo_Swim_flag);
            else
                sKY_SetGsensor(6,Pedo_total_step,0,Pedo_Swim_flag);
            //sKY_SetGsensor(7,0,Pedo_total_sec);
            gap = 0;
       }
       step_gap = gap;
    }
    
    stc_iAcce0_LPFcount1=iAcce0_LPFcount0;
    stc_iAcce1_LPFcount1=iAcce1_LPFcount0;
    stc_iAcce0_HPFcount1=iAcce0_HPFcount0;
    stc_iAcce1_HPFcount1=iAcce1_HPFcount0;
    stc_iAcceFilter1=iAcceFilter0;			       //存储滤波涉及参数值

  #endif
}


void Count_Step(uint8 mode)
{
    uint16 workflag;
    workflag = sKY_BankGsensorGetADC(4,Pedo_Swim_flag);
	if(mode && workflag)
		StepLoops();
}

