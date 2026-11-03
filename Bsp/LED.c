#include "stm32f10x.h"                  
//手电筒模块的底层驱动

/**
  * 函    数：初始化GPIO
  * 参    数：无
  * 返 回 值：无
  * 说    明：初始化了Pin15为手电筒，Pin12初始化为低电平，开启ADC电源电压检测电路，Pin13初始化为高电平，使得Key3松手后仍可以持续供电
  */
void LED_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15 | GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}
/**
  * 函    数：点亮LED
  * 参    数：无
  * 返 回 值：无
  */
void LED_ON(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
/**
  * 函    数：熄灭LED
  * 参    数：无
  * 返 回 值：无
  */
void LED_OFF(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

