#include "stm32f10x.h"
#include "MyRTC.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "app_battery.h"

//首页时间
void Show_Clock_UI(void){
    Show_Batterry();
    MyRTC_GetTime();
    OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
    OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
    OLED_ShowString(0,48,"菜单",OLED_8X16);
    OLED_ShowString(96,48,"设置",OLED_8X16);

}
int clkflag=1;
int First_Page_Clock(void){
    while(1){
        KeyNum1=Key_GetNum();
        if(KeyNum1==1){//上一项
            clkflag--;
            if(clkflag<=0){
                clkflag=2;
            }
        }else if(KeyNum1==2){//下一项
            clkflag++;
            if(clkflag>=3){
                clkflag=1;
            }

    }else if(KeyNum1==3){//确认
            OLED_Clear();
            OLED_Update();
            return clkflag;
        }
        else if(KeyNum1==4){//关机
            GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	        GPIO_SetBits(GPIOB,GPIO_Pin_12);
        }
        switch(clkflag){
            case 1:
                Show_Clock_UI();
                OLED_ReverseArea(0,48,32,16);
                OLED_Update();
                break;
            case 2:
                Show_Clock_UI();
                OLED_ReverseArea(96,48,32,16);
                OLED_Update();
                break;

        }
    }
}
