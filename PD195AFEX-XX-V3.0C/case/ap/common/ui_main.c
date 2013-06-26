/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib , main ui control
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(COM_UI_MAIN)

const uint16 SONG_NUM[11]=
{SYSFONT_SMALL_0,SYSFONT_SMALL_1,SYSFONT_SMALL_2,SYSFONT_SMALL_3,SYSFONT_SMALL_4,
 SYSFONT_SMALL_5,SYSFONT_SMALL_6,SYSFONT_SMALL_7,SYSFONT_SMALL_8,SYSFONT_SMALL_9,SYSFONT_SMALL_SLASH};
/*const uint16 TIME_NUM[11]=
{IM_MUSIC_MP3_NUMBER_0,IM_MUSIC_MP3_NUMBER_1,IM_MUSIC_MP3_NUMBER_2,IM_MUSIC_MP3_NUMBER_3,IM_MUSIC_MP3_NUMBER_4,
 IM_MUSIC_MP3_NUMBER_5,IM_MUSIC_MP3_NUMBER_6,IM_MUSIC_MP3_NUMBER_7,IM_MUSIC_MP3_NUMBER_8,IM_MUSIC_MP3_NUMBER_9,ICON_MY_COLON};*/
//const region_t region_c  =  {POSITION_C, 0, 8, 8};
//const region_t region_ef = {POSITION_F, 0, 15+29+20, 16};
//const uint16   batt_id[4]  =
//{ BATTLVL1, BATTLVL2, BATTLVL3, BATTLVL4, BATTLVL5};// BATTLVL6, BATTLVL7, BATTLVL8, BATTLVL9*/0 }; //对应1~9级电量的图标
//{IM_BATTERY_02,IM_BATTERY_03,IM_BATTERY_04,IM_BATTERY_05};
//void ui_show_com(const uint8 *string,const region_t *region1);
//只有这个界面使用,为了提高效率而放在这里
#if (0)
uint16 main_ResShowPic(uint16 id, uint8 x, uint8 y)
{
    res_infor_t res_infor;
    //背光关着时，不刷新图片
    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset + (uint32) id * RESHEADITEM);
    SD_FRead(res_fp, &res_infor, sizeof(res_infor_t));
#ifdef PC
    SIM_ChgLongPC(&res_infor.dwOffset);
    SIM_ChgLongPC(&res_infor.dwLength);
    SIM_ChgWordPC(&res_infor.wWidth);
    SIM_ChgWordPC(&res_infor.wHeight);
#endif
    /*判断res类型的合法性*/
    if (res_infor.bType != 0x02)
    {
        return 0;
    }
    /*设窗参数设置*/
    res_region.x = x;
    res_region.y = y;
    res_region.width = (uint8) res_infor.wWidth;
    res_region.height = (uint8) res_infor.wHeight;
    /*读取图片数据并刷屏*/
    PutSDImage(&res_region, res_fp->startaddr + res_infor.dwOffset);
    UpdateScreen(&res_region);
    return 1;
}
#endif

void ui_show_a(uint16 id)
{
    ResShowPic(id, POSITION_A, 48);
}

void ui_show_type(uint16 id)
{
    ResShowPic(id, 0, 8);
}
//id =0, clear the icon
void ui_show_c(uint16 id)
{
     ResShowPic(id, 16, 0);
}
 
void ui_show_d(const uint8 *string)
{
    uint8 i;
    for(i=0;i<9;i++)
    {
      ResShowPic(SONG_NUM[*(string+i)-0x30],24+i*6,1);
    }
	
}




void ui_show_d_pic(uint16 id/*, uint8 type*/)
{    
  
     ResShowPic(id, 71, 8); 
}

void ui_show_loop(uint16 id)
{
	 ResShowPic(id, 59, 8);
}


/*void ui_show_com(const uint8 *string,const region_t *region1)
{
           SetAsciiFont(FONT_TYPE_LARGE);
    PutS(string, -1);
    SetAsciiFont(FONT_TYPE_DEFAULT);
    if(ui_auto_update) 
        UpdateScreen(region1);
       
} */

