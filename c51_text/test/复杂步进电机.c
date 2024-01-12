#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

#define uint16 unsigned int

unsigned char FWD_TABLE[8] = {0xEF, 0xCF, 0xDF, 0x9F, 0xBF, 0x3F, 0x7f, 0x6F}; // 4-8拍正转相序字
unsigned char REV_TABLE[8] = {0x6F, 0x7f, 0x3F, 0xBF, 0x9F, 0xDF, 0xCF, 0xEF}; // 4-8拍反转相序字
unsigned char f_rotation[4] = {0xCF, 0x9F, 0x3F, 0x6F};
unsigned char b_rotation[4] = {0x6F, 0x3F, 0x9F, 0xCF};

unsigned char Clockwise[] = {0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09, 0x01, 0x03};     // 顺时针A-AB-B-BC-C-CD-D-DA
unsigned char Anticlockwise[] = {0x03, 0x01, 0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03}; // 逆时针转动顺序

unsigned char f_k[] = {0x03, 0x06};
unsigned char b_k[] = {0x06, 0x03};

void Delay(uint16 ms)
{
    uint16 i;

    do
    {
        for (i = 700; i; i--)
            ;
    } while (--ms);
}

int log = 0;

void Reverse() __interrupt 0
{
    if (log == 1)
    {
        log = 0;
    }
    else
    {
        log = 1;
    }
}
int log_2 = 1;
void Stop() __interrupt 2
{
    if (log_2 == 1)
    {
        log_2 = 0;
    }
    else
    {
        log_2 = 1;
    }
}

void main()
{
    int i;
    int c_time = 20;

    IT0 = 1; // 外部中断0为跳变沿触发方式
    IT1 = 1; // 外部中断1为跳变沿触发方式
    EA = 1;  // 开总中断
    EX0 = 1; // 打开外部中断0
    EX1 = 1; // 打开外部中断1

    while (1)
    {
        // 顺时针360，逆时针360
        //  for (i = 0; i < 9; i++)
        //  {
        //      P2 = Clockwise[i]; // 查表并输出相序字，可自行换成反转表格
        //      delay_ms(30);      // 改变延时可调整步进电机的转速
        //  }
        //  delay_ms(100);
        //  for (i = 0; i < 9; i++)
        //  {
        //      P2 = Anticlockwise[i]; // 查表并输出相序字，可自行换成反转表格
        //      delay_ms(30);          // 改变延时可调整步进电机的转速
        //  }
        //  delay_ms(100);

        // 顺时针90，逆时针90
        //  for (i = 0; i < 2; i++)
        //  {
        //      P2 = f_k[i];  // 查表并输出相序字，可自行换成反转表格
        //      delay_ms(30); // 改变延时可调整步进电机的转速
        //  }
        //  delay_ms(100);
        //  for (i = 0; i < 2; i++)
        //  {
        //      P2 = b_k[i];  // 查表并输出相序字，可自行换成反转表格
        //      delay_ms(30); // 改变延时可调整步进电机的转速
        //  }
        //  delay_ms(100);

        if (P3_0 == 0)
        {
            if (c_time > 10)
            {
                c_time -= 10;
            }
        }
        if (P3_1 == 0)
        {
            if (c_time < 200)
            {
                c_time += 10;
            }
        }
        if (log_2)
        {
            for (i = 0; i < 9; i++)
            {
                if (log == 0)
                {
                    P2 = Clockwise[i]; // 查表并输出相序字，可自行换成反转表格
                    delay_ms(c_time);  // 改变延时可调整步进电机的转速
                }
                else if (log == 1)
                {
                    P2 = Anticlockwise[i]; // 查表并输出相序字，可自行换成反转表格
                    delay_ms(c_time);      // 改变延时可调整步进电机的转速
                }
            }
        }
    }
}
