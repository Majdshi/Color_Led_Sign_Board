// main_display (updated to send 10-byte packet)
// ... (keep your existing includes)
#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "ds1307.h"
#include "MP3.h"
#include "AHT10.h"
#include "uart0.h"

bit reminderPlayed = 0;
bit lastTouchActive = 0;
bit voiceTriggerPending = 0;



extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;

extern unsigned char rem_hour;
extern unsigned char rem_min ;




int main_display(void) {
    unsigned char second, minute, hour ,day ,date, month,  year;
    int sel1=0,x1,y1,temp10;
	  unsigned int hum10;
    float h, temp;
    
    LCD_drawButton(1,180,220,90,90,5,RED,WHITE,"RTC",4);
    LCD_drawButton(2,300,220,90,90,5,RED,WHITE,"clk",4);
    LCD_setTextSize(2);

    initAHT10(0x38<<1);           // AHT10 reset (once)

    while(sel1!=1) {
        LCD_setTextSize(3);

        hour = readDs1307(0x02);  // real hour update
        minute = readDs1307(0x01); // real min update
        min = readDs1307(0x01); // real min update
        second = readDs1307(0x00); // real second update
        LCD_setText2Color(GREEN,BLACK);

        day = readDs1307(0x03);// real day update
        date = readDs1307(0x04); // real date update
        month = readDs1307(0x05); // real month update
        year = readDs1307(0x06); // real year update

        //////////////////////////////////////////////
        // Prepare humidity & temperature (AHT10)
        triggerMeasurement(0x38<<1);      // Trigger new measurement
        readDataFromAHT10();       // Wait for and read response
        h = Humidity_data();
        temp = Temperature_data();

        // Scale humidity and temp to one decimal place as integer (x10)
        // humidity: 0.0 -> 100.0  => 0 -> 1000 (uint16)
        // temp: may be negative, e.g. -5.3 => -53 (int16)
        // round to nearest 0.1
        hum10 = (unsigned int)(h * 10.0f + 0.5f); // e.g. 45.3 -> 453
        temp10 = (int)(temp * 10.0f + (temp >= 0 ? 0.5f : -0.5f)); // e.g. 23.7 -> 237 ; -5.3 -> -53

        ////////////////////////////////////////////
        // SEND 10-BYTE BINARY PACKET OVER UART (PUTCHAR0)
        // Packet order (10 bytes):
        // [0] second (1 byte)
        // [1] minute (1 byte)
        // [2] hour   (1 byte)
        // [3] day    (1 byte)   -- 1=Sun,2=Mon...
        // [4] date   (1 byte)
        // [5] month  (1 byte)
        // [6] hum10 high byte (uint16 >> 8)
        // [7] hum10 low byte  (uint16 & 0xFF)
        // [8] temp10 high byte (int16 >> 8)
        // [9] temp10 low byte  (int16 & 0xFF)

        PUTCHAR0((char)second);
        PUTCHAR0((char)minute);
        PUTCHAR0((char)hour);
        PUTCHAR0((char)day);
        PUTCHAR0((char)date);
        PUTCHAR0((char)month);
        PUTCHAR0((char)year);
				
        // humidity (uint16)
        PUTCHAR0((char)((hum10 >> 8) & 0xFF));   // high byte
        PUTCHAR0((char)(hum10 & 0xFF));          // low byte

        // temperature (int16, two's complement)
        PUTCHAR0((char)((temp10 >> 8) & 0xFF));  // high byte
        PUTCHAR0((char)(temp10 & 0xFF));         // low byte

        ///////////////////////////////////////////////
        //printing the time on the lcd screen
        LCD_setCursor (10, 10);
        printdaydate(day,date,month,year); 

        //printing the date on the lcd screen
        LCD_setCursor (130, 50);
        printTime(hour,minute,second);
        LCD_setText2Color(BLUE,BLACK);
        LCD_setCursor (130,90);
        printTime_AM_PM(hour,minute,second);

        LCD_setText2Color(GREEN,BLACK);
        LCD_setTextSize(2);
        LCD_setCursor (10, 140);
        printf("reminder: %d%d",(int)rem_hour/10,(int)rem_hour%10);
        printf(":%d%d",(int)rem_min/10,(int)rem_min%10);

        LCD_setTextSize(2);
        LCD_setCursor(10,160);
        printf("hu = %.1f",h);   // Display results with one decimal
        LCD_setCursor(10,180);
        printf("temp = %.1fC",temp);

        // Reminders and MP3 handling unchanged
        if (rem_hour == hour && rem_min == min && !reminderPlayed) {
            setVolume(15);
				  	delay_ms(1000);
            playFromFolder(1,96);
					  
            reminderPlayed = 1;  // prevent replay during same minute
        } else if (rem_hour != hour || rem_min != min) {
            reminderPlayed = 0;  // reset flag for next match
            resetMp3();
        }

        x1 = ReadTouchX();
        y1 = ReadTouchY();
        sel1 = ButtonTouch(x1, y1);
        delay_ms(10);
        // Check if finger is touching a button
        if (sel1 == 2 && !lastTouchActive) {
            // Only trigger once per touch press
            voiceTriggerPending = 1;
        }
        lastTouchActive = (sel1 == 2); // store current touch state

        if (voiceTriggerPending) {
            clock_reder(hour, min);
            voiceTriggerPending = 0;
        }
    }
    LCD_clearButton();
    LCD_fillScreen(BLACK);
    return 1;
}
