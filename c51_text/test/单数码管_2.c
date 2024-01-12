#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

void delay_1(unsigned int time)
{
    unsigned int j = 0;
    for (; time > 0; time--)
    {
        for (j = 0; j < 125; j++)
            ;
    }
}

char LED_mode[] = {0x38, 0x5b};

void main()
{
    char led_point = 0;
    while (1)
    {
        P2 = 2 - led_point;
        P0 = LED_mode[led_point];
        led_point = 1 - led_point;
        delay_1(1);
    }
}