#include "moto.h"

unsigned char m_a_t_1[236] = {
    0x01, 0XFD, 0X12, 0X7F, 0X00, 0X00, 0X0C, 0X80, 0X6B};

unsigned char m_a_t[236] = {
    0x01, 0XFD, 0X10, 0X00, 0X00, 0X00, 0X0C, 0X80, 0X6B};

void setup()
{
    Serial2.begin(38400);
}

void loop()
{
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

    //前进
    SpeedControl_send(0X01, 10, 0XFF, 1);
    delay(3);
    SpeedControl_send(0X02, 10, 0XFF, 0);
    delay(3);
    SpeedControl_send(0X03, 10, 0XFF, 1);
    delay(3);
    SpeedControl_send(0X04, 10, 0XFF, 0);
    delay(1000);
    //停止
    SpeedControl_send(0X01, 0, 0X00, 0);
    delay(3);
    SpeedControl_send(0X02, 0, 0X00, 1);
    delay(3);
    SpeedControl_send(0X03, 0, 0X00, 0);
    delay(3);
    SpeedControl_send(0X04, 0, 0X00, 1);
    delay(1000);

    SpeedControl_send(0X01, 10, 0XFF, 0);
    delay(3);
    SpeedControl_send(0X02, 10, 0XFF, 1);
    delay(3);
    SpeedControl_send(0X03, 10, 0XFF, 0);
    delay(3);
    SpeedControl_send(0X04, 10, 0XFF, 1);
    delay(1000);

    SpeedControl_send(0X01, 0, 0X00, 0);
    delay(3);
    SpeedControl_send(0X02, 0, 0X00, 1);
    delay(3);
    SpeedControl_send(0X03, 0, 0X00, 0);
    delay(3);
    SpeedControl_send(0X04, 0, 0X00, 1);
    delay(1000);
}
