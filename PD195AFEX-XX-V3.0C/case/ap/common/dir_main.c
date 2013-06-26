/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib, directory control file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 * File: Dir_main.c
 * By  : stevenluo
 * Version: v1.00     first version     2010-04-09 12:00
 *******************************************************************************
 */

#include "ap_common.h"
extern uint16 g_musicsequence;
extern void dir_getSelNumByFileName(file_location_t *location);
#pragma name(DIR_MAIN)

/*######################################################################*/
/*#########################BANK内的常数段#############################*/
/*######################################################################*/
static const uint8 file_pos[3] =
{16,32,48};

//{ POSITION_MENU1, POSITION_MENU2, POSITION_MENU3, POSITION_MENU4, POSITION_MENU5, POSITION_MENU6 }; //列表项的相对位置
//static const uint16 file_bck_pic[6] =
//{ MENUBCK6, MENUBCK5, MENUBCK4, MENUBCK3, MENUBCK2, MENUBCK1 };//列表项的背景图
//static const uint16 headID_pic[8] =
//{ T_MUSIC, T_VOICE, T_BROWSER, T_PHOTO, T_EBOOK, T_MOVIE, T_BROWSER, T_RECORD };//列表头的图片信息
static const uint16 file_icon[3] =
{ IM_BROWSER_ICON_FOLDER, ICON_FILE, IM_BROWSER_ICON_MUSIC};//列表选项的图标
static const uint16 file_icon1[3] =
{ IM_BROWSER_ICON_FOLDER, IM_BROWSER_ICON_MUSIC, IM_BROWSER_ICON_MUSIC };//列表选项的图标

/*######################################################################*/
/*#########################常驻数据段################################*/
/*######################################################################*/
file_record_m dir_temp_record; //缓存当前列表激活项的文件类型和文件短名
uint8 longname_flag; //当前文件选项是否为长名标记
uint8 DiskFlag = 0; //盘符标记0='C',1='H'
uint8 file_mode;
uint16 dir_total_num = 0; //当前目录的文件夹总数
uint16 file_total_num = 0; //当前目录过滤后的文件总数
uint16 list_total_num = 0; //当前目录文件夹和文件的总和，再加上目录首
uint16 dir_list_active = 0; //当前列表的激活项
uint16 old_list_active = 0;//当前列表旧的激活项
uint16 cur_group = 0; //当前列表相对list_total_num的分组后所处的位置
uint16 top; //当前列表的开始项
uint16 bottom; //当前列表的结束项
uint8 selmode = 0;//文件浏览时文件切换情况，3-切换上一屏数据，2-切换下一屏数据，1-从第一个文件切换到最后一个，
uint8 selmode_changeflag = 0;//文件浏览时切换方向改变标记
uint8 need_decode=TRUE;
/*######################################################################*/
/*#########################代码段#####################################*/
/*######################################################################*/

