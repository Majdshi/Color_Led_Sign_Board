#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "i2c.h"

//************* DS1307 ***********
U8 bcdToDec(U8 val){
	val=val/16*10 + val%16; 
	return (val);
}

U8 decToBcd(U8 val){
	val= val/10*16 + val%10 ;
	return  val;
}

bit writeDs1307(unsigned char add,unsigned char outchar){
	bit ask=1;
	startI2c();
	if(!writeByteI2c(0xD0)) { // 11010000
		ask=writeByteI2c(add);//address
		ask=writeByteI2c(decToBcd(outchar));
	}
	stopI2c();
	return ask; //if ask=0 data ok
}

unsigned char readDs1307(unsigned char add){
 unsigned char inchar = 0;
 startI2c();
 if(!writeByteI2c(0xD0)){
	writeByteI2c(add);
	stopI2c();
		startI2c();
		writeByteI2c(0xD1); // 11010001
		inchar=readByteI2c(1); //1- read byte and finish
	}
	stopI2c();
	return bcdToDec(inchar);	
}

void setupTime(U8 hour,U8 minute, U8 second){
	writeDs1307(2, hour); //hour
	writeDs1307(1, minute);//minute
	writeDs1307(0, second);//sec
}
void setupdaydate(U8 day,U8 date, U8 month, U8 year){
	writeDs1307(3, day); //day
	writeDs1307(4, date);//date
	writeDs1307(5, month);//month
	writeDs1307(6, year);//year
}

void printdaydate(U8 day,U8 date, U8 month, U8 year){
		
	
	if(day==1){
		
		printf("Sunday ",(int)day);
	}
	else if(day==2){
	
		printf("Monday ",(int)day);
	}
	else if(day==3){
	
		printf("Tuesday ",(int)day);
	}
	else if(day==4){
		
		printf("Wednesday",(int)day);
	}
	else if(day==5){
		
		printf("Thursday ",(int)day);
	}
	else if(day==6){
		
		printf("Friday ",(int)day);
	}
	else if(day==7){
		
		printf("Saturday ",(int)day);
	}

	 printf(" %d%d:",(int)date/10  ,(int)date%10);
   printf("%d%d:",(int)month/10,(int)month%10);
   printf("20%d%d ",(int)year/10,(int)year%10);


}

void printTime_AM_PM(U8 hour,U8 minute, U8 second){
	bit am_pm;

	if((int)hour==0){
		am_pm=0;
		printf("12:");
	}
	else if((int)hour==12){
		am_pm=1;
		printf("12:");
	}
	else if(0 < (int)hour && (int)hour <12){
		am_pm=0;
		if((int)hour <10){
	  printf("0%d:",(int)hour);
    }
		else{
		  printf("%d:",(int)hour);
		}
	}
	else if(12<(int)hour && (int)hour <24){
		am_pm=1;
		if((int)hour%12 <10){
	    printf("0%d:",(int)hour%12);
		;
		}
		else{
			printf("%d:",(int)hour%12);
		}
	}
	
 
 printf("%d%d:",(int)minute/10,(int)minute%10);
 printf("%d%d ",(int)second/10,(int)second%10);
	
	if(am_pm==0){
	
		LCD_print2C(290,90,"AM",3,BLUE,BLACK);
	
	}
	else{

    LCD_print2C(290,90,"PM",3,BLUE,BLACK);

	}
}

void printTime(U8 hour,U8 minute, U8 second){
 printf("%d%d:",(int)hour/10  ,(int)hour%10);
 printf("%d%d:",(int)minute/10,(int)minute%10);
 printf("%d%d ",(int)second/10,(int)second%10);
}

//==================================================