#include "STC15F2K.h"	        

/******************
	ds1302 �ڲ�RAM   RAM0  1100 000R/W   1�� 0д
					 RAM1  1100 001R/W  
					       ....... 
					 RAM30 1111 110R/W   
********************/

sbit clk = P3^2;	  //ds1302ʱ���߶���
sbit io =  P3^1;	  //������
sbit rst = P5^5;	  //��λ��
						
uchar code write_add[]={0x80,0x82,0x84}; 	
uchar code init_ds[]  ={0x55,0x17,0x15,0x01,0x01,0x13,0x13};   
uchar Secs,Mins,Hours;//,ri,yue,week,nian=0x20;


/*************дһ�����ݵ���Ӧ�ĵ�ַ��***************/
void write_ds1302(uchar add,uchar dat)
{
	uchar i;		
	rst = 1;			 //�Ѹ�λ���ø�
	for(i=0;i<8;i++)
	{				     //��λ��ǰ
		clk = 0;		 //ʱ�����õͿ�ʼд����
		io = add & 0x01;    	
		add >>= 1;		 //�ѵ�ַ����һλ
		clk = 1;		 //ʱ�����ø�
	}	
	for(i=0;i<8;i++)
	{
		clk = 0;		 //ʱ�����õͿ�ʼд����
		io = dat & 0x01;
		dat >>= 1;		 //����������һλ
		clk = 1;		 //ʱ�����ø�
	}
	rst = 0;			 //��λ�ߺϵ�
	clk = 0;
	io = 0;
}

/*************�Ӷ�Ӧ�ĵ�ַ��һ�����ݳ���***************/
uchar read_ds1302(uchar add)
{
	uchar value,i;
	rst = 1;			 //�Ѹ�λ���ø�
	for(i=0;i<8;i++)
	{				     //��λ��ǰ
		clk = 0;		 //ʱ�����õͿ�ʼд����
		io = add & 0x01;    	
		add >>= 1;		 //�ѵ�ַ����һλ
		clk = 1;		 //ʱ�����ø�
	}		
	for(i=0;i<8;i++)
	{
		clk = 0;		 //ʱ�����õͿ�ʼ������
		value >>= 1;
		if(io == 1)
			value |= 0x80;
		clk = 1;		 //ʱ�����ø�
	}
	rst = 0;			 //��λ�ߺϵ�
	clk = 0;
	io = 0;
	return value;		 //���ض�����������
}

/*************��Ҫ��ʱ�� ������ ��������***************/
void read_time()
{
	Secs = read_ds1302(0x81);	//����

	Mins  = read_ds1302(0x83);	//����
	Hours  = read_ds1302(0x85);	//��ʱ

//	ri   = read_ds1302(read_add[3]);	//����
//	yue  = read_ds1302(read_add[4]);	//����
//	nian = read_ds1302(read_add[5]);	//����
//	week = read_ds1302(read_add[6]);	//������
}

#define CLOCK_STOP 0x80 //bit7=1,clock disable
#define CLOCK_WORK 0x7f //bit7=0,clock enable
/*************��Ҫд��ʱ�� ������ ��д��ds1302��***************/
void write_time()
{
	
	write_ds1302(0x8e,0x00);			//�ر�д����
	write_ds1302(0x80,(Secs | CLOCK_STOP));
	write_ds1302(0x82,Mins);		//д��
	write_ds1302(0x84,Hours);		//дʱ
	write_ds1302(0x80,(Secs & CLOCK_WORK));
//	write_ds1302(write_add[3],ri);		//д��
//	write_ds1302(write_add[4],yue);		//д��
//	write_ds1302(write_add[5],nian);	//д��
//	write_ds1302(write_add[6],week);	//д����


	write_ds1302(0x8e,0x80);			//��д����
}

/*************�����ݱ��浽ds1302 RAM��**0-31*************/
void write_ds1302ram(uchar add,uchar dat)
{
	add <<= 1;     //��ַ�Ǵӵڶ�λ��ʼ��
	add &= 0xfe;   //�����λ����  ��д������
	add |= 0xc0;   //��ַ�����λΪ 1  
	write_ds1302(0x8e,0x00);
	write_ds1302(add,dat);	
	write_ds1302(0x8e,0x80);
}

/*************�����ݴ�ds1302 RAM������**0-31*************/
uchar read_ds1302ram(uchar add)
{
	add <<= 1;     //��ַ�Ǵӵڶ�λ��ʼ��
	add |= 0x01;   //�����λ��1  �Ƕ�����
	add |= 0xc0;   //��ַ�����λΪ 1  
	return(read_ds1302(add));	
}

/*************��ʼ��ds1302ʱ��***************/
void init_ds1302()
{
	uchar i;
	rst = 0;	//��һ�ζ�д����ʱҪ��IOƷ�õ�
	clk = 0;
	io = 0;
	i = read_ds1302ram(30);   
	if(i != 3)
	{	
		i = 3;
		write_ds1302ram(30,i);
		write_ds1302(0x8e,0x00);	            //��д����
		for(i=0;i<3;i++)
			write_ds1302(write_add[i],init_ds[i]);	//�����λֵ0 ����ds1302����
		write_ds1302(0x8e,0x80);	//��д����
	}
}

void init_ds1302_io()
{
	rst = 0;	//��һ�ζ�д����ʱҪ��IOƷ�õ�
	clk = 0;
	io = 0;	
}
