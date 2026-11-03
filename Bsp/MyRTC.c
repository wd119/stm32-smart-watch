#include"stm32f10x.h"
#include<time.h>
#include"Delay.h"

#define LSE_TIMEOUT_MS 10000
#define LSI_TIMEOUT_MS 100
int MyRTC_Time[]={2026,9,7,18,15,0};
void MyRTC_SetTime(void);
void MyRTC_Init(void){
    uint16_t lse_wait=0;
    uint16_t lsi_wait=0;
    uint8_t rtc_use_lsi=0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5){
        RCC_LSEConfig(RCC_LSE_ON);
        for(lse_wait=0; lse_wait<LSE_TIMEOUT_MS; lse_wait++){
        if(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=RESET) break;
        Delay_us(1000);
        }
        if(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET){
        RCC_LSEConfig(RCC_LSE_OFF);
        RCC_LSICmd(ENABLE);
        for(lsi_wait=0; lsi_wait<LSI_TIMEOUT_MS; lsi_wait++){
            if(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)!=RESET) break;
            Delay_us(1000);
        }
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        rtc_use_lsi=1;
        }else{
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        }
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    if(rtc_use_lsi) RTC_SetPrescaler(40000-1);
    else            RTC_SetPrescaler(32768-1);
    RTC_WaitForLastTask();

    MyRTC_SetTime();
    BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
    }
    else{
       RTC_WaitForSynchro();
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

    time_cnt=mktime(&time_tm)-8*3600;
    RTC_SetCounter(time_cnt);
    RTC_WaitForLastTask();
}
void MyRTC_GetTime(void){
    time_t time_cnt;
    struct tm time_tm;
    time_cnt=RTC_GetCounter()+8*3600;
    time_tm=*localtime(&time_cnt);
    MyRTC_Time[0]=time_tm.tm_year+1900;
    MyRTC_Time[1]=time_tm.tm_mon+1;
    MyRTC_Time[2]=time_tm.tm_mday;
    MyRTC_Time[3]=time_tm.tm_hour;
    MyRTC_Time[4]=time_tm.tm_min;
    MyRTC_Time[5]=time_tm.tm_sec;
}