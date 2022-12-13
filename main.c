#include "STC15F2K.h"
#include <string.h>
#include "LED.h"
#include "ADC.h"
#include "Definition.h"

extern void init_ds1302_io();
extern void init_ds1302();
extern void read_time();
extern void write_time();

extern uchar Secs;
extern uchar Mins;
extern uchar Hours;		 

/*void Delayms(uint t)
{
 	unsigned char i, j;
 
        i= 6;
        j= 211;
        do
        {
                 while(--j);
        }while (--i);
}*/
void  Delayms()
{
     unsigned int i;
     i = FOSC / 14000L; //1T
     while(--i)  ;
}


void Uart_Init()
{
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)

    SCON = 0x50;                //8位可变波特率

    T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    ES = 1;                     //使能串口1中断
    EA = 1;
}


char menu=0;

void Timer2_isr()
{
	S12=0;S1=1;
	P2=LED0[table[0]];
	Delayms();	
	S1=0;S2=1;
	if(flag==0)P2=LED1[table[1]];
	if(flag==1)P2=LED0[table[1]];
	Delayms();
	S2=0;S3=1;
	P2=LED0[table[2]];	
	Delayms();
	S3=0;S4=1;
	P2=LED0[table[3]];
	Delayms();	
//////////////////////////////////////////////////////
	P2=0xff;
	S4=0;S5=1;
	P2=g_LED2[t_led0[0]]&g_LED3[t_led1[0]];
	Delayms();		

	P2=0xff;
	S5=0;S6=1;
	P2=g_LED2[t_led0[1]]&g_LED3[t_led1[1]];
	Delayms();		

	P2=0xff;
	S6=0;S7=1;
	P2=g_LED2[t_led0[2]]&g_LED3[t_led1[2]];
	Delayms();		

	P2=0xff;
	S7=0;S8=1;
	P2=g_LED2[t_led0[3]]&g_LED3[t_led1[3]];
	Delayms();		

	P2=0xff;
	S8=0;S9=1;
	P2=g_LED2[t_led0[4]]&g_LED3[t_led1[4]];
	Delayms();		

	P2=0xff;
	S9=0;S10=1;
	P2=g_LED2[t_led0[5]]&g_LED3[t_led1[5]];
	Delayms();	

	P2=0xff;
	S10=0;S11=1;
	P2=g_LED2[t_led0[6]]&g_LED3[t_led1[6]];
	Delayms();	

	P2=0xff;
	S11=0;S12=1;
	P2=g_LED2[t_led0[7]]&g_LED3[t_led1[7]];
	Delayms();	

	S1=S2=S3=S4=S5=S6=S7=S8=S9=S10=S11=S12=1;
	P2=0xff;
	S1=S2=S3=S4=S5=S6=S7=S8=S9=S10=S11=S12=0;
	Delayms();
}

