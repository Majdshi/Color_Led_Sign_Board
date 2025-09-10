#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "uart1.h"


#define NEXT_SONG 0X01
#define VOLUME_UP 0X04
#define VOLUME_DOWN 0X05
#define SET_VOLUME 0x06
#define CHIP_RESET 0X0c
#define FOLDER 0X0f




void Init_Device(void);
unsigned char packetMp3[]={0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0xEF};

void sendCMptoMp3(char commandByte, char dataHigh, char dataLow){
unsigned char i;
	packetMp3[3]=commandByte;
	packetMp3[5]=dataHigh;
	packetMp3[6]=dataLow;
	for(i=0;i<8;i++){
		PUTCHAR1(packetMp3[i]);
	}
	delay_ms(200);
}

void resetMp3(void){
	            //cmd|   dataH|dataL
	sendCMptoMp3(CHIP_RESET,0,0);
}

void nextSong(){
	sendCMptoMp3(NEXT_SONG,0,0);
}



void playFromFolder(char numbFolder, char numbSong){
	sendCMptoMp3(FOLDER,numbFolder,numbSong);

}
void setVolume(char level){
	                //cmd|dataH|dataL
	sendCMptoMp3(SET_VOLUME,0,level);
}	
	