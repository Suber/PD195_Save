/*
 *******************************************************************************
 *                ACTOS AP
 *           ap common lib file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ui_pop_up_list.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */
#include "ap_common.h"
#include "common.h"

#pragma name(COM_POPUP_MENU)
static const uint8 menu_pos_list[ROW_COUNT_DEF] =
{ 16};//, POSITION_MENU4, POSITION_MENU3, POSITION_MENU2, POSITION_MENU1 };

// 定义背景颜色
void show_active_menuItem(pop_up_list_t *list, uint16 active)
{ 
 #if 1
    uint8 tmp = (uint8) (active % ROW_COUNT_DEF);
    ResShowPic(ICON_MY_MENU_BOX,0,32);
	ResShowPic(ICON_MENU_BOX_RIGHT,40,8);
	ResShowPic(ICON_MENU_BOX_LEFT,0,8);
    ResShowMultiString_Scroll(list->strid[active],MENU_STR_X_DEF, menu_pos_list[tmp], MENU_SCROLL_WIDTH,1);
#endif
}
void show_active_list1(pop_up_list_t *list, uint16 old_active, uint16 active)
{ 
    #if 1
    uint16 tmp;
    //清除旧的活动菜单项
    tmp = old_active % ROW_COUNT_DEF;
//	ResShowPic(NUMCLEAR,0, menu_pos_list[tmp]);
    ResShowMultiString(list->strid[old_active],MENU_STR_X_DEF, menu_pos_list[tmp]);
    show_active_menuItem(list, active);
	#endif
}

/*
 ********************************************************************************
 * Description : 弹出式列表选择窗体
 *
 * Arguments   :
 *    list,  弹出式列表结构体指针
 *    list->string, 语言
 *
 * Returns     : key
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ui_popup_list(pop_up_list_t *list, void(*callback)(uint16 value))
{ 
 
    uint16 i, active, old_active;
    uint8 key;
    uint8 need_draw = FALSE;
    uint8 need_drawall = TRUE;
    uint8 value_changed = FALSE;
    uint16 curGroup; //list->active / LIST_NUM_ONE_PAGE
    uint16 n; //the item to show
    uint16 iCounter = 0;
#if 1
    
    active = list->active;
    while (1)
    {
        if (need_drawall == TRUE)
        {  
            ClearScreen(NULL);
           
//            ResShowMultiString(list->headid, 255, 0);
            need_drawall = FALSE;
            need_draw = TRUE;
        }

        if (need_draw == TRUE)
        {
            ScrollFlag = 0;
            old_active = active;
            curGroup = active / ROW_COUNT_DEF;
            n = curGroup * ROW_COUNT_DEF;
            for (i = 0; i < ROW_COUNT_DEF; i++)
            {
                if ((n + i) >= list->total)
                {
                    break; //处理不足LIST_NUM_ONE_PAGE个的情况
                }

                if ((n + i) == active)
                {
                    continue;
                }
                else
                {   
                	SetAsciiFont(FONT_TYPE_LARGE); 
                    ResShowMultiString(list->strid[n + i], 255,16);//menu_pos_list[i]
                }

            }
            show_active_menuItem(list, active);
            need_draw = FALSE;
			UpdateScreen(NULL);
        }//need draw
        if (value_changed == TRUE)
        {
            show_active_list1(list, old_active, active);
            value_changed = FALSE;
        }
        key = ap_get_message();
        //handle message
        switch (key)
        {
            case AP_KEY_NEXT:
            case AP_KEY_NEXT | AP_KEY_HOLD:
            if (active < (list->total - 1))
            {
                active++;
                if ((active / ROW_COUNT_DEF) == curGroup)
                {
                    old_active = active - 1;
                    value_changed = TRUE;
                }
                else
                {
                    need_draw = TRUE;
                }

            }
            else
            {
                active = 0;
                need_draw = TRUE;
            }
            if (callback != NULL)
            {
                callback(active);
            }

            while (ap_get_message() != 0)
            {

            }
            break;

            case AP_KEY_PREV:
            case AP_KEY_PREV | AP_KEY_HOLD:
            if (active > 0)
            {
                active--;
                if ((active / ROW_COUNT_DEF) == curGroup)
                {
                    old_active = active + 1;
                    value_changed = TRUE;
                }
                else
                {
                    need_draw = TRUE;
                }

            }
            else
            {
                active = list->total - 1;
                need_draw = TRUE;
            }
            if (callback != NULL)
            {
                callback(active);
            }

            while (ap_get_message() != 0)
            {

            }
            break;

            case AP_KEY_PLAY | AP_KEY_UP: //取消退出
            return RESULT_IGNORE;

            case AP_KEY_MODE | AP_KEY_UP: //确认退出
            list->active = active;
            return 0;

            case AP_MSG_WAIT_TIMEOUT:
            if (ui_auto_select == TRUE)
            {
                return RESULT_IGNORE;
            }
            break;

            case AP_MSG_RTC:
            if (ScrollFlag && g_light_flag) // && (!scrtimefalg)
            {
                iCounter++;
                if (iCounter > 4)
                {
                    ScrollString(scroll_buf, FALSE, UNICODELANGUAGE, &region_scroll);
                }
            }
            ui_show_batt( GetBattery());
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                need_drawall = TRUE;
            }
            else if (key != 0)
            {
              
                return key;
            }
            else
            {

            }
        }// switch(key)

    }//while(1)
 
	#endif
    return 1;
}
