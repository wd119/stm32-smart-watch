#include"stm32f10x.h"
#include"MyRTC.h"
#include"OLED.h"
#include"key.h"
#include"app_ui.h"
#include"app_page.h"

void Show_SetTime_FirstUI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_Printf(0,16,OLED_8X16,"年:%4d",MyRTC_Time[0]);
    OLED_Printf(0,32,OLED_8X16,"月:%2d",MyRTC_Time[1]);
    OLED_Printf(0,48,OLED_8X16,"日:%2d",MyRTC_Time[2]);
}
void Show_SetTime_SecondUI(void){
    OLED_Printf(0,0,OLED_8X16,"时:%2d",MyRTC_Time[3]);
    OLED_Printf(0,16,OLED_8X16,"分:%2d",MyRTC_Time[4]);
    OLED_Printf(0,32,OLED_8X16,"秒:%2d",MyRTC_Time[5]);
}

static void Show_SetTime_FirstUI_Clear(void){
    OLED_Clear();
    Show_SetTime_FirstUI();
}
static void Show_SetTime_SecondUI_Clear(void){
    OLED_Clear();
    Show_SetTime_SecondUI();
}

static const AdjustItem SetTime_Adjust[]={
    {&MyRTC_Time[0],2000,2037,Show_SetTime_FirstUI_Clear, MyRTC_SetTime,24,16,32,16},
    {&MyRTC_Time[1],   1,  12,Show_SetTime_FirstUI_Clear, MyRTC_SetTime,24,32,16,16},
    {&MyRTC_Time[2],   1,  31,Show_SetTime_FirstUI_Clear, MyRTC_SetTime,24,48,16,16},
    {&MyRTC_Time[3],   0,  23,Show_SetTime_SecondUI_Clear,MyRTC_SetTime,24, 0,16,16},
    {&MyRTC_Time[4],   0,  59,Show_SetTime_SecondUI_Clear,MyRTC_SetTime,24,16,16,16},
    {&MyRTC_Time[5],   0,  59,Show_SetTime_SecondUI_Clear,MyRTC_SetTime,24,32,16,16},
};

static void SetYear  (void){ Page_Adjust(&SetTime_Adjust[0]); }
static void SetMonth (void){ Page_Adjust(&SetTime_Adjust[1]); }
static void SetDay   (void){ Page_Adjust(&SetTime_Adjust[2]); }
static void SetHour  (void){ Page_Adjust(&SetTime_Adjust[3]); }
static void SetMinute(void){ Page_Adjust(&SetTime_Adjust[4]); }
static void SetSecond(void){ Page_Adjust(&SetTime_Adjust[5]); }

uint8_t settime_flag=1;

static const PageItem SetTime_Page[]={
    {Show_SetTime_FirstUI_Clear,  0,         0, 0,16,16},
    {Show_SetTime_FirstUI_Clear,  SetYear,   0,16,16,16},
    {Show_SetTime_FirstUI_Clear,  SetMonth,  0,32,16,16},
    {Show_SetTime_FirstUI_Clear,  SetDay,    0,48,16,16},
    {Show_SetTime_SecondUI_Clear, SetHour,   0, 0,16,16},
    {Show_SetTime_SecondUI_Clear, SetMinute, 0,16,16,16},
    {Show_SetTime_SecondUI_Clear, SetSecond, 0,32,16,16},
};

int SetTime(void){
    return Page_Run(SetTime_Page,7,&settime_flag);
}