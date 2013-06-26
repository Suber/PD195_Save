/*
*******************************************************************************
*                ACTOS AP
*        ap_setting header file
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: ap_setting.h,v 1.1.1.1 2008/01/11 07:28:03 rowena Exp $
*******************************************************************************
*/
#ifndef _AP_SETTING_H
#define _AP_SETTING_H

#define up_settingamflag       0x0800 
#define up_settingdayflag      0x0400
#define up_settingmonthflag    0x0200
#define up_settingyearflag     0x0100
#define up_settingminlowflag   0x0080
#define up_settingminhighflag  0x0040
#define up_settinghourlowflag  0x0020
#define up_settinghourhighflag 0x0001


typedef struct
{
    WORD magic;
}setting_vars_t;

//global var
extern setting_vars_t g_setting_vars;
extern comval_t g_comval;

//all controls use this
extern slider_t slider;
extern function_menu_t menu1;
extern progress_t progress;
extern pop_up_list_t list;
extern pop_up_list_t main_list;
extern const uint16 main_text[];
extern const uint16 main_text_pedo[];   //pedometer
extern function_menu_t menu;

extern uint16 UpDisplayFlag;//longjin 2010.12.29
//funcs
uint16 show_clock(void);
uint16 show_rectype(void);  //add by ccm
uint16 show_backlight(void);
uint16 show_blight_color(void);//add by ccm
uint16 show_language(void);
uint16 show_powerdown(void);
uint16 show_display(void);
uint16 show_lock(void);
uint16 show_replay(void);
uint16 show_contrast(void);
uint16 show_memory(void);
uint16 show_firmware(void);
uint16 show_record(void);//¬º“Ù…Ë÷√
uint16 system_tool(void);
//uint16 show_online(void);
//uint16 show_clockdrm(void);
//uint16 show_onlined(void);
uint16 screen_set(void); 
uint16 show_charge(void);
uint16 show_fmttype(void);
//uint16 show_keytone(void);
//uint16 ear_protect(void);
uint16 show_removehds(void);
//uint16 ear_protect_param_adjust(void);
//uint16 do_format(void);
//uint16 show_keytone(void);
uint16 show_unit(void);
uint16 show_gusensou(void);
//void keytone_callback(uint16 value);
extern void RemoveHds(void); 


#endif  /* _AP_SETTING_H*/
