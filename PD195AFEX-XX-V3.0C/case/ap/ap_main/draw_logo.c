/*
*******************************************************************************
*                ACTOS AP
*        main ap of sdk 3.0
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: draw_logo.c,v 1.1.1.1 2006/05/22 03:24:01 selina Exp $
*******************************************************************************
*/
#include "actos.h"
#include "ap_common.h"
#pragma name(AP_LOGO)
/*
********************************************************************************
* Description : 显示系统开机动画
*
* Arguments   : wait_key_up, 是否要等待按键
*
* Returns     : TRUE, 正常
        FALSE, 资源文件格式错误
*
* Notes       :
*
********************************************************************************
*/
void time_delay(uint8 time)
{
    uint16 i;
    uint32 timer;
    for (i = 0; i < 10 * time; i++)
    {
        timer = 8000;
        ClearWatchDog();
        while (timer > 0)
        {
            --timer;
        }
    }

}
void draw_logo(BOOL wait_key_up)
{
    uint8 i;
    uint8 key;
    uint16 result;
    uint16 timer;
    BOOL need_wait_keyup = wait_key_up;
    ResClose(res_fp);
    	res_fp = ResOpen("LOGO.res");
    for(i=1; i <= 5; i++)
    {
        result = ResShowPic(i,0,0);
        if(result == 0)
        {
            break;
        }
#ifdef PC
        timer = 20;
#else   
        #if(PETOMETER_USE)
        if(g_comval.PedoFlag)
            timer = 0x1000;
        #else
            timer = 0xffff;
        #endif
//				time_delay(1);
#endif
       // while(--timer > 0)
        timer --;
        while(timer > 0)
        {
            key = ap_get_message();
            if( ((key & AP_KEY_UP) != 0) && need_wait_keyup != FALSE  )
            {
                need_wait_keyup = FALSE;
                if((key == (AP_KEY_MODE | AP_KEY_UP))) 
                {
                    break;
                }
            }            
            SIM_Sleep(500);
            timer--;    
        }
        if((timer != 0)) 
        {
            break;
        }
    }
    ResClose(res_fp);
    res_fp = ResOpen("ui30.res");
    if(need_wait_keyup!=0)
    {
        while((ap_get_message() & AP_KEY_UP) == 0) 
        {
            ;//qac comment
        }
    }
}


