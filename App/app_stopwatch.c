#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "app_page.h"

//秒表
uint8_t hour,min,sec;
void Show_StopWatch_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_Printf(32,20,OLED_8X16,"%02d:%02d:%02d",hour,min,sec);
    OLED_ShowString(8,44,"开始",OLED_8X16);
    OLED_ShowString(48,44,"停止",OLED_8X16);
    OLED_ShowString(88,44,"清除",OLED_8X16);

}
uint8_t start_timing_flag;//1:开始0：停止
void StopWatch_Tick(void){
    static uint16_t Count;
    Count++;
    if(Count>=1000){
        Count=0;
         if(start_timing_flag==1){
        sec++;
        if(sec>=60){
            sec=0;
            min++;
        }
        if(min>=60){
            min=0;
            hour++;
        }if(hour>99) hour=0;
    }
    }
   
}

static void SW_Start(void){ start_timing_flag=1; }
static void SW_Stop (void){ start_timing_flag=0; }
static void SW_Reset(void){ start_timing_flag=0; hour=min=sec=0; }

uint8_t stopwatch_flag=1;

static const PageItem StopWatch_Page[]={
    {Show_StopWatch_UI,0,         0, 0,16,16},
    {Show_StopWatch_UI,SW_Start,  8,44,32,16},
    {Show_StopWatch_UI,SW_Stop,  48,44,32,16},
    {Show_StopWatch_UI,SW_Reset, 88,44,32,16},
};

int StopWatch(void){
    return Page_Run(StopWatch_Page,4,&stopwatch_flag);
}
