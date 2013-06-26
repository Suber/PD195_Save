/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib, directory control file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 * File: Dir_menu_1.c
 * By  : stevenluo
 * Version: v1.00     first version     2010-04-09 12:00
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(DIR_MENU_1)
/*######################################################################*/
/*##常驻数据段,存放在JPEG（8K）空间内，地址为3400-3600############*/
/*######################################################################*/
uint8 dirbuf_longname_flag[ROW_COUNT_DEF];
uint8 dirbuf_filename[ROW_COUNT_DEF][50];
file_record_m OnePageRecord[ROW_COUNT_DEF];


/*######################################################################*/
/*#########################代码段#####################################*/
/*######################################################################*/

/********************************************************************************
 * Description :从缓存空间获取当前激活项的内容信息
 *
 ** Arguments  :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/

void record_curlacation(void)
{
    uint8 sfr_bak, mcsr9_bak;
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    memcpy(&dir_temp_record, &OnePageRecord[dir_list_active % GROUPDEFINE], sizeof(file_record_m));
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
}
/********************************************************************************
 * Description : 获取一屏的数据，然后显示出来
 *
 * Arguments  :
 *            top：列表的开始项
 *		 bottom：列表的结束项
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void show_directory_menu(uint16 dir_top, uint16 dir_bottom)
{
    if ((old_list_active == dir_list_active)&&(need_decode==TRUE))
    {
        dir_DecodeOnePage(dir_top, dir_bottom);
        selmode_changeflag = 0;
    }
	  need_decode=TRUE;//
    dir_ShowDirectory(dir_top, dir_bottom, old_list_active, dir_list_active, list_total_num);
}

/********************************************************************************
 * Description : 根据长名信息滚动激活项的内容
 *
 * Arguments  :
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void scroll_longname(void)
{

    SetAsciiFont(FONT_TYPE_LARGE);
    if (longname_flag == 1)
    {
        ScrollString(scroll_buf, FALSE, UNICODELANGUAGE, &region_scroll);
    }
    else
    {
      //  InvertFlag = 1;
        ScrollString(scroll_buf, FALSE, 0, &region_scroll);
      //  InvertFlag = 0;
    }
    UpdateScreen(NULL);
    SetAsciiFont(FONT_TYPE_SMALL);

}
/********************************************************************************
 * Description : 初始化一些公共变量
 *
 * Arguments  :
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void init_common_para(void)
{   
    old_list_active = dir_list_active;
    top = (dir_list_active / GROUPDEFINE) * GROUPDEFINE;
    bottom = top + GROUPDEFINE - 1;
    if (bottom > (list_total_num - 1))
    {
        bottom = list_total_num - 1;
    }
}
/********************************************************************************
 * Description : 选择下一项时重新初始化选项相关变量
 *
 * Arguments  :
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void sel_next_activeNum(void)
{
    if (list_total_num <= 1)
    {
        return;
    }
    if (dir_list_active < (list_total_num - 1))
    {
        dir_list_active++;
        if ((dir_list_active / GROUPDEFINE) == cur_group)
        {
            old_list_active = dir_list_active - 1;
            return;
        }
        else
        {
            if (selmode == 3)
            {
                selmode_changeflag = 1;
            }
            selmode = 2;
            cur_group = cur_group + 1;
            init_common_para();
        }
    }
    else
    {
        dir_list_active = 0;
        cur_group = 0;
        selmode = 0;
        init_common_para();
    }
}
/********************************************************************************
 * Description : 选择上一项时重新初始化选项相关变量
 *
 * Arguments  :
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void sel_prev_activeNum(void)
{
    if (list_total_num <= 1)
    {
        return;
    }
    if (dir_list_active > 0)
    {
        dir_list_active--;
        if ((dir_list_active / GROUPDEFINE) == cur_group)
        {
            old_list_active = dir_list_active + 1;
            return;
        }
        else
        {
            if (selmode == 2)
            {
                selmode_changeflag = 1;
            }
            selmode = 3;
            cur_group = cur_group - 1;
            init_common_para();
        }
    }
    else
    {
        selmode = 1;
        dir_list_active = list_total_num - 1;
        cur_group = (list_total_num - 1) / GROUPDEFINE;
        init_common_para();
    }
}
/********************************************************************************
 * Description : 浏览文件信息，消息查询，并对不同文件进行相应的处理
 *
 * Arguments  :
 * Returns     :
 *            返回相应的result 值
 * Notes       :
 *
 ********************************************************************************/