/********************************************************************************
 * Description : 根据当前的文件类型和索引号输出选项的内容
 *
 * Arguments  :
 *            icon：0-文件夹图标，1-文件图标，2-图片图标
 *		 index：当前列表的索引号
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void put_StrTypeAndIcon(uint8 icon, uint8 index)
{

    uint8 sfr_bak, mcsr9_bak;
    uint8 filename[50];
    uint8 longnameflag;
    if ((icon > 0) && (dir_fselType == FSEL_TYPE_DSC))
    {
        icon = icon + 1; //显示图片应用列表的小图标
    }
	if(dir_temp_record.name[0]=='\\')
	{
	 	if(dir_list_active == 0)
		{
		 	ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_BACK_DIR_SELECT,16, file_pos[0]);
		}
		else
		{
			ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_BACK_DIR,16, file_pos[0]);
		}
	 return;
	}
	else if(memcmp(dir_temp_record.name,"ROOT",5)==0)//主盘目录
	{
		if(dir_list_active == 0)
		{
		 	ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_HOME_SELECT,16, file_pos[0]);
		}
		else
		{
			ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_HOME,16, file_pos[0]);
		}
     return;
	}
	else
	{
			ResShowPic(file_icon[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[index]);
			/*if(activeflag == TRUE)
			{
						ResShowPic(file_icon1[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[index]);	//active - dir_top
						ResShowPic(IM_MENU_TEXT_SEL,0, file_pos[index]);
						activeflag = FALSE;
			}*/
	}
	SetAsciiFont(FONT_TYPE_LARGE);
    SetTextPos(19, file_pos[index]);//文件名坐标
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    memcpy(filename, &dirbuf_filename[index], 50);
    longnameflag = dirbuf_longname_flag[index];
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
    if (longnameflag == TRUE)
    {
        PutUnS(filename, -1);
    }
    else
    {
        PutS(filename, -1);
    }
    //SetStrEndPosition(0);

}
/********************************************************************************
 * Description : 获取当前激活项的信息内容
 *
 * Arguments  :
 *            selNum: 当前激活项号
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void show_active_Item(uint16 selNum)
{

    uint8 tmp = (uint8) (selNum % ROW_COUNT_DEF);
    uint8 sfr_bak, mcsr9_bak;
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    memcpy(&dir_temp_record, &OnePageRecord[tmp], sizeof(file_record_m));
    memcpy(scroll_buf, dirbuf_filename[tmp], 50);
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)

}
/********************************************************************************
 * Description : 根据不同AP需要显示的文件格式显示列表头的图片信息
 *
 * Arguments  :
 *            index: 浏览的文件类型所对应的图片ID:
 *#define FSEL_TYPE_MUSIC         0      //音乐文件
 *#define FSEL_TYPE_VOICE         1      //放音文件
 *#define DIRECTORY_ALL           2       //所有文件格式
 *#define FSEL_TYPE_DSC       3		  //图片文件
 *#define	FSEL_TYPE_TXT	    4         //电子书文件
 *#define	FSEL_TYPE_AMV	    5         //视频文件
 *#define	FSEL_TYPE_ALL	    6        //所有文件格式
 *#define	FSEL_TYPE_DIR       7         //文件夹
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/


/********************************************************************************
 * Description : 文件浏览的显示窗口
 *
 * Arguments  :
 *            top: 文件列表的开始项
 *            bottom : 文件列表的结束项
 Old_active：文件选项的旧激活项
 active：文件选项的新激活项
 total_num：当前目录的文件总数（暂时保留）
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void dir_ShowDirectory(uint16 dir_top, uint16 dir_bottom, uint16 Old_active, uint16 active, uint16 total_num)
{	

    uint16 i, tmp;
    uint8 str_lenth;
    int8 str_type;
    int8 language_id;
    uint8 temp_flag;
    uint8 sfr_bak, mcsr9_bak;
//	uint8 type_file;
    SetAsciiFont(FONT_TYPE_LARGE);
    if (!total_num)
    {

    }
    //show_head_info(active,total_num);
  
    if (Old_active != active)
    { 
        //清除旧的活动菜单项
        tmp = (Old_active - dir_top) % ROW_COUNT_DEF;
        region_scroll.x = 0;
        region_scroll.width = Display_Length- region_scroll.x;;
        region_scroll.height = 16;
        region_scroll.y = file_pos[tmp];
        ClearScreen(&region_scroll);  
       // ResShowPic(file_bck_pic[tmp], MENU_PTR_X_DEF, file_pos[tmp] - 2);
        DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
        memcpy(&dir_temp_record, &OnePageRecord[tmp], sizeof(file_record_m));
        DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak);
        put_StrTypeAndIcon(dir_temp_record.filetype, (uint8) tmp); 
		show_active_Item(active - dir_top);  
    }
    else //if(Old_active==active)        //新旧活动项相同表示是新的一屏数据,需要全部刷新
    {  
        ClearScreen(NULL);
//		ResShowMultiString(BROWSER,255, 0);//显示目录浏览
        for (i = dir_top; i <= dir_bottom; i++)
        {
            DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
            memcpy(&dir_temp_record, &OnePageRecord[i - dir_top], sizeof(file_record_m));
            DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
            if (i == active)
            {   
               // ResShowPic(file_icon[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[active - dir_top]);//
                DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
                memcpy(scroll_buf, dirbuf_filename[i - dir_top], 50);
                DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
                continue;
            }
            put_StrTypeAndIcon(dir_temp_record.filetype, (uint8) (i - dir_top));
        }
    }
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    temp_flag = dirbuf_longname_flag[active - dir_top];
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
    if (temp_flag!=0)
    {
        str_lenth = 25;
        str_type = 1;
        language_id = UNICODELANGUAGE;
        longname_flag = 1;
    }
    else
    {
        str_lenth = 50;
        str_type = 0;
        language_id = 0;
        longname_flag = 0;
    }

//    ResShowPic(MENUDOTA,0, file_pos[active - dir_top]); //显示指示图标 
    if ((GetStringLength(scroll_buf, str_lenth, str_type)) < 96)
    {
    	ResShowPic(IM_MENU_TEXT_SEL,0, file_pos[active - dir_top]); //显示指示图标 
//        ResShowPic(file_icon1[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[active - dir_top]);
        ScrollFlag = 0;
        show_active_Item(active - dir_top);
        //ResShowPic(FILEBCK, EXPLORER_STR_X_DEF - 2, file_pos[active - dir_top] - 1);
        put_StrTypeAndIcon(dir_temp_record.filetype, (uint8) (active - dir_top));
    }
    else
    { 
    	ResShowPic(IM_MENU_TEXT_SEL,0, file_pos[active - dir_top]); //显示指示图标 
        ResShowPic(file_icon1[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[active - dir_top]);
        ScrollFlag = 1;
		region_scroll.x = 19;
        region_scroll.width = Display_Length - region_scroll.x;
        region_scroll.height = 16;
        region_scroll.y = file_pos[active - dir_top];
	    ClearScreen(&region_scroll);  
        ScrollString(scroll_buf, TRUE, language_id, &region_scroll);
#if 0
        UpdateScreen(NULL);
        //test
        i = 0;
        while(1)
        {
            if(i>3000)
                 
            {
                ScrollString(scroll_buf, FALSE, UNICODELANGUAGE, &region_scroll);
                UpdateScreen(NULL);
                i =0 ;
            }
            else
            {
                i++;
            }
            
        }
#endif
        
    }
   // InvertRegion(&region_scroll); //选中的目录反白 
    UpdateScreen(NULL);
//    SetAsciiFont(FONT_TYPE_SMALL);
}

/********************************************************************************
 * Description : 文件导航的入口函数
 *
 * Arguments  :
 *            browser_vars_t: 文件导航的功能结构体
 *            type : 浏览的文件类型:
 *#define FSEL_TYPE_MUSIC         0      //音乐文件
 *#define FSEL_TYPE_VOICE         1      //放音文件
 *#define DIRECTORY_ALL           2       //所有文件格式
 *#define FSEL_TYPE_DSC       3		  //图片文件
 *#define	FSEL_TYPE_TXT	    4         //电子书文件
 *#define	FSEL_TYPE_AMV	    5         //视频文件
 *#define	FSEL_TYPE_ALL	    6        //所有文件格式
 *#define	FSEL_TYPE_DIR       7         //文件夹
 * Returns     :
 *            相应的result 值
 * Notes       :  only select the valid directory
 *
 ********************************************************************************/
uint16 ui_directory(file_location_t *g_browser_vars, uint8 type)
{

    uint16 result;
    file_location_t temp_location;
    list_total_num = 0;
    dir_total_num  = 0;
    file_total_num = 0;
    file_mode = type;
	
	//=========
//	dir_list_active =g_musicsequence;//歌曲序列号 longjin
	//=========
    memcpy(&temp_location, g_browser_vars, sizeof(file_location_t));
    dir_fselInit(type, FSEL_ALL_REPEATALL, 0); //初始化当前磁盘的文件系统
    dir_fselSetLocation(g_browser_vars);
    result = dir_ui_directory(g_browser_vars, type);
    if (result == 0)
    {
        return dir_File_Type(g_browser_vars->filename);
    }
    else if (result == RESULT_IGNORE)
    {
        dir_fselSetLocation(&temp_location);
		dir_getSelNumByFileName(&temp_location);//更新文件总数和文件序号 longjin
        memcpy(g_browser_vars, &temp_location, sizeof(file_location_t));	
    }
    else
    {

    }
    return result;

}

