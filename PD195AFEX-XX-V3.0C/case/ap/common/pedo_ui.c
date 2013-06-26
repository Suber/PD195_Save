/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: display module of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_common.h"
#pragma name(PEDO_UI)

const uint16 STEP_NUM[12]=
{IM_RADIO_NUMBER_0,IM_RADIO_NUMBER_1,IM_RADIO_NUMBER_2,IM_RADIO_NUMBER_3,IM_RADIO_NUMBER_4,
 IM_RADIO_NUMBER_5,IM_RADIO_NUMBER_6,IM_RADIO_NUMBER_7,IM_RADIO_NUMBER_8,IM_RADIO_NUMBER_9,	IM_RADIO_NUMBG, IM_RADIO_REGION_BK};


//============= 
void itoa_5(uint32 i, uint8 *buf)
{
    buf[0] = ((uint32)i % 100000) / 10000+'0' ;
    buf[1] = (i % 10000) / 1000+'0' ;
    buf[2] = (i % 1000) / 100+'0' ;
    buf[3] = (i % 100)/10+'0';
    buf[4] = (i % 10)+'0';
}

//============= 
void ui_show_PedoTime(uint8 *string)
{
    uint8 i,j=0; 
    for(i=0;i<8;i++)
  	{
  	    ResShowPic(STEP_NUM[*(string+i)-0x30],18+j,0);
  	    if((i==2)||(i==5))  j+=6;
  	    else    j+=9;
  	} 	
}

//=============
void  ui_show_PedoData(uint8 *string, uint8 part)
{ 
    uint8 i;
    for(i=0;i<5;i++)
  	{
  	    if (part==0)        //step
       		ResShowPic(STEP_NUM[*(string+i)-0x30],18+9*i,16);
		else if (part==1)   //meter
			ResShowPic(STEP_NUM[*(string+i)-0x30],18+9*i,32);
		else //if (part==3) //calorie
			ResShowPic(STEP_NUM[*(string+i)-0x30],18+9*i,48);
  	} 	
}

//============= 
void ui_show_PedoNum(uint8 *string)
{
    uint8 i,j=0; 
    for(i=0;i<5;i++)
  	{
  	    ResShowPic(STEP_NUM[*(string+i)-0x30],86+j,16);
  	    if(i==2)  j+=6;
  	    else    j+=9;
  	} 	
}


//=============
void Pedo_data_draw(uint32 step, uint8 armlen, uint16 weight, uint8 poollen, uint8 unit)
{
    uint8 i;
	uint32 meter = 0;
	uint32 laps = 0;
    uint8 buf[5];
	uint32 g_wCalorie,g_weight=0;

	itoa_5(step, &buf[0]);  //step
    ui_show_PedoData(buf,0);

	if(!unit)                                   //路程单位，公制英制判断
	{
		meter=step*armlen/100;	                //CM->Meter       
		laps = meter;
	//	temp_dis=g_wDistance;					//Meter
		g_weight=weight;					    //KG
	}
	else
	{
		meter=step*armlen/36;	                //Inch->Yard    
		laps = meter;
		//temp_dis=g_wDistance*914/1000;		//Meter
		g_weight=g_comval.Weight*454/1000;		//LB->KG
	}
	
#if (1)	

	itoa_5(meter, &buf[0]);
    ui_show_PedoData(buf,1); 

	if(unit)
	    meter=meter*914/1000;;	                //Yard->M
	    
	g_wCalorie=((187*weight*meter)/200)/1000;
	itoa_5(g_wCalorie, &buf[0]);//macro qac
	ui_show_PedoData(buf,2); 

	if(poollen)
	{
        laps = laps/poollen;
        itoa_2(laps, &buf[0]);
        for(i=0;i<2;i++)
      	{
      	    ResShowPic(STEP_NUM[*(buf+i)-0x30],110+9*i,32);
      	} 	
      	ResShowPic(ICON_PEDO_LAPS, 92, 32);
	}
	
#else
    itoa_5(test_up, &buf[0]);//macro qac
    ui_show_ef_step(buf,0); 
    itoa_5(test_down, &buf1[0]);//macro qac
	ui_show_ef_step(buf1,1); 
    /*
    //test
	meter = sKY_BankGsensorGetADC(1,Pedo_Swim_flag);
    itoa_5(meter, &buf[0]);//macro qac
    ui_show_ef_step(buf,0); 
	
	g_wCalorie = sKY_BankGsensorGetADC(2,Pedo_Swim_flag);
	itoa_5(g_wCalorie, &buf1[0]);//macro qac
	ui_show_ef_step(buf1,1); 

	g_wCalorie = sKY_BankGsensorGetADC(3,Pedo_Swim_flag);
	itoa_5(g_wCalorie, &buf1[0]);//macro qac
	ui_show_ef_step(buf1,2); 
	*/
#endif	
}

