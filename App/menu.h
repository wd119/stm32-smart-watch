#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x.h"

extern uint8_t KeyNum1;

void Show_Clock_UI(void);
int First_Page_Clock(void);
int Setting_Page(void);
int LED(void);
int Game(void);
int Emoji(void);
#endif // !__MENU_H
