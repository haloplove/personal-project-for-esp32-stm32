#include "moto.h"

#define time 1000

void setup()
{
    Serial2.begin(38400);
    Serial.begin(115200);
}

char cmd = 0;
char bz = 0;
void loop()
{
    cmd = Serial.read();

    // Serial.write(cmd);
    // delay(500);

    if (cmd == '5' || cmd == '6')
    {
        stright();
        delay(16000);
        bz = cmd;
        halt();
        delay(3);
    }

    if (cmd == '8')
    {
        stright();
        delay(4500);
        delay(3);
        if (bz == '5')
        {
            left();
            delay(10000);
            halt();
            delay(3);
        }
        else if (bz == '6')
        {
            right();
            delay(10000);
            halt();
            delay(3);
        }
    }

    if (cmd == '9')
    {
        if (bz == '5')
        {
            right();
            delay(10000);
            halt();
            delay(3);
        }
        else if (bz == '6')
        {

            left();
            delay(10000);
            halt();
            delay(3);
        }
        back();
        delay(20000);
        halt();
        delay(3);
    }

    halt();
    delay(3);
    // stright();
    // delay(1000);
    // back();
    // delay(1000);

    // Serial2.write(m_a_t_1, 9);
    // delay(3);
    // Serial2.write(m_b_3, 9);
    // delay(1000);
    // Serial2.write(m_a_t, 9);
    // delay(1000);

    // left();
    // delay(500);
    // right();
    // delay(500);

    // stright();
    // delay(time);

    // halt();
    // delay(time);

    // back();
    // delay(time);

    // halt();
    // delay(time);

    // left();
    // delay(time);

    // halt();
    // delay(time);

    // right();
    // delay(time);

    // halt();
    // delay(time);
}
