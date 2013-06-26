/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: timer ISR module of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_SUB_INT)

#pragma ROM(large)
void Music_timeint(void)
{
    if (g_ABMinTimeCNT != 0)
    {
        g_ABMinTimeCNT--;
    }
    if ((g_ScrollTimeCNT++) > SCROLLTIME_DEF)
    {
        g_ScrollTimeCNT = 0;
        g_ScrollFlag = TRUE;
    }
}

void Music_2HZint(void)
{
    g_2HZ_CNT++;
    g_2HZFlag = !g_2HZFlag;
}

#pragma ROM(huge)

