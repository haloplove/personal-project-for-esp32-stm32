#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

char LED_mode[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
unsigned int account = 0;
void main()
{
    P0 = LED_mode[account / 10];
    P3 = LED_mode[account % 10];
    P2 = 0x20;
    unsigned int flag = 0x20;
    while (1)
    {
        // if (P2_7 == 0)
        // {
        //     delay_ms(10);
        //     if (P2_7 == 0)
        //     {
        //         account++;
        //         P0 = LED_mode[account / 10];
        //         P3 = LED_mode[account % 10];
        //         while (P2_7 == 0)
        //             ;
        //     }
        // }

        P0 = LED_mode[1];
        delay_ms(1);
        flag = flag >> 1;
        P2 = flag;
        P0 = LED_mode[0];
        delay_ms(1);
        flag = flag << 1;
        P2 = flag;
    }
}