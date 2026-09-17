#include "stm32f10x.h"
#include "MyRTC.h"
#include "key.h"
#include "LED.h"
#include "MPU6050.h"
#include "AD.h"

void Periapheral_Init(void){
    MyRTC_Init();
    Key_Init();
    LED_Init();
    MPU_Init();
    AD_Init();
}
