#include "ap_common.h"
#include "actos.h"

#pragma name(AP_STANDBYCHARGE)

uint8 GetCureBatValue(void)
{
    uint8 bat_value = 0;
    uint8 i;
    bat_value = GetBattery();
    for (i = 0; i < 5; i++)
    {
        if (bat_value <= batvalue[i])
        {
            break;
        }
    }
    if (i >= 5)
    {
        i = 4;
    }
    if (i == 0)
    {
        return 0;
    }
    else
    {
        return (i * 2) - 2;
    }

}
uint8 StandbyCharge(void)
{
    uint8 key;
    char bat_value = 0;
    int result;
    uint8 retval;
    uint8 full_flag = FALSE, charging_flag = TRUE;
    uint8 charge_status;
    ClearScreen(NULL);
    ap_message_init(NULL);
    while (ap_get_message() != NULL)
    {

    } //清空消息池
    while (1)
    {
        key = ap_get_message(); //获取消息
        switch (key)
        {
			case AP_KEY_PLAY | AP_KEY_LONG:
				 break;
			case AP_KEY_MODE | AP_KEY_UP:
            //   case AP_KEY_PLAY | AP_KEY_UP:
            retval = GetUsbCableStatus();
            if ((retval & 0x40) != 0)
            {
                result = RESULT_UDISK;
            }
            else
            {
                result = RESULT_MAIN;
            }
            break;
            case AP_MSG_WAIT_TIMEOUT: //8秒退出
            result = RESULT_NULL;
            break;

            case AP_MSG_RTC:
            case AP_MSG_CHARGING:
            retval = GetUsbCableStatus();
            if ((retval & 0x40) != 0)
            {
                result = RESULT_UDISK;
                break;
            }            
            if (retval == 0) //USB cable out，返回主菜单界面
            {
                sKY_ChargeSet(2);
                sKY_ChargeGetandSet(); 
                result = RESULT_MAIN;
                break;
            }
            charge_status = sKY_ChargeSet(0);
            if (charge_status == 0x01)
            {
                if ((full_flag == TRUE) && (charging_flag == FALSE))
                {
                    charging_flag = TRUE;
                    full_flag = FALSE;
            	
                }
              //  ResShowPic((uint16) (1 + bat_value), 12, 52);
                bat_value++;
                if (bat_value > 3)
                {
                    bat_value = 0;
                    //	bat_value=GetCureBatValue();
                }
				 ResShowPic(IM_ADAPTOR_BATT1+bat_value, 0,0); 
            }
            else
            {
                if ((charge_status == 0x02) && (full_flag == FALSE))
                {
                    full_flag = TRUE;
                    charging_flag = FALSE;
                	ResShowPic(IM_ADAPTOR_BATT4, 0,0); 
                }
                else if (charge_status == 0x0)
                {
                    ui_err_msg(NOBATTER);
                    result = RESULT_MAIN;
                    break;
                }
                else
                {
                }
            }
            result = RESULT_NULL;
            break;
            default:
            result = ap_handle_hotkey(key);
            if (result == RESULT_REDRAW)
            {
              
                ClearScreen(NULL);
                result = RESULT_NULL;
                break;
            }
            else if ((result != 0) && (result != RESULT_STANDBY))
            {
                break;
            }
            else
            {
                break;
            }
        } /*end of swith */
        if (result != RESULT_NULL)
        {
            break;
        }
    } /* end of while(1) */
    return result;
}
