#ifndef MP3_H
#define MP3_H
#include "compiler_defs.h"
#include "C8051F380_defs.h"
void Init_Device(void);
void sendCMptoMp3(char commandByte, char dataHigh, char dataLow);
void resetMp3(void);
void nextSong();
void volumeUp();
void volumeDown();
void setVolume(char level);
void playFromFolder(char numbFolder, char numbSong);
void clock_reder(unsigned char h, unsigned char m);
#endif