#include "STC89xx.h"
#include "delay.h"
#include "mcs51/8052.h"

void main()
{
    unsigned char key = 0;

    while (1)
    {
        key = P0 & 0x1f;
        if (key != 0x1f)
        {
            if (P0_4 == 0)
            {
                key = P0 & 0x1f;
            }
            P2 = key;
        }
    }
}
