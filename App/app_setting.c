#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "app_settime.h"

void Show_Setting_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
}
int setflag=1;
int Setting_Page(void){
    while(1){
        KeyNum1=Key_GetNum();
        uint8_t setflag_temp=0;
        if(KeyNum1==1){//上一项
            setflag--;
            if(setflag<=0){
                setflag=2;
            }
        }else if(KeyNum1==2){//下一项
            setflag++;
            if(setflag>=3){
                setflag=1;
            }

    }else if(KeyNum1==3){//确认
            OLED_Clear();
            OLED_Update();
            setflag_temp=setflag;
        }
        if(setflag_temp==1){
            return 0;
        }else if(setflag_temp==2){
            SetTime();
        }
        switch(setflag){
            case 1:
                Show_Setting_UI();
                OLED_ReverseArea(0,0,16,16);
                OLED_Update();
                break;
            case 2:
                Show_Setting_UI();
                OLED_ReverseArea(0,16,96,16);
OLED_Update();
                break;

        }
    }
}
