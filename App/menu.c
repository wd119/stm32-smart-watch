#include "stm32f10x.h"
#include "MyRTC.h"
#include"OLED.h"
#include"key.h"
#include"LED.h"
#include"SetTime.h"
#include"menu.h"
#include"Delay.h"
#include"dno.h"
#include"AD.h"
uint8_t KeyNum1;

uint16_t ADValue;
float VBAT;
int Battery_Capacity;

void Show_Batterry(void){
    int sum=0;
    for(int i=0;i<3000;i++){

        ADValue=AD_GetValue();
        sum+=ADValue;
    }
    ADValue=sum/3000;
    
    VBAT=(float)ADValue/4095*3.3;
    Battery_Capacity=(ADValue-3276)*100/815;
    if(Battery_Capacity<0) Battery_Capacity=0;
   // OLED_ShowNum(64,0,ADValue,4,OLED_6X8);


    OLED_ShowNum(85,4,Battery_Capacity,3,OLED_6X8);
    OLED_ShowChar(103,4,'%',OLED_6X8);
    if(Battery_Capacity==100) OLED_ShowImage(110,0,16,16,Battery);
    else if(Battery_Capacity>=10&&Battery_Capacity<100){
        OLED_ShowImage(110,0,16,16,Battery);
        OLED_ClearArea((112+Battery_Capacity/10),5,(10-Battery_Capacity/10),6);
        OLED_ClearArea(85,4,6,8);

    }else{
        OLED_ShowImage(110,0,16,16,Battery);
        OLED_ClearArea(112,5,10,6);
        OLED_ClearArea(85,4,12,8);
    }
}
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
//游戏
void Show_Game_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_ShowString(0,16,"谷歌小恐龙",OLED_8X16);

}
uint8_t game_flag=1;
int Game(void){
    while(1){
        KeyNum1=Key_GetNum();
        uint8_t game_flag_temp=0;
        if(KeyNum1==1){//上一项
            game_flag--;
            if(game_flag<=0){
                game_flag=2;
            }
        }else if(KeyNum1==2){//下一项
            game_flag++;
            if(game_flag>=3){
                game_flag=1;
            }

    }else if(KeyNum1==3){//确认
            OLED_Clear();
            OLED_Update();
           game_flag_temp=game_flag;
        }
        if(game_flag_temp==1){
            return 0;
        }
        else if(game_flag_temp==2){
            DinoGame_Pos_Init();
            DinoGame_Animation();
        }
        switch(game_flag){
            case 1:
                Show_Game_UI();
                OLED_ReverseArea(0,0,16,16);
                OLED_Update();
                break;
            case 2:
                Show_Game_UI();
                LED_OFF();
                OLED_ReverseArea(0,16,80,16);
                OLED_Update();
                break;
           

        }
    }
}
//------------------------动态表情包-----------------------------------
void Show_Emoji_UI(void){
    for(uint8_t i=0;i<3;i++){
        //闭眼
        OLED_Clear();
        OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);//左眉毛
        OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);//右眉毛
        OLED_DrawEllipse(40,32,6,6-i,1);//左眼
        OLED_DrawEllipse(88,32,6,6-i,1);//右眼
        OLED_ShowImage(54,40,20,20,Mouth);
        OLED_Update();
        Delay_ms(100);

}
//睁眼
 for(uint8_t i=0;i<3;i++){
        
        OLED_Clear();
        OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);//左眉毛
        OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);//右眉毛
        OLED_DrawEllipse(40,32,6,4+i,1);//左眼
        OLED_DrawEllipse(88,32,6,4+i,1);//右眼
        OLED_ShowImage(54,40,20,20,Mouth);
        OLED_Update();
        Delay_ms(100);

}
}

int Emoji(void){
    while(1){
        KeyNum1=Key_GetNum();
        if(KeyNum1==3){
            OLED_Clear();
            OLED_Update();
            return 0;
        }
        Show_Emoji_UI();
    }
}