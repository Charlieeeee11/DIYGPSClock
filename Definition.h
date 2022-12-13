#define FOSC 11059200L
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

#define FOSC 11059200L          //系统频率
#define BAUD 9600             //串口波特率

#define true 1
#define false 0

typedef unsigned char uint8_t;

bit count = 0;
uint8_t count1 = 0;
uint8_t UTC_Hours;
uint8_t UTC_Mins;
uint8_t UTC_Secs;
unsigned short g_RecBuf;
bit Next_Flag = false;

const short GPSTOKEN = 0x5250;

int tl = 0;
int s_tl = 0;
char l_Secs = 0;
bit busy;

sbit S1=P3^3;
sbit S2=P3^4;
sbit S3=P3^5;
sbit S4=P3^6;
sbit S5=P3^7;
sbit S6=P1^2;
sbit S7=P1^3;
sbit S8=P1^4;
sbit S9=P1^5;
sbit S10=P1^6;
sbit S11=P1^7;
sbit S12=P5^4;

sbit bell=P3^0;

sbit set=P1^0;
sbit jia=P1^1;

uchar t_led0[8]={0};   
uchar t_led1[8]={0}; 
uchar table[4]={1,2,3,4};
char ps=0;
char flag=0;

int ld;