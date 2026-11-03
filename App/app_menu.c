#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "app_led.h"
#include "app_game.h"
#include "app_emoji.h"
#include "app_stopwatch.h"
#include "app_attitude.h"
#include "app_page.h"

//滑动菜单界面
uint8_t pre_selection;//上次选择的选项

uint8_t target_selection;//目标选项
uint8_t x_pre=48;//上次选项的x坐标 
uint8_t Speed=4;//速度
uint8_t move_flag;//开始移动的标志位 1表示开始移动 0表示停止移动

void Menu_Animation(void){
    OLED_Clear();
    OLED_ShowImage(42,10,44,44,Frame);
    if(pre_selection<target_selection){
        x_pre-=Speed;
        if(x_pre==0){
        pre_selection++;
        move_flag=0;
        x_pre=48;
        }
    }
    if(pre_selection>target_selection){
        x_pre+=Speed;
        if(x_pre==96){
        pre_selection--;
        move_flag=0;
        x_pre=48;
        }
    }
    if(pre_selection>=1){
    OLED_ShowImage(x_pre-48,16,32,32,Menu_Graph[pre_selection-1]);
    }
     if(pre_selection>=2){
    OLED_ShowImage(x_pre-96,16,32,32,Menu_Graph[pre_selection-2]);
    }
    OLED_ShowImage(x_pre,16,32,32,Menu_Graph[pre_selection]);
    OLED_ShowImage(x_pre+48,16,32,32,Menu_Graph[pre_selection+1]);
    OLED_ShowImage(x_pre+96,16,32,32,Menu_Graph[pre_selection+2]);
    OLED_Update();
}
void Set_Selection(uint8_t move_flag, uint8_t Pre_selection,uint8_t Target_selection){
    if(move_flag==1){
        pre_selection=Pre_selection;
        target_selection=Target_selection;
        
    }
    Menu_Animation();
} 
void MenuTOFunction(void){
    for(uint8_t i=0;i<=6;i++){
        OLED_Clear();
if(pre_selection>=1){
    OLED_ShowImage(x_pre-48,16+8*i,32,32,Menu_Graph[pre_selection-1]);
    }
  
    OLED_ShowImage(x_pre,16+8*i,32,32,Menu_Graph[pre_selection]);
    OLED_ShowImage(x_pre+48,16+8*i,32,32,Menu_Graph[pre_selection+1]);
    
    OLED_Update();  
    }
    
}
uint8_t menu_flag=1;

static void Menu_To_StopWatch (void){ MenuTOFunction(); StopWatch();  }
static void Menu_To_LED       (void){ MenuTOFunction(); LED();        }
static void Menu_To_MPU6050   (void){ MenuTOFunction(); MPU6050();    }
static void Menu_To_Game      (void){ MenuTOFunction(); Game();       }
static void Menu_To_Emoji     (void){ MenuTOFunction(); Emoji();      }
static void Menu_To_Gradienter(void){ MenuTOFunction(); Gradienter(); }

static void (*const Menu_Enter[8])(void)={
    0,
    0,                    /* 1 : exit to main loop */
    Menu_To_StopWatch,    /* 2 */
    Menu_To_LED,          /* 3 */
    Menu_To_MPU6050,      /* 4 */
    Menu_To_Game,         /* 5 */
    Menu_To_Emoji,        /* 6 */
    Menu_To_Gradienter,   /* 7 */
};

int Menu(void){
    move_flag=1;
    uint8_t DirectFlag=2;

     while(1){
        uint8_t key=Key_GetNum();
        uint8_t menu_flag_temp=0;
        if(key==PAGE_KEY_POWER){
            Power_Off();
        }else if(key==PAGE_KEY_UP){
            DirectFlag=1;
            move_flag=1;
            if(menu_flag<=1){
                menu_flag=7;
            }else{
                menu_flag--;
            }
        }else if(key==PAGE_KEY_DOWN){
            DirectFlag=2;
            move_flag=1;
            if(menu_flag>=7){
                menu_flag=1;
            }else{
                menu_flag++;
            }
        }else if(key==PAGE_KEY_OK){
            OLED_Clear();
            OLED_Update();
            menu_flag_temp=menu_flag;
        }
        if(menu_flag_temp==1){
            return 0;
        }else if(Menu_Enter[menu_flag_temp]){
            Menu_Enter[menu_flag_temp]();
        }

    if(menu_flag==1){
         if(DirectFlag==1){
                    Set_Selection(move_flag,1,0);
                }else if(DirectFlag==2){
                    Set_Selection(move_flag,0,0);
                }
    }else{
         if(DirectFlag==1){
                    Set_Selection(move_flag,menu_flag,menu_flag-1);
                }else if(DirectFlag==2){
                    Set_Selection(move_flag,menu_flag-2,menu_flag-1);
                }
    }
}
}
