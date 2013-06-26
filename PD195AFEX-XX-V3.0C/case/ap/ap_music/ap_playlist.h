/*
 *******************************************************************************
 *                        SDK30F_D1
 *                     music player header  file
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: ap_playlist.h
 * Version: 1> v1.00     first version    by Mars   2009-2-13 9:28
 *******************************************************************************
 */
#ifndef _UI_PLAYLIST_H
#define _UI_PLAYLIST_H
#include "actos.h"//系统头文件
#include "mmcmd.h"//模块层命令名定义
#include "basal.h"//basal module 头文件
#include "enhanced.h"//enhanced module 头文件
#include "ap_common.h"//控件头文件

/*常量宏定义*/
#define  VM_ALBUM_PICINFO    (GetVMCap()*512 - 0x200000)

#define  PLAYLISTFILENAME      "ASDKMM  LIB"
#define  PLAYLISTMAGIC         "ASDKMMLIB"
#define  FILEINFOR             "FILEINFOR"
#define  ARTISTNAME            "ARTISTS"
#define  ALBUMNAME             "ALBUMS"
#define  GENRENAME             "GENRES"
#define   ALLSONGNAME          "ALLSONGS"
#define   HEADITEMSIZE            16
#define   IDMSTARTADDRESS    0x4000     // IDM/IPM内存起始地址
#define   IDMPAGESIZE              0x4000     // IDM/IPM每个页的扇区数
#define   FILEINDEXADDRESS         0x6000  //存储文件序号在
#define   FILEINFORSIZE       256                 //每个音乐文件存储在数据区的大小
#define   MusicExtNum            9
#define   ID3ITEMSIZE           32
#define   SORTITEMSIZE       8

#define   TOTALFILEMAX             4096
#define   ONEPAGEFILENUM       1024
#define   IPMPAGEFILENUM        (IDMPAGESIZE/FILEINFORSIZE)

#define   ListDisplayNum  5              //一屏最多显示的列表数目
#define   OneSectorListNum         64      //每个扇区存储的列表数目
#define   OneSectorListIndexNum    256      //每个扇区存储的列表数目
/*数据结构定义*/
typedef struct
{
    char ID3TagFlag; //歌曲文件是否存在ID3和TAG信息的标志，1为有，0为无
    char ArtistString[ID3ITEMSIZE]; //艺术家名称 32 bytes
    char AlbumString[ID3ITEMSIZE * 2]; //专辑名称  64bytes
    char GenreString[ID3ITEMSIZE]; //流派名称      32 bytes
    char TitleString[ID3ITEMSIZE * 2]; //标题&文件长名      64 bytes
    char TrackString[8]; //track num string
    unsigned int TrackNum; //专辑曲目号
    unsigned long DirClusterNo; //文件父目录的目录首簇号
    char FileName[12]; //文件短名信息
} music_list_infor_t;

typedef struct
{
    char HeadItemName[12]; //列表名称
    unsigned int ItemNum; //列表个数
    unsigned int ItemOffset; //列表位置
} list_head_item_t;

typedef struct
{
    unsigned int ListIndex;
    unsigned int ListChildNum;
    unsigned int ListTotal;
    unsigned int FileIndex;
} ID3_list_item_t;

typedef enum
{
    AllSong, // 全部歌曲
    Album, //专辑
    Artist, //艺术家
    Genre//流派
} list_type_t;

typedef enum
{
    Playlist, //播放列表类型
    Filelist  //文件列表类型
} music_list_type_t;

/*外部变量声明*/
extern handle list_fp;
extern uint8 ScrollFlag; //滚动标志位
extern uint8 scroll_buf[]; //滚屏buffer区
extern region_t region_scroll; //滚屏区域
extern const WORD menulist_bck_pic[];

extern list_type_t g_list_type; //播放列表类型
extern uint g_playlist_total; //播放列表总数
extern uint g_playlist_offset; //播放列表在数据文件的扇区偏移
extern uint g_playlist_num; //播放列表的序号

extern uint g_filelist_total; //列表中文件总数
extern uint g_filelist_start; //播放列表中起始文件的索引
extern uint g_filelist_num; //列表中的文件序号

extern uint g_filelist_index; //文件在排序索引表中的序号
extern uint g_filelist_offset; //排序索引表在文件中的偏移扇区

/*外部函数声明*/
extern int show_music_list(const WORD headid);
extern int show_music_file(const WORD head_id);
extern void ui_show_list(uint active, uint old_active, list_type_t musiclist_type, bool scroll_flag);

#endif
