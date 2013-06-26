#include "ap_common.h"
#include "enhanced.h"
#include "string.h"
#include "Filesys.h"
#include "driver.h"

#pragma name(PEDO_SAVE)


#define PEDO_RECORD_MAX     8
#define PEDO_RECORD_SAVE_MAX     8
#define PEDO_RECORD_MAGIC   0x1721

extern pedo_save_vars_t far pedo_save_list[PEDO_RECORD_SAVE_MAX];
extern pedo_save_vars_head_t far pedo_save_head;
extern char far buff[512];

uint8 pedo_save(uint8 mode)
{

    uint8 i;
	uint16 j;
    uint8 Cnum;
    uint16 addr;
    uint8 Pedo_Record_buff[PEDO_RECORD_MAX*sizeof(Pedo_Record_t)];
    
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
        pedo_save_list[i].Magic = 0;
        //if(Pedo_record.PedoUnit)
        //{
            pedo_save_list[i].No = 0;
            pedo_save_list[i].Age = 0;
            pedo_save_list[i].Cate = 0;
            pedo_save_list[i].Date.year = 0;
            pedo_save_list[i].Date.month = 0;
            pedo_save_list[i].Date.day = 0;
            pedo_save_list[i].Time.hour = 0;
            pedo_save_list[i].Time.minute = 0;
            pedo_save_list[i].Time.second = 0;
            pedo_save_list[i].Gender = 0;
            pedo_save_list[i].Height = 0;
            pedo_save_list[i].Cate = 0;
			for(j=0; j < 34; j++)
			{
                pedo_save_list[i].Resv[j] = 0;
            }
            //pedo_save_list[i].Stride = 0;//(uint32) Pedo_record.Stride* 254 / 100;
            pedo_save_list[i].Weight = 0;//(uint32) Pedo_record.Weight * 454 / 1000;
            //pedo_save_list[i].ArmLen = 0;//(uint32) Pedo_record.ArmLen * 254 / 100;
            pedo_save_list[i].PoolLen = 0;//(uint32) Pedo_record.PoolLen * 914 / 1000;
        //}
        //else
        //{
           // pedo_save_list[i].Stride = Pedo_record.Stride;
           // pedo_save_list[i].Weight = Pedo_record.Weight;
           // pedo_save_list[i].ArmLen = Pedo_record.ArmLen;
           // pedo_save_list[i].PoolLen = Pedo_record.PoolLen;
        //}
        
        //pedo_save_list[i].PedoUnit = 0;//Pedo_record.PedoUnit;
        pedo_save_list[i].TotalSec = 0;//Pedo_record.TotalSec;
        pedo_save_list[i].TotalStep = 0;//Pedo_record.TotalStep;
        
        //if(mode)
        //{
            //pedo_save_list[i].Distance = 0;//(uint32) pedo_save_list[i].ArmLen *  pedo_save_list[i].TotalSec / 100;
            //pedo_save_list[i].LapCount = 0;//(uint32) pedo_save_list[i].Distance / pedo_save_list[i].PoolLen;
        //}
        //else
        //{
        //    pedo_save_list[i].Distance = 0;//(uint32) pedo_save_list[i].Stride *  pedo_save_list[i].TotalSec / 100;
        //    pedo_save_list[i].LapCount = 0;
        //}
        
        //pedo_save_list[i].Calorie = 0;//(uint32) ((187 * pedo_save_list[i].Weight * pedo_save_list[i].Distance) / 200) / 1000;
        

    }
    
    if(i == PEDO_RECORD_MAX)    
    {
        Cnum = PEDO_RECORD_MAX;
    }
    else Cnum = i;
    
    pedo_save_head.Magic = 0;
    pedo_save_head.Version = 0;
    pedo_save_head.Total = 0;
    pedo_save_head.Cate = 0;
    pedo_save_head.Date.year = 0;
    pedo_save_head.Date.month = 0;
    pedo_save_head.Date.day = 0;
    pedo_save_head.Time.hour = 0;
    pedo_save_head.Time.minute = 0;
    pedo_save_head.Time.second = 0;
    pedo_save_head.CRC = 0;
    for(j = 0; j < 32; j++)
    {
        pedo_save_head.Mode[j] = 0;
    }
    for(j = 0; j < 462; j++ )
    {
        pedo_save_head.Resv[j] = 0;
    }

  return Cnum;  

}

