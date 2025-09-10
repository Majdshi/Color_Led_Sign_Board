//===================== Example11_3.c ==============
#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"
#include "ds1307.h"
#include "uart0.h"

void Init_Device(void);
int main_display(void);
int RTC(void);
void time_page(void);



unsigned char hour;
unsigned char min ;
unsigned char sec ;

unsigned char day;
unsigned char date;
unsigned char month;
unsigned char year;

unsigned char rem_hour=12;
unsigned char rem_min=0;


void main(void){	
	int page_sel=0;
	Init_Device();
	initSYS();
	
	
	//setupTime(1,0,0); //seting start time
  setupdaydate(2,21,4,25); // seting start date
  hour=1;
	min=0;
  sec=0;
  
	while (1){
		
		switch(page_sel){
			case 0: setupTime(hour,min,sec);
	hour = readDs1307(0x02);  // real hour update
  min = readDs1307(0x01); // real min update
  sec = readDs1307(0x00); // real second update
setupTime(hour,min,sec);
							page_sel= main_display();
							break;
			case 1: page_sel=RTC();
							break;
			
		  
		
		}
		
					}
}
