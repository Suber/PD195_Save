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

#pragma renamecode(?PR?KY_GSENSOR3)

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
bool sKY_GsensorOn3()
{
    uint8 ret=1;
    
    disable_interrupt();    //禁止中断
    
    I2C_Init();	
    I2C_Start();
    ret&=I2C_TransByte(0x32);        //--addr--
	ret&=I2C_TransByte(0x20|0x80);   // reg|0x80 I2C_AUTO_INCREMENT
	ret&=I2C_TransByte(0x57);        //工作  0x57 0x2f
	ret&=I2C_TransByte(0x00);        // 0xa8: HP filter 0.5Hz
	ret&=I2C_TransByte(0x00);
	ret&=I2C_TransByte(0x88);
	ret&=I2C_TransByte(0x00);
	I2C_Stop();

    SetMfpToNand();
 	enable_interrupt();     //开中断

 	return ret;
}

void sKY_GsensorOff3()
{
    disable_interrupt(); //禁止中断
    
    I2C_Init();	
    I2C_Start();
    I2C_TransByte(0x32);    //--addr--
	I2C_TransByte(0x20);
	I2C_TransByte(0x00);    //睡眠
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
void sKY_ReadGSensor3(void)
{
	uint8 xtemp,ytemp,ztemp;

	disable_interrupt(); //禁止中断

    I2C_Init();
	I2C_Start();
	I2C_TransByte(0x32);        //--addr--
	I2C_TransByte(0x28|0x80);   // reg|0x80 I2C_AUTO_INCREMENT
	//I2C_Stop();
    I2C_Start();
	I2C_TransByte(0x33); //--addr--
    xtemp = I2C_ReceByte(0x00); 
    gsensor_xadc1 = I2C_ReceByte(0x00); 
    ytemp = I2C_ReceByte(0x00);
    gsensor_yadc1 = I2C_ReceByte(0x00);
	ztemp = I2C_ReceByte(0x00);
	gsensor_zadc1 = I2C_ReceByte(0x01);
	I2C_Stop();
	
    SetMfpToNand();
	enable_interrupt(); //开中断
	
	gsensor_xadc1 = (gsensor_xadc1 << 8) | xtemp; 
	gsensor_yadc1 = (gsensor_yadc1 << 8) | ytemp; 
	gsensor_zadc1 = (gsensor_zadc1 << 8) | ztemp; 
	//12bit
    gsensor_xadc1 = (int16)gsensor_xadc1 >> 4;
    gsensor_yadc1 = (int16)gsensor_yadc1 >> 4;
    gsensor_zadc1 = ((int16)gsensor_zadc1 >> 4) + 264;  //offset

    gsensor_xadc1 = (int16)gsensor_xadc1 + 2048; 
	gsensor_yadc1 = (int16)gsensor_yadc1 + 2048; 
	gsensor_zadc1 = (int16)gsensor_zadc1 + 2048; 
 
}

