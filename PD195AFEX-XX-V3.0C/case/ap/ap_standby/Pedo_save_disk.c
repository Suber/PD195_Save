#include "ap_common.h"
#include "enhanced.h"
#include "string.h"
#include "Filesys.h"
#include "driver.h"

#pragma name(PEDO_SD)


#define PEDO_RECORD_MAX     8
#define PEDO_RECORD_SAVE_MAX     8
#define PEDO_RECORD_MAGIC   0x1721


typedef unsigned int    UInt;
uint8 pedo_save_record(uint16 sect, UInt f_handle, uint8 *filename, uint8 remain, uint8 new_flag, uint8 mode);

extern UInt g_peim_save_handle;  
extern pedo_save_vars_t far pedo_save_list[PEDO_RECORD_SAVE_MAX];
extern pedo_save_vars_head_t far pedo_save_head;
extern char far buff[512];


extern void read_record(uint8 mode);

uint8 pedo_save_disk(uint8 num, uint8 *filename, uint8 mode)
{
    uint8 new_flag=0;
    uint8 remain = 0;
    UInt f_handle = NULL;
    uint32 file_len = 0L;
    uint16 index;
    //uint16 j;
    uint16 sect;
  
    if(!num)
    {
        return 1;
    }       
    read_record(mode);
    index = pedo_save_head.Total;
    sect = index / 8;

    if(index % 8)
    {
        remain = 1;
    }
    else
    {
        remain = 0;
    }
    if((sect < 0) || (sect > 3) )
    {
        return 0;
    }
    f_handle = FS_FOpen(filename, FS_OPEN_NORMAL);
    if(!f_handle)
    {
        f_handle = FS_FCreate(filename);
        FS_FAttrAlter(ATTR_HIDDEN);
        new_flag = 1;
    }
    if(!f_handle)
    {
        FS_FClose(f_handle);
        return 0;
        
    }
    else
    {
        pedo_save_record(sect, f_handle, filename, remain, new_flag, mode);
        FS_FClose(f_handle);
    }
    return -1;
}
uint8 pedo_save_record(uint16 sect, UInt f_handle, uint8 *filename, uint8 remain, uint8 new_flag, uint8 mode)
{   
   
    //uint16 result; 
    UInt f_handle_tmp = NULL;
    uint32 file_len = 0L;
    uint32 file_tmp_len = 0L;
    //uint16 i;
    //uint16 index;
    
    mode = mode;
	sect = sect;
	remain = remain;
    new_flag = new_flag;
	*filename = *filename;
    
	//if(new_flag)
	{
	    FS_FWrite((const char *)(&pedo_save_head), 1, f_handle);
        FS_FWrite((const char *)(&pedo_save_list[0]), 1, f_handle);
        return 1;
    }
    
    //else
    //{
    //    if(!FS_JustBeing(filename, NULL, 0x10))
    //    {
   //         ui_err_msg(MREADERR);
   //         return 0;
            
   //     }
   //     else
   //     {
   //         if (FS_FRemove(filename))
    //        {
    //            FS_FWrite((const char *)(&pedo_save_head), 1, f_handle);
    //            FS_FWrite((const char *)(&pedo_save_list[0]), 1, f_handle);
     //           return 1;
     //       }
     //       else
    //        {
    //            ui_err_msg(MREADERR);
    //            return 0;
    //        }
            /*
            FS_FSeek(0, 0, f_handle);
            result = FS_FRead(buff, 1, f_handle);
            if(!result)
            {
                return 0;
            }
            /////////
            memcpy(&index, &buff[2], sizeof(uint16));
            /////////read header
            for(i=0; i < sect; i++)
            {
                result = FS_FRead(buff, 1, f_handle);
                if(!result)
                {
                    return 0;
                }
            }
            if(!remain)
            {
                result = FS_FWrite((const char *)(&pedo_save_list[0]), 1, f_handle);
                if(!result)
                {
                    return 0;
                }
                return 1;
            }
            else
            {
                /*
                result = FS_FRead(buff, 1, f_handle);
                if(!result)
                {
                    return 0;
                }
                for(i = 0; i < PEDO_RECORD_SAVE_MAX; i++)
                {
                    if(pedo_save_list[i].Magic != 0x1721)
                    {
                        break;
                    }
                }
                memcpy(&buff[64*remain], &pedo_save_list[0], (512 - 64*remain)*sizeof(char));
                */
                /*
                result = FS_FWrite((const char *)(&pedo_save_list[0]), 1, f_handle);
                if(!result)
                {
                    return 0;
                }
                return 1;
                */
                /*
                if((i + remain) <= 8)
                {
                    return 1;
                }
                else 
                {
                    result = FS_FWrite((const char *)(&pedo_save_list[8 - remain]), 1, f_handle);
                    if(!result)
                    {
                        return 0;
                    }
                    return 1;
                }
                */

            
        
	return 0;
}
