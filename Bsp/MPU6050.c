#include"stm32f10x.h"
#include"MyI2C.h"
#include"MPU6050_Reg.h"

#define MPU6050_add  0xD0
//向从机指定地址写一个字�?
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data){
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_add);
    MyI2C_RcAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_RcAck();
    MyI2C_SendByte(Data);
    MyI2C_RcAck();
    MyI2C_End();
}

uint8_t MPU_r(uint8_t RegAdd){
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_add);
    MyI2C_RcAck();
    MyI2C_SendByte(RegAdd);
    MyI2C_RcAck();
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_add| 0x01);
    MyI2C_RcAck();
    uint8_t data=MyI2C_RcByte();
    MyI2C_SendAck(1);
    MyI2C_End();
    return data;
}
void MPU_Init(void){
    MyI2C_Init();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0X01);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0X00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0X04);
    MPU6050_WriteReg(MPU6050_CONFIG,0X06);
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0X18);
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0X18);
}
void MPU6050_GetData(int16_t *Acx,int16_t *Acy,int16_t*Acz,int16_t *Gyx,int16_t *Gyy,int16_t*Gyz){
    uint8_t DataH,DataL;
    DataH=MPU_r(MPU6050_ACCEL_XOUT_H);
    DataL=MPU_r(MPU6050_ACCEL_XOUT_L);
    *Acx=(DataH<<8) | DataL;
    DataH=MPU_r(MPU6050_ACCEL_YOUT_H);
    DataL=MPU_r(MPU6050_ACCEL_YOUT_L);
    *Acy=(DataH<<8) | DataL;
    DataH=MPU_r(MPU6050_ACCEL_ZOUT_H);
    DataL=MPU_r(MPU6050_ACCEL_ZOUT_L);
    *Acz=(DataH<<8) | DataL;
    DataH=MPU_r(MPU6050_GYRO_XOUT_H);
    DataL=MPU_r(MPU6050_GYRO_XOUT_L);
    *Gyx=(DataH<<8) | DataL;

    DataH=MPU_r(MPU6050_GYRO_YOUT_H);
    DataL=MPU_r(MPU6050_GYRO_YOUT_L);
    *Gyy=(DataH<<8) | DataL;

    DataH=MPU_r(MPU6050_GYRO_ZOUT_H);
    DataL=MPU_r(MPU6050_GYRO_ZOUT_L);
    *Gyz=(DataH<<8) | DataL;
}
