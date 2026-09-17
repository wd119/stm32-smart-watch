#ifndef __MPU6050_REG_H
#define __MPU6050_REG_H

/* ============ MPU6050 寄存器地址定义 ============ */
/* 寄存器地址均为 8 位，读写时先发地址再收发数据 */

/* ---------------- 配置类寄存器 ---------------- */
#define 	MPU6050_SMPLRT_DIV		0x19	/*采样率分频器：采样率 = 陀螺仪输出率 / (1 + 分频值)*/
#define 	MPU6050_CONFIG			0x1A	/*配置寄存器：外部帧同步(FSYNC)与数字低通滤波器(DLPF)设置*/
#define 	MPU6050_GYRO_CONFIG		0x1B	/*陀螺仪配置：自检 + 满量程范围，可设 ±250/500/1000/2000 °/s*/
#define 	MPU6050_ACCEL_CONFIG	0x1C	/*加速度计配置：自检 + 满量程范围，可设 ±2/4/8/16 g*/

/* ---------- 数据输出寄存器（均为16位有符号数，高字节在前） ---------- */
/* 这14个字节地址连续，可一次性突发读取，注意先收到的是高字节 */
#define 	MPU6050_ACCEL_XOUT_H	0x3B	/*加速度计 X 轴输出，高字节*/
#define 	MPU6050_ACCEL_XOUT_L	0x3C	/*加速度计 X 轴输出，低字节*/
#define 	MPU6050_ACCEL_YOUT_H	0x3D	/*加速度计 Y 轴输出，高字节*/
#define 	MPU6050_ACCEL_YOUT_L	0x3E	/*加速度计 Y 轴输出，低字节*/
#define 	MPU6050_ACCEL_ZOUT_H	0x3F	/*加速度计 Z 轴输出，高字节*/
#define 	MPU6050_ACCEL_ZOUT_L	0x40	/*加速度计 Z 轴输出，低字节*/
#define 	MPU6050_TEMP_OUT_H		0x41	/*温度输出，高字节。合成16位后：T(°C) = 读数 / 340 + 36.53*/
#define 	MPU6050_TEMP_OUT_L		0x42	/*温度输出，低字节*/
#define 	MPU6050_GYRO_XOUT_H		0x43	/*陀螺仪 X 轴输出，高字节*/
#define 	MPU6050_GYRO_XOUT_L		0x44	/*陀螺仪 X 轴输出，低字节*/
#define 	MPU6050_GYRO_YOUT_H		0x45	/*陀螺仪 Y 轴输出，高字节*/
#define 	MPU6050_GYRO_YOUT_L		0x46	/*陀螺仪 Y 轴输出，低字节*/
#define 	MPU6050_GYRO_ZOUT_H		0x47	/*陀螺仪 Z 轴输出，高字节*/
#define 	MPU6050_GYRO_ZOUT_L		0x48	/*陀螺仪 Z 轴输出，低字节*/

/* ---------------- 电源管理与身份识别 ---------------- */
#define 	MPU6050_PWR_MGMT_1		0x6B	/*电源管理1：设备复位、睡眠模式、循环模式、时钟源选择*/
#define 	MPU6050_PWR_MGMT_2		0x6C	/*电源管理2：各轴待机(Standby)控制*/
#define 	MPU6050_WHO_AM_I		0x75	/*器件ID寄存器，固定返回 0x68，可用于检测I2C通信是否正常*/
#endif
