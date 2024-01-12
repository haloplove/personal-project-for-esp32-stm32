#include <Arduino.h>
// 		* 串口位置控制命令格式：

// 	地址 0xFD  2字节方向+速度  加速度   3字节脉冲   CRC
// 	[0]  [1]     [2]  [3]       [4]   [5] [6] [7]  [8]

// **********************************************************/
// 		cmd[0] = 0x01;	/* 地址 */
// 		cmd[1] = 0xFD;	/* 功能码 */
// 		cmd[2] = 0x12;	/* 方向半字节 + 速度高半字节 */
// 		cmd[3] = 0xFF;	/* 速度字节,( (uint16_t)(cmd[2] & 0x0F) << 8 | (uint16_t)cmd[3] )组成速度   0-254*/
// 		cmd[4] = 0x40;	/* 加速度 */
// 		cmd[5] = 0x00;	/* 脉冲数高字节 */
// 		cmd[6] = 0x0C;	/* 脉冲数中字节 */
// 		cmd[7] = 0x80;	/* 脉冲数低字节 */
// 		cmd[8] = 0x6B;;	/* 固定校验字节 */
//最大4FF

#define mo_add_1 0x01
#define mo_add_2 0x02
#define mo_add_3 0x03
#define mo_add_4 0x04
#define mo_speed_active 10
#define mo_speed_halt 0

// 1正转  2停止  3反转

// unsigned char m_a_1[236] = {
//     mo_add_1, 0XFD, 0X10, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_a_2[236] = {
//     mo_add_1, 0XFD, 0X10, 0X00, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_a_3[236] = {
//     mo_add_1, 0XFD, 0X00, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_b_1[236] = {
//     mo_add_2, 0XFD, 0X10, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_b_2[236] = {
//     mo_add_2, 0XFD, 0X10, 0X00, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_b_3[236] = {
//     mo_add_2, 0XFD, 0X00, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_c_1[236] = {
//     mo_add_3, 0XFD, 0X10, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_c_2[236] = {
//     mo_add_3, 0XFD, 0X10, 0X00, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_c_3[236] = {
//     mo_add_3, 0XFD, 0X00, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_d_1[236] = {
//     mo_add_4, 0XFD, 0X10, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_d_2[236] = {
//     mo_add_4, 0XFD, 0X10, 0X00, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// unsigned char m_d_3[236] = {
//     mo_add_4, 0XFD, 0X00, 0XE8, 0X00, 0X00, 0X0C, 0X80, 0X6B};

// void stright(void)
// {
//     Serial2.write(m_a_3, 9);
//     delay(3);
//     Serial2.write(m_b_1, 9);
//     delay(3);
//     Serial2.write(m_c_3, 9);
//     delay(3);
//     Serial2.write(m_d_1, 9);
// }

// void back(void)
// {
//     Serial2.write(m_a_1, 9);
//     delay(3);
//     Serial2.write(m_b_3, 9);
//     delay(3);
//     Serial2.write(m_c_1, 9);
//     delay(3);
//     Serial2.write(m_d_3, 9);
// }

// void left(void)
// {
//     Serial2.write(m_a_1, 9);
//     delay(3);
//     Serial2.write(m_b_1, 9);
//     delay(3);
//     Serial2.write(m_c_3, 9);
//     delay(3);
//     Serial2.write(m_d_3, 9);
// }

// void right(void)
// {
//     Serial2.write(m_a_3, 9);
//     delay(3);
//     Serial2.write(m_b_3, 9);
//     delay(3);
//     Serial2.write(m_c_1, 9);
//     delay(3);
//     Serial2.write(m_d_1, 9);
// }

//速度控制模式
void SpeedControl_send(uint8_t add, uint16_t speed, uint8_t accel, uint8_t dir)
{
    uint8_t data[6] = {0};
    data[0] = add;
    data[1] = 0XF6;
    if (speed > 0X4FF)
        speed = 0X4FF;
    data[2] = (uint8_t)(dir << 4) | (speed >> 8);
    data[3] = (uint8_t)speed;
    data[4] = accel;
    data[5] = 0X6B;
    Serial2.write(data, 6);
}

//位置控制模式
void PositionControl_send(uint8_t add, uint16_t speed, uint8_t accel, uint32_t PWM, uint8_t dir)
{
    uint8_t data[9] = {0};
    data[0] = add;
    data[1] = 0XFD;
    if (speed > 0X4FF)
        speed = 0X4FF;
    data[2] = (uint8_t)(dir << 4) | (speed >> 8);
    data[3] = (uint8_t)speed;
    data[4] = accel;
    data[5] = (uint8_t)(PWM >> 16);
    data[6] = (uint8_t)(PWM >> 8);
    data[7] = (uint8_t)PWM;
    data[8] = 0X6B;
    Serial2.write(data, 9);
}

void stright(void)
{
    //前进
    SpeedControl_send(mo_add_1, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_2, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_3, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_4, mo_speed_active, 0XFF, 1);
}
void back(void)
{
    //倒退
    SpeedControl_send(mo_add_1, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_2, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_3, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_4, mo_speed_active, 0XFF, 0);
}

void halt()
{
    //停止
    SpeedControl_send(mo_add_1, mo_speed_halt, 0X00, 0);
    delay(3);
    SpeedControl_send(mo_add_2, mo_speed_halt, 0X00, 1);
    delay(3);
    SpeedControl_send(mo_add_3, mo_speed_halt, 0X00, 0);
    delay(3);
    SpeedControl_send(mo_add_4, mo_speed_halt, 0X00, 1);
}
void left(void)
{
    //左移
    SpeedControl_send(mo_add_1, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_2, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_3, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_4, mo_speed_active, 0XFF, 1);
}
void right(void)
{
    //右移
    SpeedControl_send(mo_add_1, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_2, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_3, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_4, mo_speed_active, 0XFF, 0);
}

void turn_left(void)
{
    //前进
    SpeedControl_send(mo_add_1, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_2, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_3, mo_speed_active, 0XFF, 0);
    delay(3);
    SpeedControl_send(mo_add_4, mo_speed_active, 0XFF, 0);
}

void turn_right(void)
{
    //前进
    SpeedControl_send(mo_add_1, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_2, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_3, mo_speed_active, 0XFF, 1);
    delay(3);
    SpeedControl_send(mo_add_4, mo_speed_active, 0XFF, 1);
}