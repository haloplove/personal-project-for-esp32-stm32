

keil与sdcc语法差异


1.sbit
keil C：
sbit SDA = P1 ^ 5;

sbit CLK_ST = P1 ^ 6;

SDCC:
#define SDA P1_5

#define CLK_ST P1_6



2.中断
在sdcc中，中断处理函数使用关键字__interrupt标识，后面跟上中断号

keil C：
void SerialComm()  interrupt 4

SDCC:
void SerialComm()  __interrupt 4

3._nop_()
在sdcc中，没有_nop_()，可以用 #define _nop_() __asm NOP __endasm 替代

SDCC:
#define _nop_() __asm NOP __endasm


	                Keil	                 SDCC
寄存器声明	     sfr P0 = 0x80	       __sfr__at 0x80 P0
位寄存器声明	sbit P00 = P0^0	       __sbit__at 0x80 P00
位寄存器声明	sbit P01 = P0^1	       __sbit__at 0x81 P01
片外RAM声明	        xdata	              __xdata

