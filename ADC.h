#include "intrins.h"
#include <math.h>

sfr ADC_LOW2    =   0xBE;           //ADC��2λ���

#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��

void InitADC();

void GetADCResult(unsigned char ch,unsigned int *value);

void ADC_convert(void);