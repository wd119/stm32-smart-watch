#include"stm32f10x.h"
#include"Delay.h"

void MyI2C_W_scl(uint8_t  BitValue){
    GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
    Delay_us(10);
}

void MyI2C_W_sda(uint8_t  BitValue){
    GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
    Delay_us(10);
}

uint8_t MyI2C_R_sda(void){
    uint8_t BitValue;
    BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
    Delay_us(10);
    return BitValue;
}

void MyI2C_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 |GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_SetBits(GPIOB,GPIO_Pin_10 |GPIO_Pin_11);
}
//起始条件
void MyI2C_Start(void){
    MyI2C_W_sda(1);
    MyI2C_W_scl(1);
    MyI2C_W_sda(0);
    MyI2C_W_scl(0);
}
//终止条件
void MyI2C_End(void){
    MyI2C_W_sda(0);
    MyI2C_W_scl(1);
    MyI2C_W_sda(0);
}
//发送一个字节
void MyI2C_SendByte(uint8_t Byte){
    uint8_t i;
    for(i=0;i<8;i++){
        MyI2C_W_sda(Byte& (0x80>>i));
        MyI2C_W_scl(1);
        MyI2C_W_scl(0);
    }
}
//接收一个字节
uint8_t MyI2C_RcByte(void){
    uint8_t i,Byte=0x00;
    MyI2C_W_sda(1);
    for(i=0;i<8;i++){
    MyI2C_W_scl(1);
    Byte = Byte << 1;
    if(MyI2C_R_sda()==1){
        Byte|=0x01;
    }
    MyI2C_W_scl(0);
}
return Byte;
}
//发送响应
void MyI2C_SendAck(uint8_t AckBit){
        MyI2C_W_sda(AckBit);
        MyI2C_W_scl(1);
        MyI2C_W_scl(0);
}
//接收响应
uint8_t MyI2C_RcAck(void){
    uint8_t AckBit;
    MyI2C_W_sda(1);
    MyI2C_W_scl(1);
    AckBit=MyI2C_R_sda();
    MyI2C_W_scl(0);
return AckBit;
}
