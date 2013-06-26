/*
 *******************************************************************************
 *                ACTOS AP
 *           ap common lib file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ui_err_msg.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"
//#include "common.h"
#pragma name(COM_ERR_MSG)

/*
 ********************************************************************************
 * Description : 系统警告信息
 *
 * Arguments   :
 *    msgID,      错误信息ID
 *    string_id,  语言

 * Returns     :
 *        0
 * Notes       :
 *    任意键退出,没有按键等待5秒之后退出.
 ********************************************************************************
 */
uint16 ui_err_msg(uint16 msgID)
{	

    ClearScreen(NULL);
	SetAsciiFont(FONT_TYPE_LARGE);
    ResShowMultiString(msgID, 255,16);
    UpdateScreen(NULL);
    ap_sleep(4);   
    return 0;
 
}

/*
 ********************************************************************************
 * Description : 系统警告信息
 *
 * Arguments   :
 *    msgID,      错误信息ID
 *    string_id,  语言

 * Returns     :
 *        0
 * Notes       :
 *    立即退回
 ********************************************************************************
 */
uint16 ui_show_msg(uint16 msgID) //无DELAY
{
  
    uint16 i; //czh
    ClearScreen(NULL);
    ResShowMultiString(msgID, 255, 16);
    UpdateScreen(NULL);
    for (i = 0; i < 6000; i++)
    {   
    }
	
    return 0;

}
