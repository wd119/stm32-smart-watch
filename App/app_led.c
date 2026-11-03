#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "LED.h"
#include "app_page.h"

// ÷µÁÕ≤

void Show_LED_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_ShowString(20,20,"OFF",OLED_12X24);
    OLED_ShowString(72,20,"ON",OLED_12X24);

}
uint8_t led_flag=1;

static const PageItem LED_Page[]={
    {Show_LED_UI,0,        0, 0,16,16},
    {Show_LED_UI,LED_OFF, 20,20,36,24},
    {Show_LED_UI,LED_ON,  72,20,24,24},
};

int LED(void){
    return Page_Run(LED_Page,3,&led_flag);
}
