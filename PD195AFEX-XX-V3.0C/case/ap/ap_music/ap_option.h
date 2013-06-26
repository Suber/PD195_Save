/*
******************************************************************************* *                               AS211A
*                              AS211A
*                              Module: music ap
*                 Copyright(c) 2002-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* Descripton: the head file of music
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xusong    2010-07-14 11:00     1.0             build this file 
 *******************************************************************************
 */
//通过这个文件来配置一些功能,以适合不同的方案使用
//#define OPT_Sup_LRC_fun//支持歌词显示功能
#ifndef PC
//#define OPT_Sup_follow_fun//支持跟读对比功能
#endif
//#define OPT_Sup_AVReplay_fun//是否支持自动复读功能
//#define OPT_Sup_FadeOut_fun//支持淡出功能
//#define OPT_Sup_FadeIn_fun//支持淡入功能
//#define OPT_Sup_FselFolder_fun//支持文件夹切换功能
//#define OPT_Sup_FFRevSpeed_fun//支持快进快退调速
//#define OPT_Sup_FFRevSound_fun//支持快进快退声音
//#define OPT_Sup_FadeInOutTime_fun//支持淡入淡出时间
//#define play_txt_in_music    //支持音乐播放中同时播放电子书
//#define LDB_LRC                         //支持LDB歌词显示
//#define Show_Album_picture //显示专辑图片
//#define USE_SPECTRUM
//------------------------------
#define FollowReadTempFileName  "$$TEMP$$WAV"

#define SCROLLTIME_DEF     260/20           //20ms * 13 = 260ms//滚屏频率分频系数
#define ABReplayMiniTime_def 400/20     //20ms * 10 = 200ms//AB复读的最小长度
//#define ACTIVE_USERPL                   //列表编辑
//#define MUSICEDIT                    //支持音乐编辑功能，包括串烧，插入选段，插入录音，主要应用于录音笔方案
//#define MUSIC_BOOKMARK
