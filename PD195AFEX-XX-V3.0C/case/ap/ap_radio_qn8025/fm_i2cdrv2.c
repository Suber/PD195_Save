/********************************************************************************************
 Airoha Technology FM module driver(I2C DRIVER)  for ATJ2091
 last update 2007/09/19
 Version:V0.1
 ********************************************************************************************/

/***********************I2C CONTROL*********************/
//SDA output/input enable port-GPIOC1
#include "ap_radio.h"

#pragma name(FM_IIC_DRV)
uint8 sfr_bak;
//#define     SDA_OUTPORT         (uint8*)GPIOBOUTEN
//#define     SDA_OUTPORT_NUM      0x05          //B5 output enalbe
//#define     SDA_INPORT           (uint8*)GPIOBINEN
//#define     SDA_INPORT_NUM       0x05

//#define     SCL_OUTPORT          (uint8*)GPIOBOUTEN
//#define     SCL_OUTPORT_NUM      0x04           //B4 output enable
//#define     SCL_INPORT           (uint8*)GPIOBINEN
//#define     SCL_INPORT_NUM       0x04

//data port
//#define     SDA_DATAPORT            (uint8*)GPIOBDAT
//#define     SDA_DATAPORT_NUM        0x05          //B5 outpu data


//#define     SCL_DATAPORT            (uint8*)GPIOBDAT
//#define     SCL_DATAPORT_NUM        0x04         //B4 output data


#define     SDA_OUTPORT          0x00
#define     SDA_OUTPORT_NUM      0x04          //B4 output enalbe
#define     SDA_INPORT           0x01
#define     SDA_INPORT_NUM       0x04

#define     SCL_OUTPORT          0x00
#define     SCL_OUTPORT_NUM      0x05           //B5 output enable
#define     SCL_INPORT           0x01
#define     SCL_INPORT_NUM       0x05

//data port
#define     SDA_DATAPORT            0x02
#define     SDA_DATAPORT_NUM        0x04          //B4 output data
#define     SCL_DATAPORT            0x02
#define     SCL_DATAPORT_NUM        0x05         //B5 output data
extern uint8 qnd_i2c;
extern uint8 qnd_i2c_timeout;
void Set_GPIO(uint8 GPIO_port, uint8 GPIO_port_num);
void Reset_GPIO(uint8 GPIO_port, uint8 GPIO_port_num);
//void FM_Wait50ms(void);
void I2C_Wait5us(void);
void SetMfpToNand(void);


void I2C_Wait5us(void)  //1ms
{
    uint16 i;
    for (i = 10; i > 0; i--)
    {
        ; //FM短暂延时
    }

}

//void FM_Wait50ms(void)
//{
//    uint16 j, k;
//    for (j = 0; j < 50; j++)
//    {
//        ClearWatchDog();
//        for (k = 0; k < 2000; k++)
//        {
//            ; //FM延时
//        }
//    }
//}
void Set_GPIO(uint8 GPIO_port, uint8 GPIO_port_num)
{
    uint8 sfr_bak;

    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    switch (GPIO_port)
    {

        case 0:
        GPIOBOUTEN |= (0x01 << GPIO_port_num);
        break;
        case 1:
        GPIOBINEN |= (0x01 << GPIO_port_num);
        break;
        case 2:
        GPIOBDAT |= (0x01 << GPIO_port_num);
        break;
        default:
        break;
    }
    SFR_BANK = sfr_bak;
}

void Reset_GPIO(uint8 GPIO_port, uint8 GPIO_port_num)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    switch (GPIO_port)
    {

        case 0:
        GPIOBOUTEN &= ~(0x01 << GPIO_port_num);
        break;
        case 1:
        GPIOBINEN &= ~(0x01 << GPIO_port_num);
        break;
        case 2:
        GPIOBDAT &= ~(0x01 << GPIO_port_num);
        break;
        default:
        break;
    }
    SFR_BANK = sfr_bak;
}
void I2C_Enable(void)
{
//	sKY_TouchOnOff(FALSE);

    disable_interrupt();
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
}

void I2C_Disable(void)
{ 
	
    SFR_BANK = sfr_bak;
    enable_interrupt(); 
//	sKY_TouchOnOff(TRUE);
}
void I2C_Init(void)
{
    Set_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    Set_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
    Set_GPIO(SCL_OUTPORT, SCL_OUTPORT_NUM);
    Set_GPIO(SCL_INPORT, SCL_INPORT_NUM);
    Set_GPIO(SDA_INPORT, SDA_INPORT_NUM);
}

void I2C_Exit(void)
{

    Reset_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
    I2C_Wait5us();
    Reset_GPIO(SCL_OUTPORT, SCL_OUTPORT_NUM);
}  
void Send_ACK(uint8 i) //i=0 for ACK and i=1 for Nack
{  
    Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    if (i)
    	Set_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    else
    	Reset_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    I2C_Wait5us();
    Reset_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    Reset_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    I2C_Wait5us();
}
 

