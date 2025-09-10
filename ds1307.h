//===================DS1307.h=======================

#ifndef _1307_H
#define _1307_H
bit write_ds1307(unsigned char add,unsigned char outchar);
unsigned char readDs1307(unsigned char add);
void setupTime(U8 hour,U8 minute, U8 second);
void printTime(U8 hour,U8 minute, U8 second);
void printTime_AM_PM(U8 hour,U8 minute, U8 second);
void setupdaydate(U8 day,U8 date, U8 month, U8 year);
void printdaydate(U8 day,U8 date, U8 month, U8 year);
#endif
//==================================================
