#include "stm32f10x.h"
void Timer_Init(void){
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // Enable TIM2 clock

       TIM_InternalClockConfig(TIM2); // Configure TIM2 to use the internal clock
       TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
       TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
       TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period=100-1; // Set the period for 1s interrupt
    TIM_TimeBaseStructure.TIM_Prescaler=720-1; // Set the prescaler for 1MHz timer clock
        TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
        TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); // Initialize TIM2 with the specified parameters
    TIM_ClearFlag(TIM2,TIM_FLAG_Update); // Clear the update flag

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); // Enable TIM2 update interrupt
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // Set NVIC priority grouping
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; // Set the IRQ channel for TIM2
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; // Enable the IRQ channel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; // Set preemption priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStructure); // Initialize NVIC with the specified parameters

    TIM_Cmd(TIM2,ENABLE); // Start TIM2

}
