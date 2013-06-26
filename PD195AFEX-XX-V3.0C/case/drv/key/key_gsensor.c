#include "actos.h"
#include "chip_cfg.h"
#include "ap_cfg.h"
#include "key.h"

extern uint8 gsensor_type;

#pragma renamecode(?PR?KY_GSENSOR)

/*
 *******************************************************************************
 *                sKY_GsensorOn/Off{RPrefix}:
 *
 * Descriptor:
 *
 * Arguments: VOID
 *
 * Returns:   VOID
 *******************************************************************************
 */
void sKY_GsensorCheck()
{
    if(BAPI_sKY_GsensorOn1())
        gsensor_type=1;
    else if(BAPI_sKY_GsensorOn2())
        gsensor_type=2;
    else if(BAPI_sKY_GsensorOn3())
        gsensor_type=3;
    else
        gsensor_type=0;
}

void sKY_GsensorOn()
{
    if(gsensor_type==1)
        BAPI_sKY_GsensorOn1();
    else if(gsensor_type==2)
        BAPI_sKY_GsensorOn2();
    else if(gsensor_type==3)
        BAPI_sKY_GsensorOn3();
}

void sKY_GsensorOff()
{
    if(gsensor_type==1)
        BAPI_sKY_GsensorOff1();
    else if(gsensor_type==2)
        BAPI_sKY_GsensorOff2();
    else if(gsensor_type==3)
        BAPI_sKY_GsensorOff3();
}

void sKY_ReadGSensor(void)
{
    if(gsensor_type==1)
        BAPI_sKY_ReadGSensor1();
    else if(gsensor_type==2)
        BAPI_sKY_ReadGSensor2();
    else if(gsensor_type==3)
        BAPI_sKY_ReadGSensor3();
}