void uart1_isr() interrupt 4 using 1
{
	unsigned char RecBuf;
	tl++;
    if (RI)
    {
		RecBuf=SBUF;
        RI = 0;                 //清除RI位
		//SBUF=RecBuf;
		if (!count1)
		{
			if (!count)
			{
				g_RecBuf = RecBuf;
			}
			else
			{
				g_RecBuf = g_RecBuf | (RecBuf << 8);
				if (g_RecBuf == GPSTOKEN) count1++;
				g_RecBuf = 0;
			}
			count = ~count;
		}
		else if (count1 == 4)
		{
			UTC_Hours = (RecBuf - 48) << 4;
			count1++;
		}
		else if (count1 == 5)
		{
			UTC_Hours |= RecBuf - 48;
			//UTC_Hours += 0x8;
			count1++;
		}
		else if (count1 == 6)
		{
			UTC_Mins = (RecBuf - 48) << 4;
			count1++;
		}
		else if (count1 == 7)
		{
			UTC_Mins |= RecBuf - 48;
			count1++;
		}
		else if (count1 == 8)
		{
			UTC_Secs = (RecBuf - 48) << 4;
			count1++;
		}
		else if (count1 == 9)
		{
			UTC_Secs |= RecBuf - 48;
			count1++;
		}
		else if (count1 > 9 && RecBuf == ',')
		{
			Next_Flag = true;
			count1++;
		}
		else if (Next_Flag)
		{
			if (RecBuf != 65)
			{
				UTC_Hours = 0;
				UTC_Mins = 0;
				UTC_Secs = 0;
			}
			else
			{
				if (UTC_Hours == 0x0 || UTC_Hours == 0x1 || UTC_Hours == 0x10 || UTC_Hours == 0x11)
					UTC_Hours += 0x8;
				else if (UTC_Hours >= 0x2 && UTC_Hours <= 0x9 || (UTC_Hours >= 0x12 && UTC_Hours <= 0x15))
					UTC_Hours += 0xE;
				else if (UTC_Hours >= 0x16 && UTC_Hours <= 0x19)
					UTC_Hours -= 0x16;
				else if (UTC_Hours >= 20 && UTC_Hours <= 0x24)
					UTC_Hours -= 0x1C;
				if(Secs != UTC_Secs || Mins != UTC_Mins || Hours != UTC_Hours)
				{
					ET1 = 1;

					Hours = UTC_Hours;
					Mins = UTC_Mins;
					Secs = UTC_Secs;
					write_time();
				}
				
			}
			count1 = 0;
			Next_Flag = false;
				
		}
		else
		{
			count1++;
		}
		
		
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}



void main()
{
	uchar i;
    //Timer0Init();	
	InitADC();
	init_ds1302_io();
	init_ds1302();

	P3M0=0xf8;P3M1=0x00;
	P2M0=0xff;P2M1=0x00;
	P1M0=0xff;P1M1=0x00;
	P5M0=0x10;P5M1=0x00;
	Uart_Init();
	
	while(1)
	{
		
		if(menu==0)
		{
			read_time();
			
			if(Secs%2)
			{
				flag = 1;
				table[0]=Hours/16;
				table[1]=Hours%16;
				table[2]=Mins/16;
				table[3]=Mins%16;
			}
			else
			{
				 flag = 0;
			}

			for(i=0;i<8;i++)t_led1[i]=i;
			for(i=0;i<8;i++)t_led0[i]=8;

			if(Secs==0x01)t_led0[0]=0;
			if(Secs==0x02)t_led0[0]=1;
			if(Secs==0x03)t_led0[0]=2;
			if(Secs==0x04)t_led0[0]=3;
			if(Secs==0x05)t_led0[0]=4;
			if(Secs==0x06)t_led0[0]=5;
			if(Secs==0x07)t_led0[0]=6;
			if(Secs==0x08)t_led0[0]=7;
		
			if(Secs==0x09)t_led0[1]=0;
			if(Secs==0x10)t_led0[1]=1;
			if(Secs==0x11)t_led0[1]=2;
			if(Secs==0x12)t_led0[1]=3;
			if(Secs==0x13)t_led0[1]=4;
			if(Secs==0x14)t_led0[1]=5;
			if(Secs==0x15)t_led0[1]=6;
			if(Secs==0x16)t_led0[1]=7;
		
			if(Secs==0x17)t_led0[2]=0;
			if(Secs==0x18)t_led0[2]=1;
			if(Secs==0x19)t_led0[2]=2;
			if(Secs==0x20)t_led0[2]=3;
			if(Secs==0x21)t_led0[2]=4;
			if(Secs==0x22)t_led0[2]=5;
			if(Secs==0x23)t_led0[2]=6;
			if(Secs==0x24)t_led0[2]=7;
		
			if(Secs==0x25)t_led0[3]=0;
			if(Secs==0x26)t_led0[3]=1;
			if(Secs==0x27)t_led0[3]=2;
			if(Secs==0x28)t_led0[3]=3;
			if(Secs==0x29)t_led0[3]=4;
			if(Secs==0x30)t_led0[3]=5;
			if(Secs==0x31)t_led0[3]=6;
			if(Secs==0x32)t_led0[3]=7;
		
			if(Secs==0x33)t_led0[4]=0;
			if(Secs==0x34)t_led0[4]=1;
			if(Secs==0x35)t_led0[4]=2;
			if(Secs==0x36)t_led0[4]=3;
			if(Secs==0x37)t_led0[4]=4;
			if(Secs==0x38)t_led0[4]=5;
			if(Secs==0x39)t_led0[4]=6;
			if(Secs==0x40)t_led0[4]=7;
		
			if(Secs==0x41)t_led0[5]=0;
			if(Secs==0x42)t_led0[5]=1;
			if(Secs==0x43)t_led0[5]=2;
			if(Secs==0x44)t_led0[5]=3;
			if(Secs==0x45)t_led0[5]=4;
			if(Secs==0x46)t_led0[5]=5;
			if(Secs==0x47)t_led0[5]=6;
			if(Secs==0x48)t_led0[5]=7;
		
			if(Secs==0x49)t_led0[6]=0;
			if(Secs==0x50)t_led0[6]=1;
			if(Secs==0x51)t_led0[6]=2;
			if(Secs==0x52)t_led0[6]=3;
			if(Secs==0x53)t_led0[6]=4;
			if(Secs==0x54)t_led0[6]=5;
			if(Secs==0x55)t_led0[6]=6;
			if(Secs==0x56)t_led0[6]=7;
		
			if(Secs==0x57)t_led0[7]=0;
			if(Secs==0x58)t_led0[7]=1;
			if(Secs==0x59)t_led0[7]=2;
			if(Secs==0x00)t_led0[7]=3;				
		}

		Timer2_isr();
		Delayms();						
	}
}