//=============
void Pedo_time_draw(uint32 sec)
{
    time_t timenow;
    uint8  g_now_time[8]; //00:00:00

	timenow.hour = sec / 3600;
	sec %= 3600;
	timenow.minute = (uint8) (sec / 60);
	timenow.second = (uint8) (sec % 60);

    itoa2(timenow.hour, &g_now_time[0]);//macro qac
    g_now_time[2]=':';
    itoa2(timenow.minute, &g_now_time[3]);//macro qac
    g_now_time[5]=':';
    itoa2(timenow.second, &g_now_time[6]);//macro qac
    ui_show_PedoTime(g_now_time); 

    if((Pedo_work_flag == TRUE)||(Swim_work_flag == TRUE))
        ResShowPic(IM_MUSIC_STATUS_PLAY,0,0);
    else
        ResShowPic(IM_MUSIC_STATUS_PAUSE,0,0);

	//if(Pedo_work_flag == TRUE)
	//{
	//	ResShowPic(ICON_PEDO_RUN_01 + Pedo_ui_num, 95, 40);	
	//}

}

//=============
void Pedo_RefreshMainScreen(uint8 mode)
{
    ui_auto_update = FALSE;
    
    ClearScreen(NULL);
    
    ui_show_lock_1();
    
    ui_show_batt(GetBattery());

    if(mode)
        ResShowPic(ICON_PEDO_SWIM, 0, 16);
    else
	    ResShowPic(ICON_PEDO_PEDO, 0, 16);
	ResShowPic(ICON_PEDO_DIS, 0, 32);
	ResShowPic(ICON_PEDO_CAL, 0, 48);
    
	SetAsciiFont(FONT_TYPE_LARGE);
	SetTextPos(65, 32);
	if(!g_comval.PedoUnit)
	    PutS("m", -1);
	else
	    PutS("yd", -1);
	SetTextPos(65, 48);
	PutS("kcal", -1);
	
    Pedo_RefreshDataScreen(mode);
    
	UpdateScreen(NULL);
    ui_auto_update = TRUE;
    return;
}

//=============
void Pedo_RefreshDataScreen(uint8 mode)
{
    uint32 sec,step;
    uint8 armlen,poollen,unit;
    uint16 weight;

    ui_auto_update = FALSE;
    
    if(mode)
    {
        sec = Swim_total_sec;
        step = Swim_total_step;
        armlen = g_comval.Height/2;
        poollen = g_comval.PoolLen;
    }
    else
    {
        sec = Pedo_total_sec;
        step = Pedo_total_step;
        if(g_comval.Gender)
        {
            armlen = (g_comval.Height*83)/200;
        }
        else
        {
            armlen = (g_comval.Height*413)/1000;
        }
        poollen = 0;
    }
    weight = g_comval.Weight;
    unit = g_comval.PedoUnit;
    Pedo_time_draw(sec);
    Pedo_data_draw(step,armlen,weight,poollen,unit); 

    UpdateScreen(NULL);
    ui_auto_update = TRUE;
}


