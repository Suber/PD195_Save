#include "actos.h"
#include "chip_cfg.h"
#include "ap_cfg.h"
#include "math.h"

extern void SetMfpToNand(void);
extern void I2C_Init(void);
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern uint8 I2C_TransByte(uint8 param1);
extern uint8 I2C_ReceByte(uint8 param);

extern uint8 gsensor_posflag;
extern uint32 step_counter;		//20110524	已运动步数
extern uint32 step_second;
extern uint32 step_counter_swim;		//20110524	已运动步数
extern uint32 step_second_swim;
extern uint16 gsensor_xadc1;
extern uint16 gsensor_yadc1;
extern uint16 gsensor_zadc1;
extern uint16 gsensor_gap;
extern uint16 gsensor_gap_last;
extern uint16 gsensor_got;
extern uint8 g_debounce;
extern uint8 earOutFlag;
extern uint32 gf_tmp32;

#pragma renamecode(?PR?KY_GSENSOR1)

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
bool sKY_GsensorOn1()
{
    uint8 ret=1;
    
    disable_interrupt();    //禁止中断

	I2C_Init();	    
	I2C_Start();
	ret&=I2C_TransByte(0x2a);    //--addr--
	ret&=I2C_TransByte(0x00);
	ret&=I2C_TransByte(0x00);    //工作
	I2C_Stop();

    SetMfpToNand();
 	enable_interrupt();     //开中断

 	return ret;
}

void sKY_GsensorOff1()
{
    disable_interrupt(); //禁止中断
    
	I2C_Init();
	I2C_Start();
	I2C_TransByte(0x2a); //--addr--
	I2C_TransByte(0x00);
	I2C_TransByte(0x01);	//睡眠
	I2C_Stop();

    SetMfpToNand();
	enable_interrupt(); //开中断
}

/*
 *******************************************************************************
 *                sKY_ReadGSensor{RPrefix}:
 *
 * Descriptor:
 *
 * Arguments: VOID
 *
 * Returns:   VOID
 *******************************************************************************
 */
void sKY_ReadGSensor1(void)
{
	uint32 i = 0;
	uint8 xtemp,ytemp;

	disable_interrupt(); //禁止中断
	
    I2C_Init();
	I2C_Start();
	I2C_TransByte(0x2a); //--addr--
	I2C_TransByte(0x01); //0x01
	//I2C_Stop();
   	I2C_Start();
	I2C_TransByte(0x2b); //--addr--
    gsensor_xadc1 = I2C_ReceByte(0x00); 
	xtemp = I2C_ReceByte(0x00); 
	gsensor_yadc1 = I2C_ReceByte(0x00);
	ytemp = I2C_ReceByte(0x01);
	I2C_Stop();
	
    SetMfpToNand();
	enable_interrupt(); //开中断

	gsensor_xadc1 = (gsensor_xadc1 << 8) | xtemp; 
	gsensor_yadc1 = (gsensor_yadc1 << 8) | ytemp; 
	gsensor_xadc1 = gsensor_xadc1*2 -2048;
	gsensor_yadc1 = gsensor_yadc1*2 -2048;  // 1g=>1024
	gsensor_zadc1 = 2048;
	
}


