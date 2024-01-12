#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

#define uchar unsigned char
#define uint8 unsigned char
#define uint16 unsigned int
#define _nop_() __asm NOP __endasm

#define My_pwm P2_0   //  P2.0���pwm
#define My_pwm_1 P2_4 //  P2.0���pwm
uchar time;           // ����ռ�ձȵı���
uchar time_1;         // ����ռ�ձȵı���
void delay(int ms)
{
    uint16 i;
    do
    {
        i = 790;
        while (--i)
            ;
    } while (--ms);
}
// ���ڳ�ʼ��
// char Led_mode[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
void tim0() __interrupt 1
{
    TR0 = 0;    // ����ֵʱ���رն�ʱ��
    TH0 = 0xff; //(65536-10)/256;//����ֵ��ʱ
    TL0 = 0xf7; //(65536-10)%256;//0.01ms
    TR0 = 1;    // �򿪶�ʱ��

    time++;
    if (time >= 100) // 1khz
        time = 0;
    if (time <= 50) // ռ�ձ�%50,�ɸ�
        My_pwm = 1;
    else
        My_pwm = 0;
    time_1++;
    if (time_1 >= 100) // 1khz
        time_1 = 0;
    if (time_1 <= 50) // ռ�ձ�%30,�ɸ�
        My_pwm_1 = 1;
    else
        My_pwm_1 = 0;
}

void Delay10ms() //@11.0592MHz
{
    unsigned char i, j;

    i = 108;
    j = 145;
    do
    {
        while (--j)
            ;
    } while (--i);
}

void main()
{

    TMOD = 0x21;      // ��ʱ��0������ʽ1
    TH0 = 0xff;       //(65536-10)/256;//����ֵ��ʱ
    TL0 = 0xf7;       //(65536-10)%256;//0.01ms
    TH1 = TL1 = 0xfa; // 4800bps
    PCON = 0;         // �����ʲ��ӱ�
    SCON = 0x50;
    TR1 = 1;
    EA = 1;  // �����ж�
    ET0 = 1; // ����ʱ��0�ж�
    TR0 = 1; // ������ʱ��0

    uchar receive;

    while (1)
    {
        while (RI == 1)
        {
            RI = 0;
            receive = SBUF;
            SBUF = receive;
            while (TI == 0)
                ;
            TI = 0;
        }
        if (receive == '1')
        {
            P2_1 = 0;
            P2_2 = 1;
            P2_6 = 1;
            P2_7 = 0;

            Delay10ms();
        }
        if (receive == '2')
        {

            P2_1 = 1;
            P2_2 = 0;
            P2_6 = 0;
            P2_7 = 1;
            Delay10ms();
        }
        receive = 0;
        P2_1 = 0;
        P2_2 = 0;
        P2_6 = 0;
        P2_7 = 0;
        Delay10ms();
    }
}