uint16 dir_ui_directory(file_location_t *location, uint8 type)
{

    uint8 Strname[12];
    uint8 key;
    uint8 result;
    uint8 no_file_flag = FALSE;
    uint8 long_key = FALSE;
    uint8 timecount = 0;
    uint16 scrolcount = 0;
    uint8 scrol_way = 0;
    uint8 need_draw = TRUE;
    uint8 stop_update = FALSE;
    uint16 ScrollCounter = 0;
 //   uint16 ScrollCounter2=0;

    type = type;
 
    if (location->disk == 'C')
    {
        DiskFlag = 0;
    }
    else
    {
        DiskFlag = 1;
    }

    ScrollFlag = 0; //初始化为不滚屏
    memset(file_name, 0, 12);
    if (location->fselTotal == 0)
    {
        init_dir_buf1(0, 0);
    }
    else
    {
       init_dir_buf1(location->fselTotal, 1); 
    }

    if (list_total_num < 1)
    {
        show_info(DiskFlag);
        return RESULT_IGNORE;
    }
    dir_list_active = 0;
    //dir_list_active=dir_list_active+dir_total_num+1;//longjin
    cur_group = 0;
    selmode = 0;

    init_common_para();
    while (ap_get_message() != 0)
    {
    }
    ap_message_init(NULL);

    while (1)
    {
        if (need_draw == TRUE)
        {
            show_directory_menu(top, bottom);
            ui_show_batt( GetBattery());
            need_draw = FALSE;
        }

        if ( ScrollFlag )
        {
            if(ScrollCounter>3000)
            {
                ScrollCounter = 0;
                scroll_longname();
                
            }
            ScrollCounter++;
        }
		/*
		if(ScrollFlag2 == TRUE)
		{  
		  if(ScrollCounter2++>5000)
		  	{
		    	ScrollCounter2=0;
                ScrollString2_re();
			    UpdateScreen(NULL);
			  
		  	}
		}
		*/
        key = ap_get_message();

        switch (key)
        {
            case AP_KEY_NEXT | AP_KEY_HOLD:
            case AP_KEY_NEXT:
//			updownflag=0;
            sel_next_activeNum();
            need_draw = TRUE;
            break;
            
            case AP_KEY_PREV | AP_KEY_HOLD:
            case AP_KEY_PREV:
//			updownflag = 1;
            sel_prev_activeNum();
            need_draw = TRUE;
            break;
            
            case AP_KEY_MODE | AP_KEY_UP://返回操作
            #if(MACHINE_MODEL == 1)
			case AP_KEY_PREV | AP_KEY_UP:
			#endif
            return_frontdir: if (dir_fselDirLayer == 0) //根目录不响应
            {
                //    dir_fselDirNo[dir_fselDirLayer] = 0;
                // dir_fselGetLocation(location);
                return RESULT_IGNORE;
            }
            // record_curlacation();
            dir_fselDirNo[dir_fselDirLayer] = 0;
            if (dir_fselRetParentDir(Strname) == TRUE)
            {
                init_dir_buf1(0, 0); //初始化目录查询表并计算有效目录总数和文件总数
                dir_list_active = dir_fselDirNo[dir_fselDirLayer];
                old_list_active = dir_list_active;
                cur_group = dir_list_active / GROUPDEFINE;
                init_common_para();
            }
            need_draw = TRUE;
            break;
            
            case AP_KEY_PLAY | AP_KEY_UP://确定
		    //case AP_KEY_NEXT | AP_KEY_UP:
            if ((dir_fselDirLayer == 0) && (dir_list_active == 0))
            {
                //  dir_fselGetLocation(location);
                return RESULT_IGNORE;
            }
            record_curlacation();
            if (dir_temp_record.name[0] == '\\') //当前所处子目录第一项时,需要返回父目录
            {
                goto return_frontdir;
            }
            if (dir_temp_record.filetype == TYPE_DIR)
            {
                if (dir_fselDirLayer >= TOTALDIRLAYER) //需实验看看是7还是8?
                {
                    break;
                }
                if (!dir_fselEntCurrentDir(dir_temp_record.name)) //调完后dir_fselDirLayer加1
                {
                    break;
                }
                //初始化目录查询表并计算有效目录总数和文件总数
                init_dir_buf1(0, 0);
                dir_fselDirNo[dir_fselDirLayer - 1] = (uint8) dir_list_active;
                dir_list_active = 0;
                cur_group = 0;
                init_common_para();
            }
            else
            {   
                /*result = show_del_play();
				if(result==RESULT_MUSIC_PLAY)
				{*/
	                if (dir_fselDirLayer == 0)
	                {
	                    dir_fselDirNo[dir_fselDirLayer] = 0;
	                }
	                dir_fselFileNo = dir_list_active - dir_total_num;
	                result = dir_File_Type(dir_temp_record.name);

	                if (result != NULL)
	                {
	                    dir_fselGetLocation(location);
	                    memcpy(location->filename, dir_temp_record.name, 12);
	                    location->DirEntry = dir_temp_record.DirEntry;
	                    return result;
	                }
	                else
	                {
	                    //	   show_error_info(MFMTERR);
	                    old_list_active = dir_list_active;
	                }
				 /*}
				 else if(result==0)//删除文件
				 {
				        if (dir_list_active == 0)
			            {
			                break;
			            }
				        record_curlacation();
				        if(dir_temp_record.filetype == 0)
				      	{
				      	   break;
				      	}		
			            result = show_delete_menu(type);
						 if (result == 0)
                         {
		                     result = del_sel_file(type, dir_list_active + 1);
		                     if (result == TRUE)
		                    {
		                    dir_fselTotal--;
		                    list_total_num--;
		                    if (list_total_num == 1)
		                    {
		                        dir_list_active=0;
		                    }
		                    if ((list_total_num - dir_list_active) < GROUPDEFINE)
		                    {
		                        selmode = 1;
		                    }
		                    else
		                    {
		                        selmode = 0;
		                    }
		                    if (dir_list_active >= list_total_num)
		                    {
		                        dir_list_active = list_total_num - 1;

		                    }
		                    cur_group = dir_list_active / GROUPDEFINE;
		                    dir_fselFileNo = dir_list_active;
		                    init_common_para();
		                    need_draw = TRUE;
		                    break;
		                    }
							 else
			                {
			                   //    ui_err_msg(DELFAIL);
			                    return RESULT_IGNORE;
			                }
                         }
							 
				  }
				 
				  if(result==RESULT_IGNORE)//删除文件取消与播放取消共用
				  {
	                if ((list_total_num - dir_fselFileNo) < GROUPDEFINE)
	                {
	                    selmode = 1;
	                }
	                else
	                {
	                    selmode = 0;
	                }
	                cur_group = dir_list_active / GROUPDEFINE;
	                dir_fselFileNo = dir_list_active + 1;
	                init_common_para();
	                need_draw = TRUE;
	                break;
				  }
				  else
				  return result;*/
						 
            }
            need_draw = TRUE;
            break;
/*
			case AP_KEY_VOLADD | AP_KEY_UP:
            case AP_KEY_VOLSUB | AP_KEY_UP:
	        case AP_KEY_VOL| AP_KEY_UP:
	 del_file: 	
	 	    if (dir_list_active == 0)
            {
                break;
            }
	        record_curlacation();
	        if(dir_temp_record.filetype == 0)
	      	{
	      	   break;
	      	}		
            result = show_delete_menu(type);
            if (result == 0)
            {
                result = del_sel_file(type, dir_list_active + 1);
                if (result == TRUE)
                {
                    dir_fselTotal--;
                    list_total_num--;
                    if (list_total_num == 1)
                    {
                        dir_list_active=0;
                    }
                    if ((list_total_num - dir_list_active) < GROUPDEFINE)
                    {
                        selmode = 1;
                    }
                    else
                    {
                        selmode = 0;
                    }
                    if (dir_list_active >= list_total_num)
                    {
                        dir_list_active = list_total_num - 1;

                    }
                    cur_group = dir_list_active / GROUPDEFINE;
                    dir_fselFileNo = dir_list_active;
                    init_common_para();
                    need_draw = TRUE;
                    break;

                }
                else
                {
//                    ui_err_msg(DELFAIL);
                    return RESULT_IGNORE;
                }
            }
            else if (result == RESULT_IGNORE)
            {
                if ((list_total_num - dir_fselFileNo) < GROUPDEFINE)
                {
                    selmode = 1;
                }
                else
                {
                    selmode = 0;
                }
                cur_group = dir_list_active / GROUPDEFINE;
                dir_fselFileNo = dir_list_active + 1;
                init_common_para();
                need_draw = TRUE;
                break;
            }
            else
            {
                //   GetLocationByName(location, temp_filename, temp_num, type);
                return result;
            }
			
            case AP_KEY_MODE | AP_KEY_LONG:
            if (type == FSEL_TYPE_DIR)
            {
                record_curlacation();
                if ((dir_fselDirLayer == 0) && (dir_list_active == 0))
                {
                    dir_fselDirLayer = 0;
                    dir_fselFileNo = 0;
                    memset(dir_fselDirNo, 0, 8);
                }
                else if (dir_temp_record.name[0] == '\\')//当处于目录首时，什么都不用做
                {
                }
                else if ((dir_temp_record.filetype == TYPE_DIR) && (dir_fselDirLayer <= 8))
                {

                    dir_fselEntCurrentDir(dir_temp_record.name); //调完后dir_fselDirLayer加1
                    dir_fselDirNo[dir_fselDirLayer - 1] = (uint8) dir_list_active;

                }
                else
                {

                }
                dir_fselGetLocation(location); //更新location
                memcpy(location->filename, dir_temp_record.name, 12);
                location->DirEntry = dir_temp_record.DirEntry;
                return RESULT_NULL;
            }
            return RESULT_MAIN;
            */
            
            case AP_MSG_WAIT_TIMEOUT:
            break;
            
            case AP_MSG_RTC:
            ui_show_batt( GetBattery());
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key != 0)
            {
                if (key == RESULT_STANDBY)
                {
                    dir_fselGetLocation(location);
                    memcpy(location->filename, dir_temp_record.name, 12);
                    location->DirEntry = dir_temp_record.DirEntry;
                    //save_location(location);
                    return key;
                }

                if (key == RESULT_REDRAW)
                {
                    if (location->disk == 'C')
                    {
                        DiskFlag = 0;
                    }
                    else
                    {
                        DiskFlag = 1;
                    }

                    dir_ShowDirectory(top, bottom, dir_list_active, dir_list_active, list_total_num);
                }
                else
                {
                    return key;
                }
            }
            break;
        } //switch() end
        //音乐播放时自动换曲的处理
    }//while(1) end

    return 1;
}
