#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_ui.h"
#include "Delay.h"

//------------------------¶¯Ì¬±íÇé°ü-----------------------------------
void Show_Emoji_UI(void){
    for(uint8_t i=0;i<3;i++){
        //±ÕÑÛ
        OLED_Clear();
        OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);//×óÃ¼Ã«
        OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);//ÓÒÃ¼Ã«
        OLED_DrawEllipse(40,32,6,6-i,1);//×óÑÛ
        OLED_DrawEllipse(88,32,6,6-i,1);//ÓÒÑÛ
        OLED_ShowImage(54,40,20,20,Mouth);
        OLED_Update();
        Delay_ms(100);

}
//ÕöÑÛ
 for(uint8_t i=0;i<3;i++){
        
        OLED_Clear();
        OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);//×óÃ¼Ã«
        OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);//ÓÒÃ¼Ã«
        OLED_DrawEllipse(40,32,6,4+i,1);//×óÑÛ
        OLED_DrawEllipse(88,32,6,4+i,1);//ÓÒÑÛ
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
