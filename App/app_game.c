#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "LED.h"
#include "app_dino.h"
#include "app_page.h"

//”Œœ∑
void Show_Game_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_ShowString(0,16,"π»∏Ë–°ø÷¡˙",OLED_8X16);

}
uint8_t game_flag=1;

static void Game_Enter(void){
    DinoGame_Pos_Init();
    DinoGame_Animation();
    LED_OFF();
}

static const PageItem Game_Page[]={
    {Show_Game_UI,0,          0, 0,16,16},
    {Show_Game_UI,Game_Enter, 0,16,80,16},
};

int Game(void){
    return Page_Run(Game_Page,2,&game_flag);
}
