//===================== uart1.h ====================
#include "initsys.h"
#ifndef _UART1_H
#define _UART1_H
#define RI1 (SCON1 & 0x01)//  UART1 receive interrupt flag
#define clrRI1 SCON1 &= ~0x01// clear RI1 receive interrupt flag
#define TI1 (SCON1 & 0x02) //  UART1 Transmit interrupt flag
#define clrTI1 SCON1 &= ~0x02// clear TI1 interrupt flag


void PUTCHAR1(U8 tav);
U8 GETCHAR1(void);

#endif
//==================================================
