#ifndef __APP_PAGE_H
#define __APP_PAGE_H

#include "stm32f10x.h"

#define PAGE_KEY_UP     1
#define PAGE_KEY_DOWN   2
#define PAGE_KEY_OK     3
#define PAGE_KEY_POWER  4

typedef struct {
    void    (*Draw)(void); //函数指针 UI
    void    (*Action)(void);//行为
    int16_t  rx, ry;//反显矩阵的位置和大小
    uint8_t  rw, rh;
} PageItem;

typedef struct {
    int    *value;
    int     lo, hi;
    void  (*Draw)(void);
    void  (*OnChange)(void);
    int16_t  rx, ry;
    uint8_t  rw, rh;
} AdjustItem;

uint8_t Page_Run(const PageItem *items, uint8_t count, uint8_t *sel);
void    Page_Loop(void (*Body)(void));
void    Page_Adjust(const AdjustItem *item);
void    Power_Off(void);

#endif
