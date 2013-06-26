#include "ap_common.h"
#include "enhanced.h"
#include "string.h"
#include "Filesys.h"
#include "driver.h"

#pragma name(FMT_DATA)


#define PEDO_RECORD_MAX     20
#define PEDO_RECORD_MAGIC   0x1721



uint8 fmt_date2save(uint8 *sect, pedo_save_vars_t **tmp, uint8 num, uint8 mode)
{
    uint8 i;
    for(i = 0; i < num; i++)
    {
       strcpy(sect, tmp[i])
    }
    return -1;
}
