/*
 *******************************************************************************
 *                        SDK30F_D1
 *                     music player header  file
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: ap_music.h
 * By  : rcmai
 * Version: 1> v1.00     first version     2004-3-19 8:28
 *******************************************************************************
 *******************************************************************************
 */
#ifndef _ap_music_shuffle_H

#define _ap_music_shuffle_H

#define GroupNum 10              //定义每组歌曲的总数
extern uint16 far FileNumArr[GroupNum]; //用来存放一组需要进行shuffle操作的文件序号
extern uint16 far CurrentfileNum; //当前已经播放了多少首歌曲
extern uint16 far Nextgroup; //当前播放到第几组的下一组序号
extern uint16 far TotalGroup; //共分多少组
extern uint8 far Currentgroupfilenum; //当前播放的歌曲在播放数组里的序号
extern uint8 far curgrouptotal;

BOOL shuffleAll(uint8 *strFile, bool *firstshuffle);
BOOL SelectNext(char* strFile);
uint16 GetFileTotalCur(void);
#endif     //_ap_music_shuffle_H
