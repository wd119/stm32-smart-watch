#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "LED.h"
#include "app_dino.h"

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
