#include"stm32f10x.h"
#include<time.h>
int MyRTC_Time[]={2026,9,7,18,15,0};
void MyRTC_SetTime(void);
void MyRTC_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5){
        RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    //等待同步
    RTC_WaitForSynchro();
    //等待上次写入完成
    RTC_WaitForLastTask();
    RTC_SetPrescaler(32768-1);
    RTC_WaitForLastTask();

    MyRTC_SetTime();
    BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
    }
    else{
       RTC_WaitForSynchro();
    //等待上次写入完成
    RTC_WaitForLastTask();
    }

    
}
void MyRTC_SetTime(void){
    time_t time_cnt;
    struct tm time_tm;
    time_tm.tm_year=MyRTC_Time[0]-1900;
    time_tm.tm_mon=MyRTC_Time[1]-1;
    time_tm.tm_mday=MyRTC_Time[2];  
    time_tm.tm_hour=MyRTC_Time[3];
    time_tm.tm_min=MyRTC_Time[4];
    time_tm.tm_sec=MyRTC_Time[5];

    time_cnt=mktime(&time_tm)-8*3600;//将时间转换为时间�?
    RTC_SetCounter(time_cnt);
    RTC_WaitForLastTask();
}
void MyRTC_GetTime(void){
    time_t time_cnt;
    struct tm time_tm;
    time_cnt=RTC_GetCounter()+8*3600;//获取时间�?
    time_tm=*localtime(&time_cnt);
    MyRTC_Time[0]=time_tm.tm_year+1900;
    MyRTC_Time[1]=time_tm.tm_mon+1;
    MyRTC_Time[2]=time_tm.tm_mday;
    MyRTC_Time[3]=time_tm.tm_hour;
    MyRTC_Time[4]=time_tm.tm_min;
    MyRTC_Time[5]=time_tm.tm_sec;
}