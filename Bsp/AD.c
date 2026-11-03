#include"stm32f10x.h"
/**
  * 说    明：EOC 轮询上限，单位是「循环圈数」，不是时间。
  *           单次转换实耗约 5.67us（ADC 时钟 12MHz：采样 55.5 周期
  *           + 转换 12.5 周期 = 68 周期）；轮询一圈约 0.1us，
  *           所以正常情况下几十圈内 EOC 就会置位。
  *           这个上限只在 ADC 未使能、时钟异常等导致 EOC 永不置位时
  *           兜底。取 100000（约 10ms 量级）远大于正常所需，
  *           以免把正常情况误判成超时。
  */
#define ADC_POLL_LIMIT 100000

void AD_Init(void){
    //开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    //配置ADC分频 最大14MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    //初始化GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //规则组配置 
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//单次或连续
    ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
    ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//触发源选择
    ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
    ADC_InitStructure.ADC_NbrOfChannel=1;//选择的通道数
    ADC_InitStructure.ADC_ScanConvMode=DISABLE;//扫描模式
    ADC_Init(ADC1,&ADC_InitStructure);

    ADC_Cmd(ADC1,ENABLE);//使能
    ADC_ResetCalibration(ADC1);//重启校准
    while(ADC_GetResetCalibrationStatus(ADC1)==SET);
    ADC_StartCalibration(ADC1);//开启校准
    while(ADC_GetCalibrationStatus(ADC1)==SET);
}
/**
  * 函    数：软件触发一次 ADC 转换，等它转完后读回结果
  * 参    数：无
  * 返 回 值：12 位转换值，范围 0~4095
  * 说    明：AD_Init 把 ADC 配成了单次模式（ADC_ContinuousConvMode 为
  *           DISABLE），它不会自己连续转换，所以每要一个数都得在这里
  *           软件启动一次。
  *           调用 ADC_SoftwareStartConvCmd 后 EOC 标志被硬件清零，
  *           转换结束时自动置位，下面的 while 就是在等这个置位。
  *           等待是带超时的：万一 EOC 永不置位（ADC 没使能、时钟没配好、
  *           受干扰），超过 ADC_POLL_LIMIT 圈就放弃，不会死等。
  *           超时时返回上一次成功的读数而不是 0 —— 这个值拿去算电池
  *           百分比，返回 0 会让电量瞬间掉到 0%；返回旧值顶多是显示
  *           停在几毫秒前，用户无感。
  */
uint16_t AD_GetValue(void){
    static uint16_t last_value=0;
    uint32_t poll=0;
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET){
        if(++poll>ADC_POLL_LIMIT) return last_value;
    }
    last_value=ADC_GetConversionValue(ADC1);
    return last_value;
}