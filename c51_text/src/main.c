#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

#define uchar unsigned char
#define uint8 unsigned char
#define uint16 unsigned int
#define _nop_() __asm NOP __endasm

#define My_pwm P2_0   //  P2.0输出pwm
#define My_pwm_1 P2_4 //  P2.0输出pwm
uchar time;           // 定义占空比的变量
uchar time_1;         // 定义占空比的变量
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
// 串口初始化
// char Led_mode[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
void tim0() __interrupt 1
{
    TR0 = 0;    // 赋初值时，关闭定时器
    TH0 = 0xff; //(65536-10)/256;//赋初值定时
    TL0 = 0xf7; //(65536-10)%256;//0.01ms
    TR0 = 1;    // 打开定时器

    time++;
    if (time >= 100) // 1khz
        time = 0;
    if (time <= 50) // 占空比%50,可改
        My_pwm = 1;
    else
        My_pwm = 0;
    time_1++;
    if (time_1 >= 100) // 1khz
        time_1 = 0;
    if (time_1 <= 50) // 占空比%30,可改
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

    TMOD = 0x21;      // 定时器0工作方式1
    TH0 = 0xff;       //(65536-10)/256;//赋初值定时
    TL0 = 0xf7;       //(65536-10)%256;//0.01ms
    TH1 = TL1 = 0xfa; // 4800bps
    PCON = 0;         // 波特率不加倍
    SCON = 0x50;
    TR1 = 1;
    EA = 1;  // 开总中断
    ET0 = 1; // 开定时器0中断
    TR0 = 1; // 启动定时器0

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