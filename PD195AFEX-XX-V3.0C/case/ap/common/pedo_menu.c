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
#pragma name(PEDO_MENU)

#define PEDO_RECORD_MAX     8
#define PEDO_RECORD_MAGIC   0x1721
#define DISK_C 0
#define DISK_H 1

static uint16 result = 0;
//uint8 Pedo_Record_buff[PEDO_RECORD_MAX*sizeof(Pedo_Record_t)];

const uint16 Menu_Pedo[]={STRSAVE,CDELALL,RECORDLIST,SMAIN10}; 

extern void ui_show_PedoTime(uint8 *string);
extern void ui_show_PedoData(uint8 *string, uint8 part);
extern void ui_show_PedoNum(uint8 *string);
extern void Pedo_data_draw(uint32 step, uint8 armlen, uint16 weight, uint8 poollen, uint8 unit);
extern void Pedo_time_draw(uint32 sec);


//=============
void Pedo_record_num(uint8 Cnum,uint8 Tnum)
{
    uint8  g_num[5]; //00/00

	itoa2(Cnum, &g_num[0]);
    g_num[2]=';';
    itoa2(Tnum, &g_num[3]);
    ui_show_PedoNum(g_num); 
}

//=============
uint16 Pedo_record_draw(uint8 mode)
{
    uint8 key,i;
    uint8 Tnum,Cnum;
    uint16 addr;
    bool need_draw = TRUE;
    bool need_draw_all = TRUE;
    uint8 Pedo_Record_buff[PEDO_RECORD_MAX*sizeof(Pedo_Record_t)];
    
    if(mode)    addr = VM_AP_SWIM;
    else    addr = VM_AP_PEDO;
    VMRead(Pedo_Record_buff, addr, PEDO_RECORD_MAX*sizeof(Pedo_Record_t));
    for(i=0;i<PEDO_RECORD_MAX;i++)
    {
        memcpy(&Pedo_record, &Pedo_Record_buff[i*sizeof(Pedo_Record_t)], sizeof(Pedo_Record_t));
        if(Pedo_record.magic != PEDO_RECORD_MAGIC)  break;
    }
    if(i==0)    return 0;
    
    Tnum = i;
    Cnum = 0;

    while (1)
    {
        //show ui
        if (need_draw_all)
        {
            ui_auto_update = FALSE;
            ClearScreen(NULL);
            if(mode)
                ResShowPic(ICON_PEDO_SWIM, 0, 16);
            else
        	    ResShowPic(ICON_PEDO_PEDO, 0, 16);
	        ResShowPic(ICON_PEDO_DIS, 0, 32);
	        ResShowPic(ICON_PEDO_CAL, 0, 48);

            need_draw_all = FALSE;
        }    
        if (need_draw)
        {
            ui_auto_update = FALSE;
           
            ui_show_lock_1();
            ui_show_batt(GetBattery());
        	memcpy(&Pedo_record, &Pedo_Record_buff[Cnum*sizeof(Pedo_Record_t)], sizeof(Pedo_Record_t));
            Pedo_time_draw(Pedo_record.TotalSec);
            if(mode)
                Pedo_data_draw(Pedo_record.TotalStep,Pedo_record.ArmLen,Pedo_record.Weight,Pedo_record.PoolLen,Pedo_record.PedoUnit); 
            else
                Pedo_data_draw(Pedo_record.TotalStep,Pedo_record.Stride,Pedo_record.Weight,0,Pedo_record.PedoUnit);
            Pedo_record_num(Cnum+1,Tnum);
            
        	SetAsciiFont(FONT_TYPE_LARGE);
        	SetTextPos(65, 32);
        	if(!Pedo_record.PedoUnit)
        	    PutS("m", -1);
        	else
        	    PutS("yd", -1);
        	SetTextPos(65, 48);
        	PutS("kcal", -1);

            UpdateScreen(NULL);
            ui_auto_update = TRUE;
            
            need_draw = FALSE;
        }
        //handle msg
        key = ap_get_message();
        switch (key)
        {
            case AP_KEY_MODE | AP_KEY_UP:
            case AP_KEY_PLAY | AP_KEY_UP:
            #if(MACHINE_MODEL != 2)
            case AP_KEY_NEXT | AP_KEY_UP:
            case AP_KEY_PREV | AP_KEY_UP:
            #endif
            case AP_MSG_WAIT_TIMEOUT:
                return 0;
     		#if(MACHINE_MODEL == 1)
    		case AP_KEY_VOLADD | AP_KEY_UP:
    		#elif(MACHINE_MODEL == 2)
    		case AP_KEY_NEXT | AP_KEY_UP:
    		#endif
    		    Cnum ++;
    		    if(Cnum >= Tnum) Cnum = 0;
    		    need_draw = TRUE;
    		    break;
    		#if(MACHINE_MODEL == 1)    		
    		case AP_KEY_VOLSUB | AP_KEY_UP:
    		#elif(MACHINE_MODEL == 2)
    		case AP_KEY_PREV | AP_KEY_UP:
    		#endif
    		    if(Cnum > 0) Cnum --;
    		    else Cnum = Tnum-1;;
    		    need_draw = TRUE;
    		    break;

            default:
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                {
                    need_draw = TRUE;
                    need_draw_all = TRUE;
                }
                else if (key == RESULT_AUTO_BACK)
                {
                    return NULL;
                }
                else if (key != 0)
                {
        	       return key;
                }
                else
                {
                }
                break;
        }
    }
    return NULL;
}

