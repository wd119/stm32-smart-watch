#include "stm32f10x.h"
#include "OLED.h"
#include "key.h"
#include "menu.h"
#include "MPU6050.h"
#include "Delay.h"
#include <math.h>

//MPU6050
int16_t ax,ay,az,gx,gy,gz;//MPU6050测得的三轴加速度和角速度
float roll_g,pitch_g,yaw_g;//陀螺仪解算欧拉角 
float roll_a,pitch_a;//加速度计解算欧拉角
float Roll,Pitch,Yaw;//互补滤波欧拉角
float a=0.9;//互补滤波系数
float dela=0.005;//采样周期
double pi=3.1415927;
void MPU6050_Calculation(void){
    Delay_ms(5);
    MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
    roll_g=Roll+(float)gx*dela;
    pitch_g=Pitch+(float)gy*dela;
    yaw_g=yaw_g+(float)gz*dela;

    pitch_a=atan2((-1)*ax,az)*180/pi;
    roll_a=atan2(ay,az)*180/pi;

    Roll=a*roll_g+(1-a)*roll_a;
    Pitch=a*pitch_g+(1-a)*pitch_a;
    Yaw=a*yaw_g;
}
void Show_MPU6050_UI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_Printf(0,16,OLED_8X16,"Roll:%.2f",Roll);
    OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
    OLED_Printf(0,48,OLED_8X16,"Yaw:%.2f",Yaw);
}
int MPU6050(void){
    while(1){
        KeyNum1=Key_GetNum();
        if(KeyNum1==3){
            OLED_Clear();
            OLED_Update();
            return 0;
        }
        OLED_Clear();
        MPU6050_Calculation();
        Show_MPU6050_UI();
        OLED_ReverseArea(0,0,16,16);
        OLED_Update();
    }
}

//------------------------水平仪-----------------------------------

void Show_Gradienter_UI(void){
    MPU6050_Calculation();
    OLED_DrawCircle(64,32,30,0);
    OLED_DrawCircle(64-Roll,32+Pitch,4,1);
}
int Gradienter(void){
      while(1){
        KeyNum1=Key_GetNum();
        if(KeyNum1==3){
            OLED_Clear();
            OLED_Update();
            return 0;
        }
        OLED_Clear();
        Show_Gradienter_UI();
        OLED_Update();
    }
}
