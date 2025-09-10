#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "MP3.h"
#define time 1000

void clock_reder(unsigned char h, unsigned char m) {
unsigned char hour_index;

   
	  delay_ms(time);

    if (h == 0) {
			  playFromFolder(1,76); 
	  delay_ms(time);
        playFromFolder(1,72); // "12"
		    delay_ms(time);
        playFromFolder(1,m);  // Minute
			  delay_ms(time);
        playFromFolder(1,81); // "AM"
    }
    else if (h == 12) {
			  playFromFolder(1,76); 
	  delay_ms(time);
        playFromFolder(1,72); // "12"
			  delay_ms(time);
        playFromFolder(1,m);  // Minute
			  delay_ms(time);
        playFromFolder(1,82); // "PM"
    }
    else if (h < 12) {
			  playFromFolder(1,76); 
	  delay_ms(time);
        playFromFolder(1,(60 + h)); // 1–11
			  delay_ms(time);
        playFromFolder(1,m);
			  delay_ms(time);
        playFromFolder(1,81);     // "AM"
    }
    else if (h < 24) {
        hour_index = h % 12;
			  playFromFolder(1,76); 
	  delay_ms(time);
        if (hour_index == 0) hour_index = 12; // For 12 PM
        playFromFolder(1,(60 + hour_index)); // 1–11 again
			  delay_ms(time);
        playFromFolder(1,m);
			  delay_ms(time);
        playFromFolder(1,82);              // "PM"
    }
}
