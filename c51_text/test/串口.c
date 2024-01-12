#include "8051.h"

sbit P1_0 = P1 ^ 0;
void main()
{
    unsigned char i;
    unsigned int j;
    SCON = 0x00;
    i = 0x01;
    for (;;)
    {
        P1_0 = 0;
        SBUF = i;
        while (!TI)
        {
            ;
        }
        for (j = 0; j <= 254; j++)
        {
            ;
        }
        i = i * 2;
        if (i == 0x00)
            i = 0x01;
    }
}