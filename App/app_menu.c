#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "menu.h"
#include "app_stopwatch.h"
#include "app_attitude.h"

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
int Menu(void){
    move_flag=1;
    uint8_t DirectFlag=2;//置1移动到上一项 2 下一项

     while(1){
        KeyNum1=Key_GetNum();
        uint8_t menu_flag_temp=0;
        if(KeyNum1==1){//上一项
            DirectFlag=1;
            move_flag=1;
            menu_flag--;
            if(menu_flag<=0){
                menu_flag=7;
            }
        }else if(KeyNum1==2){//下一项
            DirectFlag=2;
            move_flag=1;
            menu_flag++;
            if(menu_flag>=8){
                menu_flag=1;
            }

    }else if(KeyNum1==3){//确认
            OLED_Clear();
            OLED_Update();
            menu_flag_temp=menu_flag;
        }
        if(menu_flag_temp==1){
            return 0;
        }else if(menu_flag_temp==2){
            MenuTOFunction();
           StopWatch();
        }else if(menu_flag_temp==3){
            MenuTOFunction();
           
           LED();
        }else if(menu_flag_temp==4){
             MenuTOFunction();
           MPU6050();
        }else if(menu_flag_temp==5){
           MenuTOFunction();
           Game();
        }else if(menu_flag_temp==6){
           MenuTOFunction();
           Emoji();
        }else if(menu_flag_temp==7){
           MenuTOFunction();
           Gradienter();
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
