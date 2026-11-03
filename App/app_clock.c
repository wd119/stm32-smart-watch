#include "stm32f10x.h"
#include "MyRTC.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "app_battery.h"
#include "app_page.h"

//首页时间
void Show_Clock_UI(void){
    Show_Batterry();
    MyRTC_GetTime();
    OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
    OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
    OLED_ShowString(0,48,"菜单",OLED_8X16);
    OLED_ShowString(96,48,"设置",OLED_8X16);

}
uint8_t clkflag=1;

static const PageItem Clock_Page[]={
    {Show_Clock_UI,0,  0,48,32,16},
    {Show_Clock_UI,0, 96,48,32,16},
};

int First_Page_Clock(void){
    return Page_Run(Clock_Page,2,&clkflag);
}
