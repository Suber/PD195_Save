#include "ap_common.h"
#include "string.h"
#include "Filesys.h"
#include "driver.h"


#pragma name(DIR)

/*
const uint8 g_dir_ext[] = "*  ";

const uint8 g_dir_record[12] = "RECORD     ";

const uint8 g_dir_rec_swim[12] = "SWIM       ";

const uint8 g_dir_rec_pedo[12] = "PEDO       ";
*/


uint8 dir(uint8 mode)
{
    /*
    uint8 g_dir_rec_peim[2][12];
	uint8 i;

    strcpy(g_dir_rec_peim[0], g_dir_record);
    
    if(mode)
    {
        strcpy(g_dir_rec_peim[1], g_dir_rec_swim);
    }
    else
    {
        strcpy(g_dir_rec_peim[1], g_dir_rec_pedo);
    }
        
    
    for(i=0; i<3; i++)
    {
        if(FS_CD(g_dir_rec_peim[0]))
        {
            if(FS_CD(g_dir_rec_peim[1]))
            {
                return -1;
            }
            else
            {
                FS_MakeDir(g_dir_rec_peim[1]);
                FS_CD(":");
            }
       }

       else
       {
            FS_MakeDir(g_dir_rec_peim[0]); 
            FS_CD(":");
       }
    }
    
    */
    
   	mode = mode;
    return 1;
}

