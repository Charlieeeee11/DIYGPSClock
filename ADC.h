#include "intrins.h"
#include <math.h>

sfr ADC_LOW2    =   0xBE;           //ADC低2位结果

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

void InitADC();

void GetADCResult(unsigned char ch,unsigned int *value);

void ADC_convert(void);