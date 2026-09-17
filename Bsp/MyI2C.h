#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);
//起始条件
void MyI2C_Start(void);
//终止条件
void MyI2C_End(void);
//发送一个字节
void MyI2C_SendByte(uint8_t Byte);
//接收一个字节
uint8_t MyI2C_RcByte(void);
//发送响应
void MyI2C_SendAck(uint8_t AckBit);
//接收响应
uint8_t MyI2C_RcAck(void);

#endif