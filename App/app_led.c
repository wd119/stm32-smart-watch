#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "LED.h"

//手电筒

void Show_LED_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_ShowString(20,20,"OFF",OLED_12X24);
    OLED_ShowString(72,20,"ON",OLED_12X24);

}
uint8_t led_flag=1;
int LED(void){
    while(1){
        KeyNum1=Key_GetNum();
        uint8_t led_flag_temp=0;
        if(KeyNum1==1){//上一项
            led_flag--;
            if(led_flag<=0){
                led_flag=3;
            }
        }else if(KeyNum1==2){//下一项
            led_flag++;
            if(led_flag>=4){
                led_flag=1;
            }

    }else if(KeyNum1==3){//确认
            OLED_Clear();
            OLED_Update();
            led_flag_temp=led_flag;
        }
        if(led_flag_temp==1){
            return 0;
        }
        switch(led_flag){
            case 1:
                Show_LED_UI();
                OLED_ReverseArea(0,0,16,16);
                OLED_Update();
                break;
            case 2:
                Show_LED_UI();
                LED_OFF();
                OLED_ReverseArea(20,20,36,24);
                OLED_Update();
                break;
            case 3:
                Show_LED_UI();
                LED_ON();
                OLED_ReverseArea(72,20,24,24);
                OLED_Update();
                break;
           

        }
    }
}