//=============
uint16 Pedo_record_save(uint8 mode)
{
    uint8 i;
    uint8 Cnum;
    uint16 addr;
    date_t date;
    time_t time;
    uint8 Pedo_Record_buff[PEDO_RECORD_MAX*sizeof(Pedo_Record_t)];

    if(!DRV_ActiveUD(DISK_C, MODE_SLOW))
    {
        return 0;
    }

     
    if (!DRV_ActiveDrive('C', MODE_SLOW))
    {
        return 0;
    }
    
    
    TM_GetDate(&date, 0);
    TM_GetTime(&time);	


    if(mode)    
    {   
        if(Swim_total_step == 0)    return 0;   
        addr = VM_AP_SWIM;
    }
    else  
    {
        if(Pedo_total_step == 0)    return 0;   
        addr = VM_AP_PEDO;
    }
    VMRead(Pedo_Record_buff, addr, PEDO_RECORD_MAX*sizeof(Pedo_Record_t));
    for(i=0;i<PEDO_RECORD_MAX;i++)
    {
        memcpy(&Pedo_record, &Pedo_Record_buff[i*sizeof(Pedo_Record_t)], sizeof(Pedo_Record_t));
        if(Pedo_record.magic != PEDO_RECORD_MAGIC)  break;
    }
    if(i==PEDO_RECORD_MAX)    
    {
        memmove(Pedo_Record_buff, &Pedo_Record_buff[sizeof(Pedo_Record_t)], (PEDO_RECORD_MAX-1)*sizeof(Pedo_Record_t));
        Cnum = PEDO_RECORD_MAX-1;
    }
    else Cnum = i;

    Pedo_record.magic = PEDO_RECORD_MAGIC;
    
    Pedo_record.Date.year = date.year;
    Pedo_record.Date.month = date.month;
    Pedo_record.Date.day = date.day;
    Pedo_record.Time.hour = time.hour;
    Pedo_record.Time.minute = time.minute;
    Pedo_record.Time.second = time.second;

    if(g_comval.Gender)
    {
        Pedo_record.Stride = (g_comval.Height*83)/200;
    }
    else
    {
        Pedo_record.Stride = (g_comval.Height*413)/1000;
    }
    Pedo_record.PedoUnit = g_comval.PedoUnit;
    Pedo_record.Weight = g_comval.Weight;
    
    Pedo_record.ArmLen = g_comval.Height/2;
    
    Pedo_record.PoolLen = g_comval.PoolLen;
    if(mode)
    {
        Pedo_record.TotalSec = Swim_total_sec;
        Pedo_record.TotalStep = Swim_total_step;
    }
    else
    {
        Pedo_record.TotalSec = Pedo_total_sec;
        Pedo_record.TotalStep = Pedo_total_step;
    }
    memcpy(&Pedo_Record_buff[Cnum*sizeof(Pedo_Record_t)], &Pedo_record, sizeof(Pedo_Record_t));
    VMWrite(Pedo_Record_buff, addr);
    return NULL;
}


//=============
uint16 Pedo_record_del(uint8 mode)
{
    uint16 addr;
    uint8 Pedo_Record_buff[PEDO_RECORD_MAX*sizeof(Pedo_Record_t)];
    
    if(mode)    
    {   
        addr = VM_AP_SWIM;
    }
    else  
    {
        addr = VM_AP_PEDO;
    }
    memset(Pedo_Record_buff, 0, PEDO_RECORD_MAX*sizeof(Pedo_Record_t));
    VMWrite(Pedo_Record_buff, addr);
    return NULL;
}

//=============
uint16 Pedo_menu(uint8 mode)
{       
        function_menu_t menu;
		//uint16 result = 0;
		uint16 active = 0;

		while(1)
	 	{
		     menu.total = sizeof(Menu_Pedo)/2;        
		     menu.menuhead = STDISP;//ÏÔÊ¾ÉèÖÃ
		     menu.string = Menu_Pedo;
		     menu.active = active;
		     menu.string_id = g_comval.langid;
		     //ui_auto_select = TRUE;  
		     result = ui_function_menu(&menu, NULL);
		     //ui_auto_select = FALSE;
			 active = menu.active;
			 if(result!=NULL)
			 {
			   	return  result;
			 }
			 else
		 	 {
                switch (active)
                {
			        case 0: //save
			            result = Pedo_record_save(mode);
			            if(result!=NULL)
			            {
			   	            return  result;
			            }
					  	return  RESULT_IGNORE;
			             
					case 1: //delet
					    result = Pedo_record_del(mode);
			            if(result!=NULL)
			            {
			   	            return  result;
			            }
					  	return  RESULT_IGNORE;


					case 2: //list
					    result = Pedo_record_draw(mode);
			            if(result!=NULL)
			            {
			   	            return  result;
			            }
					  	return  RESULT_IGNORE;
					  	
                    case 3: //exit
					    return  RESULT_RETURN_TO_MAIN;
					  	
					default:
					    break;
                }
		 	 }
 	 	}
        return  0;
}
 
