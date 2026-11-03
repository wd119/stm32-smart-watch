#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "app_settime.h"
#include "app_page.h"

void Show_Setting_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
}
uint8_t setting_flag=1;

static const PageItem Setting_Page_Items[]={
    {Show_Setting_UI,0,       0, 0,16,16},
    {Show_Setting_UI,SetTime, 0,16,96,16},
};

int Setting_Page(void){
    return Page_Run(Setting_Page_Items,2,&setting_flag);
}
