#include "stm32f10x.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;
float VBAT;
int Battery_Capacity;

void Show_Batterry(void){
    int sum=0;
    for(int i=0;i<3000;i++){

        ADValue=AD_GetValue();
        sum+=ADValue;
    }
    ADValue=sum/3000;
    
    VBAT=(float)ADValue/4095*3.3;
    Battery_Capacity=(ADValue-3276)*100/815;
    if(Battery_Capacity<0) Battery_Capacity=0;
   // OLED_ShowNum(64,0,ADValue,4,OLED_6X8);


    OLED_ShowNum(85,4,Battery_Capacity,3,OLED_6X8);
    OLED_ShowChar(103,4,'%',OLED_6X8);
    if(Battery_Capacity==100) OLED_ShowImage(110,0,16,16,Battery);
    else if(Battery_Capacity>=10&&Battery_Capacity<100){
        OLED_ShowImage(110,0,16,16,Battery);
        OLED_ClearArea((112+Battery_Capacity/10),5,(10-Battery_Capacity/10),6);
        OLED_ClearArea(85,4,6,8);

    }else{
        OLED_ShowImage(110,0,16,16,Battery);
        OLED_ClearArea(112,5,10,6);
        OLED_ClearArea(85,4,12,8);
    }
}
