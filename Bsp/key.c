#include "stm32f10x.h"      
#include "Delay.h"
/*
	按键驱动层
*/
//全局变量 存所按键的编号 中断函数里只写
uint8_t KeyNum;
//初始化按键
void Key_Init(void){
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
/**
  * 函    数：读取刚刚发生了一次的按键
  * 参    数：无
  * 返 回 值：无
  * 说    明：读取完KeyNum清零
  */
uint8_t Key_GetNum(void){
	uint8_t Temp=KeyNum;
	KeyNum=0;
	return Temp;
}
/**
  * 函    数：检测是否长按Key3
  * 参    数：无
  * 返 回 值：无
  * 说    明：定时器中断中每一ms检测Pin4电平，将低电平的持续时间写入press_time
  */
int press_time;
void Key3_Tick(void){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0){
		press_time++;
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1){
		press_time=0;
	}
}
/**
  * 函    数：检测引脚当前电平
  * 参    数：无
  * 返 回 值：没按返回0，按了返回所按的按钮编号
  * 说    明：返回1：Key1按下，返回2：Key2按下，返回3：Key3按下，返回4：长按Key3（1s以上）
  */
uint8_t Key_GetState(void){
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
		return 1;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0){
		return 2;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0&&press_time>1000){
		return 4;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0){
		return 3;
	}

	else{
		return 0;
	}
	
}
/**
  * 函    数：按键扫描
  * 参    数：无
  * 返 回 值：无
  * 说    明：由 TIM2 中断每 1ms 调用一次。
  *           内部每 20ms 才真正采一次样（count 每数到 20 就清零）。
  *           用前后两次采样的下降沿判定"松手"：
  *           上一拍有键(PreState!=0)、这一拍没键(CurrentKeyNum==0)，
  *           就把键号写进全局 KeyNum，供主循环 Key_GetNum() 取走。
  *           本函数只写不清零，清零是 Key_GetNum() 的事。
  */
void Key_Tick(void){
    static uint8_t count;
    static uint8_t CurrentKeyNum,PreState;

    count++;
    if(count>=20){
        count=0;
        PreState=CurrentKeyNum;
        CurrentKeyNum=Key_GetState();
		if(PreState!=0&&CurrentKeyNum==0){
			KeyNum=PreState;
    }
}
}