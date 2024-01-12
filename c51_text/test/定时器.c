#include <STC89xx.h>
#include "delay.h"
#include "mcs51/8052.h"
#define FOSC 11059200

#define T_NUMBER_1MS (65536 - FOSC / 12 / 1000)

SFR(MOD_0, 0x89);
SFR(T_TL0, 0x8A);
SFR(T_TH0, 0x8C);
SFR(T_TL1, 0x8B);
SFR(T_TH1, 0x8D);

SBIT(T_TF0, 0x88, 5);
SBIT(T_TF1, 0x88, 7);
SBIT(T_TR0, 0x88, 4);
SBIT(T_TR1, 0x88, 6);

SBIT(LED0, _P1, 0);
SBIT(LED1, _P1, 1);

unsigned int count1 = 0;
unsigned int count2 = 0;
void main()
{
    MOD_0 = 0b00010001;
    T_TL0 = T_NUMBER_1MS;
    T_TH0 = T_NUMBER_1MS >> 8;
    T_TL1 = T_NUMBER_1MS;
    T_TH1 = T_NUMBER_1MS >> 8;

    T_TR0 = 0x01;
    T_TR1 = 0x01;

    while (1)
    {
        if (T_TF0 == 1)
        {
            T_TF0 = 0;
            T_TL0 = T_NUMBER_1MS;
            T_TH0 = T_NUMBER_1MS >> 8;
            count1 += 1;
            if (count1 >= 50)
            {
                count1 = 0;
                if (LED0 == 1)
                {
                    LED0 = 0;
                }
                else
                {
                    LED0 = 1;
                }
            }
        }
        if (T_TF1 == 1)
        {
            // 软件清0
            T_TF1 = 0;
            T_TL1 = T_NUMBER_1MS;
            T_TH1 = T_NUMBER_1MS >> 8;
            count2 += 1;
            if (count2 >= 50)
            {
                count2 = 0;
                if (LED1 == 1)
                {
                    LED1 = 0;
                }
                else
                {
                    LED1 = 1;
                }
            }
        }
    }
}