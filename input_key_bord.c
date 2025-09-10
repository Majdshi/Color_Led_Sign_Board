#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "ds1307.h"


void input_key_bord(void){

LCD_clearButton();
LCD_fillScreen(BLACK);	

LCD_drawButton(0,15,10,80,70,5,RED,WHITE,"0",4);
LCD_drawButton(1,109,10,80,70,5,RED,WHITE,"1",4);
LCD_drawButton(2,203,10,80,70,5,RED,WHITE,"2",4);
LCD_drawButton(3,297,10,80,70,5,RED,WHITE,"3",4);
LCD_drawButton(4,391,10,80,70,5,RED,WHITE,"4",4);
LCD_drawButton(5,15,100,80,70,5,RED,WHITE,"5",4);
LCD_drawButton(6,109,100,80,70,5,RED,WHITE,"6",4);
LCD_drawButton(7,203,100,80,70,5,RED,WHITE,"7",4);
LCD_drawButton(8,297,100,80,70,5,RED,WHITE,"8",4);
LCD_drawButton(9,391,100,80,70,5,RED,WHITE,"9",4);
LCD_drawButton(10,10,180,220,30,5,GREEN,WHITE,"Send",3);
LCD_drawButton(11,240,180,220,30,5,BLUE,WHITE,"Clear",3);
LCD_drawButton(12,240,215,220,30,5,BLUE,WHITE,"back",3);
LCD_fillRoundRect(10,250,220,50,5,RED);
LCD_fillRoundRect(240,250,220,50,5,WHITE);
LCD_printC(10,220,"Text box:",3,WHITE);




}