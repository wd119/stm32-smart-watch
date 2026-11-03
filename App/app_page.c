#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "app_page.h"

void Power_Off(void){
    GPIO_ResetBits(GPIOB,GPIO_Pin_13);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
    while(1);
}

static void Page_Draw(const PageItem *it){
    if(it->Draw) it->Draw();
    if(it->rw) OLED_ReverseArea(it->rx,it->ry,it->rw,it->rh);
    OLED_Update();
}

uint8_t Page_Run(const PageItem *items,uint8_t count,uint8_t *sel){
    uint8_t ok=0;
    if(*sel<1||*sel>count) *sel=1;
    while(1){
        uint8_t k=Key_GetNum();
        if(k==PAGE_KEY_POWER){
            Power_Off();
        }else if(k==PAGE_KEY_UP){
            *sel=(*sel<=1)?count:(uint8_t)(*sel-1);
        }else if(k==PAGE_KEY_DOWN){
            *sel=(*sel>=count)?1:(uint8_t)(*sel+1);
        }else if(k==PAGE_KEY_OK){
            OLED_Clear();
            OLED_Update();
            ok=*sel;
        }
        if(ok){
            const PageItem *it=&items[ok-1];
            if(it->Action==0) return ok;
            it->Action();
            ok=0;
        }
        Page_Draw(&items[*sel-1]);
    }
}

void Page_Loop(void (*Body)(void)){
    while(1){
        uint8_t k=Key_GetNum();
        if(k==PAGE_KEY_POWER){
            Power_Off();
        }else if(k==PAGE_KEY_OK){
            OLED_Clear();
            OLED_Update();
            return;
        }
        if(Body) Body();
    }
}

void Page_Adjust(const AdjustItem *it){
    while(1){
        uint8_t k=Key_GetNum();
        if(k==PAGE_KEY_POWER){
            Power_Off();
        }else if(k==PAGE_KEY_UP){
            if(++(*it->value)>it->hi) *it->value=it->lo;
            if(it->OnChange) it->OnChange();
        }else if(k==PAGE_KEY_DOWN){
            if(--(*it->value)<it->lo) *it->value=it->hi;
            if(it->OnChange) it->OnChange();
        }else if(k==PAGE_KEY_OK){
            return;
        }
        it->Draw();
        if(it->rw) OLED_ReverseArea(it->rx,it->ry,it->rw,it->rh);
        OLED_Update();
    }
}
