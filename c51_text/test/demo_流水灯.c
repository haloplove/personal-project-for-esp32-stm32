#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

#define sbit __sbit

// #define LED P04
//  SBIT(LED0, _P1, 0); //位声明，将P1-0管脚声明为LED0

int flag = 0;
int i = 1;
void time0_int(void) __interrupt 1
{
    P1 = !P1;
}

void main()
{
    // while (1)
    // {
    //     // LED = 0;
    //     // delay_ms(50);
    //     // LED = 1;
    //     // delay_ms(50);
    //     P16 = 0;
    //     delay_ms(100);
    // P0 = ~(0x01 << flag);
    //     P16 = 1;
    //     delay_ms(100);
    //     if (flag == 8)
    //     {
    //         i = -i;
    //     }
    //     else if (flag < 0)
    //     {
    //         i = -i;
    //     }
    //     flag += i;
    // }

    P0 = 0x7f;
    delay_ms(50);
    while (1)
    {
        P0 = _cror_(P0, 1);
        delay_ms(50);
    }
}

// #define uint unsigned int
// #define uchar unsigned char

// uchar LedCtrl[8] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

// void main()
// {

//     uint i;
//     while (1)
//     {
//         for (i = 0; i < 8; i++)
//         {
//             P0 = LedCtrl[i];
//         }
//     }
// }