void Check_ACK(void)
{
		uint8 sfr_bak;
		Reset_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
    Set_GPIO(SDA_INPORT, SDA_INPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    I2C_Wait5us();
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    qnd_i2c_timeout = (uint8) Read_SDA;
    SFR_BANK = sfr_bak;
    Reset_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    Set_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
   
}

void I2C_Start(void)
{
    Set_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SCL_OUTPORT, SCL_OUTPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    I2C_Wait5us();
    Reset_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    I2C_Wait5us();
    I2C_Wait5us();
    Reset_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
	/*Reset_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    I2C_Wait5us();
    Reset_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);*/

}
void I2C_Stop(void)
{
		Set_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
		I2C_Wait5us();
    Set_GPIO(SCL_OUTPORT, SCL_OUTPORT_NUM);
		I2C_Wait5us();
    Reset_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    I2C_Wait5us();
    I2C_Wait5us();
    Set_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
	/*Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);*/
}

void Write_Bit(uint8 i)
{
    I2C_Wait5us();
    if (i)
    	Set_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    else
    	Reset_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
    I2C_Wait5us();
    Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
    I2C_Wait5us();
    I2C_Wait5us();
    Reset_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
}

void Write_Byte(uint8 Data)
{
    uint8 i;
    for( i = 0;i < 8;i++)
    {
        Write_Bit(Data>>7);
        Data<<=1;
    }
}

uint8 I2C_ReadByte(void)
{
    uint8 Data1=0x00;
    uint8 j;
    uint8 sfr_bak;
    Reset_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
    Set_GPIO(SDA_INPORT, SDA_INPORT_NUM);
    for( j = 0;j < 8;j++)
    {
        I2C_Wait5us();
				Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
				sfr_bak = SFR_BANK;
    		SFR_BANK = BANK_GPIO;
    		Data1 <<= 1;
        Data1 |= ((uint8)Read_SDA)?1:0;
        SFR_BANK = sfr_bak;
        I2C_Wait5us();
    	Reset_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
     }
    Set_GPIO(SDA_OUTPORT, SDA_OUTPORT_NUM);
    
    return Data1;
}
uint8 I2C_Read_1byte(uint8 Slave,uint8 Regis_Addr)
{ 
    uint8 Data=0x00;
    uint8 temp;
    temp =Slave | 0x01;
		disable_interrupt(); //禁止中断
    I2C_Start();
    Write_Byte(Slave);
    Check_ACK();
    if (!qnd_i2c_timeout) 
    {
        Write_Byte(Regis_Addr);
        Check_ACK();
        Set_GPIO(SDA_DATAPORT, SDA_DATAPORT_NUM);
        I2C_Wait5us();
        Set_GPIO(SCL_DATAPORT, SCL_DATAPORT_NUM);
        if(!qnd_i2c_timeout) 
        {
            I2C_Start();
            Write_Byte(temp);
            Check_ACK();
            if (!qnd_i2c_timeout) 
            {
                Data = I2C_ReadByte();
                Send_ACK(1);
            }
        }
    }
    I2C_Stop();
    enable_interrupt(); //开中断
    return Data;
}
uint8 I2C_Write_1byte(uint8 Slave,uint8 Regis_Addr,uint8 Data)
{
    uint8 temp;
    temp=Slave;
	  disable_interrupt(); //禁止中断
    I2C_Start();
    Write_Byte(temp);
    Check_ACK();
    if (!qnd_i2c_timeout) 
    {
        Write_Byte(Regis_Addr);
        Check_ACK();
        if (!qnd_i2c_timeout) 
        {
            Write_Byte(Data);
            Check_ACK();
        }
    }
    I2C_Stop();
    enable_interrupt(); //开中断
    return !qnd_i2c_timeout;
}

uint8 QND_I2C_WRITE(uint8 Regis_Addr,uint8 Data)
{
    uint8 ret;
    uint8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) 
    {
        ret = I2C_Write_1byte(I2C_DEV0_ADDRESS, Regis_Addr, Data);
        if(ret) break;
    }
    if(!tryCount) 
    {
    //SPRINTF(("QND_I2C_WRITE error!\n"));
    		//return RESULT_MAIN;    
        return 0;
    }
    qnd_i2c = 0;
    return 1;
} 

uint8 QND_I2C_READ(uint8 Regis_Addr)
{
    uint8 ret;
    uint8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) {
    ret = I2C_Read_1byte(I2C_DEV0_ADDRESS, Regis_Addr);
        if(!qnd_i2c_timeout) break;
    }
    if(!tryCount) 
    {
    //SPRINTF(("QND_I2C_READ error!\n"));    
    }
    qnd_i2c = 0;
    return ret;
}
uint8 QND_WriteReg(uint8 Regis_Addr,uint8 Data)
{


    return QND_I2C_WRITE(Regis_Addr,Data);

    return 1;
}
  

uint8 QND_ReadReg(uint8 Regis_Addr)
{
    uint8 Data;



    Data = QND_I2C_READ(Regis_Addr);

    return Data;
}


void QNF_SetRegBit(uint8 reg, uint8 bitMask, uint8 data_val) //S_LARGE
{
    uint8 temp;
    temp = QND_ReadReg(reg);
    temp &= (uint8)(~bitMask);
    temp |= data_val & bitMask;
//    temp |= data_val;
    QND_WriteReg(reg, temp);
}


void SetMfpToNand(void)
{

    uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    GPIOBOUTEN &= ~(0x01 << SDA_OUTPORT_NUM);
    GPIOBOUTEN &= ~(0x01 << SCL_OUTPORT_NUM);
    GPIOBINEN &= ~(0x01 << SDA_INPORT_NUM);
    GPIOBINEN &= ~(0x01 << SCL_INPORT_NUM);
    SFR_BANK = sfr_bak;
}
