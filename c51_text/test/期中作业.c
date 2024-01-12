#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

#define uchar unsigned char
#define uint8 unsigned char
#define uint16 unsigned int

#define SEG_CODE_PORT P0
#define BIT_CODE_PORT P2

#define max 13

uint8 SegCodeTable[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
                        0x80, 0x90, 0x88}; // 0~9的显示码

uchar t = 0;
// sbit P2_0 = P2 ^ 0;
// sbit P2_1 = P2 ^ 1;
// bit Ldelay = 0;
// bit flag = 0;
int Ldelay = 0;
int flag = 0;
int num = 0;

void int0_src() __interrupt 0
{
    flag = !flag;
    TH0 = 0x3c;
    TL0 = 0xb0;
}
void time0() __interrupt 1
{
    t++;
    if (t == 20)
    {
        t = 0;
        // Ldelay=1;
        num++;
    }
    TH0 = 0x3c;
    TL0 = 0xb0;
    // if (num == max)//3-2func
    // {
    //     flag = 0;
    //     num = 0;
    // }
}
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

void Led4segRefresh(int f_num)
{
    uint8 i;
    // 第一个数码管
    i = f_num % 10;
    SEG_CODE_PORT = SegCodeTable[i];
    BIT_CODE_PORT = 0x00;
    delay(1);
    BIT_CODE_PORT = 0xf8;
    // 第二个数码管
    i = f_num / 10;
    SEG_CODE_PORT = SegCodeTable[i];
    BIT_CODE_PORT = 0x01;
    delay(1);
    BIT_CODE_PORT = 0xf8;
}

void main()
{
    IT0 = 1;
    IE = 0x81; // 中断初始化
    TMOD = 0x01;
    TH0 = 0x3c;
    TL0 = 0xb0;
    EA = 1;

    while (1)
    {
        if (flag == 1)
        {
            TR0 = 1;
            ET0 = 1;
            // P2_0 = 1;
            // P2_1 = 0;
        }
        else if (flag == 0)
        {
            TR0 = 0;
            ET0 = 0;
            // P2_0 = 0;
            // P2_1 = 1;
        }
        Led4segRefresh((num % max));
    }
}