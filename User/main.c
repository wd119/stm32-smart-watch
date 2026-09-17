#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "menu.h"
#include"Timer.h"
#include"key.h"
#include"dno.h"
// Device header
int main(void){
	OLED_Init();
	OLED_Clear();
	Periapheral_Init();
	Timer_Init();
	int clkflag1;
	extern int press_time;
	extern uint8_t KeyNum;
	while(1){
		// OLED_ShowNum(64,0,press_time,4,OLED_6X8);
		// OLED_ShowNum(64,8,KeyNum,1,OLED_6X8);
		// OLED_Update();
		clkflag1=First_Page_Clock();
		if(clkflag1==1){
			Menu();
		}else if(clkflag1==2){
			Setting_Page();
	}
	
	}
}
void TIM2_IRQHandler(void){
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		Key_Tick();
		StopWatch_Tick();
		Dino_Tick();
		Key3_Tick();
           TIM_ClearITPendingBit(TIM2,TIM_IT_Update); // 
    }
}