/*void ui_show_recordTime(const uint8 *string)
{
    uint8 i; 
    for(i=0;i<8;i++)
  	{
       ResShowPic(TIME_NUM[*(string+i)-0x30],7*i,8);
  	} 	

}*/

//=============
/*void ui_show_recordbit(const uint8 *string)
{
   
    uint8 i; 
    for(i=0;i<3;i++)
  	{
       ResShowPic(SONG_NUM[*(string+i)-0x30],22+5*i,32);
  	} 
  	
}  */
//==============

/*void ui_show_recordName(const uint8 *string) //录音文件名显示
{
    SetAsciiFont(FONT_TYPE_LARGE);
    SetTextPos(0, 16);
    PutS(string, -1);
	SetAsciiFont(FONT_TYPE_DEFAULT);
}*/
void  ui_show_ef(const uint8 *string/*, uint8 part*/)
{ 
	uint8 id;    
	id=*string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22,16);    
	ResShowPic(ICON_MY_COLON,22+6,16);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+9,16);    
	string++;    id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+15,16);    
	ResShowPic(ICON_MY_COLON,22+21,16);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+24,16);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+30,16);
}

/********************************************************************************
 * Description : 根据获取到的电压值显示相应的电池图标
 *
 * Arguments  :
 *            value：当前获取到的电压值
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void ui_show_batt(uint8 value)
{
    //uint8 cur_value;
    #if(MACHINE_MODEL==0)
    if ((GetUsbCableStatus() != 0) &&( sKY_ChargeSet(0) == 0x0)) //当有接充电器或USB，而没有电池时，电池图标显示空
    #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
    if (check_5v() &&( sKY_ChargeSet(0) == 0x0)) //当有接充电器或USB，而没有电池时，电池图标显示空
    #endif
    {
//		ResShowPic(BATTERY1, POS_BATTERY, POSY_BATTERY);
//        ResShowPic(batt_id[0], POSITION_BATT, 0);
		ResShowPic(IM_USB_BATTERY_01, POSITION_BATT, 0);   
    }
    else
    {
        //分5档显示电池图标，对应关系如下：
        /*
         电池电压范围
         1.       空格电：<=3.5V  0.05V误差

         2.       一格电：3.5-3.6V  0.05V误差

         3.       两格电：3.6-3.65V   0.05V误差

         4.       三格电：3.65-3.8V    0.05V误差

         5         满格：  3 .8V以上    0.05V 误差

         */
        #if(MACHINE_MODEL==0)
        if ((GetUsbCableStatus() != 0) && ( sKY_ChargeSet(0) == 1))
        #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
        if (check_5v() && ( sKY_ChargeSet(0) == 1))
        #endif
        {
            if (cur_value < 4)
            {
                cur_value++;
            }
            else
            {
                cur_value = 0;
            }
        }
        #if(MACHINE_MODEL==0)
        else if ((GetUsbCableStatus() != 0) && ( sKY_ChargeSet(0) == 2))
        #elif((MACHINE_MODEL==1)||(MACHINE_MODEL==2))
        else if (check_5v() && ( sKY_ChargeSet(0) == 2))
        #endif
        {
             cur_value = 4;
        }
        else
        {
            for (cur_value = 0; cur_value < 5; cur_value++)
            {
                if (value <= batvalue[cur_value])
                {
                    break;
                }
            }
            if (cur_value >= 5)
            {
                cur_value = 4;
            }
            if(cur_value > last_value)
                cur_value = last_value;
            else
                last_value = cur_value;
        }
    	ResShowPic(IM_USB_BATTERY_01+cur_value, POSITION_BATT, 0);   
    }
}

void show_waiting(void)
{
    ClearScreen(NULL);//清屏
    SetAsciiFont(FONT_TYPE_LARGE);
    ResShowMultiString(WAITPASS, 255, 24);//显示正在初始化
    UpdateScreen(NULL);//清屏
    SetAsciiFont(FONT_TYPE_DEFAULT);
}

