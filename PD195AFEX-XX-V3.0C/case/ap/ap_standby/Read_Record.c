#include "ap_common.h"
#include "enhanced.h"
#include "string.h"
#include "Filesys.h"
#include "driver.h"

#pragma name(PEDO_RR)


#define PEDO_RECORD_MAX     8
#define PEDO_RECORD_SAVE_MAX     8
#define PEDO_RECORD_MAGIC   0x1721

const uint8 Mode[32] = "PDXXX_VX.X_XG_WATER_PROFF_MP3";

extern pedo_save_vars_t far pedo_save_list[PEDO_RECORD_SAVE_MAX];
extern pedo_save_vars_head_t far pedo_save_head;
extern char far buff[512];

void read_record(uint8 mode)
{
    uint8 i;
	uint16 j;
    uint16 addr;
    date_t date;
    time_t time;
    uint8 Pedo_Record_buff[PEDO_RECORD_MAX*sizeof(Pedo_Record_t)];
    
    TM_GetDate(&date, 0);
    TM_GetTime(&time);	
    if(mode)    
    {   
        addr = VM_AP_SWIM;  
    }
    else  
    {
        addr = VM_AP_PEDO;
    }   

    VMRead(Pedo_Record_buff, addr, PEDO_RECORD_MAX*sizeof(Pedo_Record_t));


    for(i=0;i<PEDO_RECORD_MAX;i++)
    {
        memcpy(&Pedo_record, &Pedo_Record_buff[i*sizeof(Pedo_Record_t)], sizeof(Pedo_Record_t));
        if(Pedo_record.magic != PEDO_RECORD_MAGIC)  
            break;
        
        pedo_save_list[i].Magic = Pedo_record.magic;
        pedo_save_list[i].Age = g_comval.Age;
        pedo_save_list[i].Gender = g_comval.Gender;
        pedo_save_list[i].Height = g_comval.Height;
        pedo_save_list[i].No = i;
        pedo_save_list[i].Date.year = Pedo_record.Date.year;
        pedo_save_list[i].Date.month = Pedo_record.Date.month;
        pedo_save_list[i].Date.day = Pedo_record.Date.day;
        pedo_save_list[i].Time.hour = Pedo_record.Time.hour;
        pedo_save_list[i].Time.minute = Pedo_record.Time.minute;
        pedo_save_list[i].Time.second = Pedo_record.Time.second;
        pedo_save_list[i].Weight = Pedo_record.Weight;
        pedo_save_list[i].PoolLen = Pedo_record.PoolLen;             
        pedo_save_list[i].TotalSec = Pedo_record.TotalSec;
        pedo_save_list[i].TotalStep = Pedo_record.TotalStep;
        pedo_save_list[i].CRC = 0;
        for(j=0; j < 34; j++)
		{
            pedo_save_list[i].Resv[j] = 0;
        }
        pedo_save_list[i].Cate = mode;
    }
    
    pedo_save_head.Magic = 0x1721;
    pedo_save_head.Version = 1;
    pedo_save_head.Total = i;
    pedo_save_head.Cate = mode;
    pedo_save_head.CRC = 0;
    pedo_save_head.Date.year = date.year;
    pedo_save_head.Date.month = date.month;
    pedo_save_head.Date.day = date.day;
    pedo_save_head.Time.hour = time.hour;
    pedo_save_head.Time.minute = time.minute;
    pedo_save_head.Time.second = time.second;
    memcpy(&pedo_save_head.Mode[0], &Mode[0], sizeof(Mode));
    for(j=0; j < 462; j++)
    {
        pedo_save_head.Resv[j] = 0;
    }  
    for(j=0; j < 512; j++)
    {
        buff[i] = 0;
    }
    //for(i=0;i<num;i++)
    //{
    //strcpy(sect, pedo_save_list);
    //}


    //VMWrite(Pedo_Record_buff, addr);
}

