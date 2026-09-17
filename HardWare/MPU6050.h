#ifndef __MPU6050_H
#define __MPU6050_H

#include "MPU6050_Reg.h"

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);
uint8_t MPU_r(uint8_t RegAdd);
void MPU_Init(void);
void MPU6050_GetData(int16_t *Acx,int16_t *Acy,int16_t*Acz,int16_t *Gyx,int16_t *Gyy,int16_t*Gyz);
#endif