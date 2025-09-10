#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "i2c.h"
#include "initsys.h"

#define usec 8  // delay clk 1/2T

// Starts I2C serial transfer
void startI2c(void){
	SDA=1;
	SCL=1;
	delay_us(usec);
	SDA=0;
	delay_us(usec);
	SCL=0;
}

// Stops I2C serial transfer 
void stopI2c(void){
	SDA=0;
	SCL=1;
	delay_us(usec);
	SDA=1;
	delay_us(usec);
}


bit writeByteI2c(U8 outchar){
    bit ack;
	U8 i;
	// Write 8 bits 
   for(i=0;i<8;i++)	{
	     outchar=outchar<<1;
           SDA=CY;
  	      delay_us(usec);
		SCL=1;
		delay_us(usec); 
		SCL=0; 	
	}

	delay_us(usec);
	// Get ACK bit 
	SDA=1;
	SCL=1;
	delay_us(usec);
	ack=SDA;	
	SCL=0;
	delay_us(usec);
	return(ack);	
}


U8 readByteI2c(bit ask_master) {
	U8 i2cData=0,i;

	SDA=1;
	for(i=7 ; i!=0xff ; i--){
		SCL=1; 
		delay_us(usec);  
		//bit7_i2c=SDA;
		if(SDA)
  		 i2cData|=1<<i;
		SCL=0;  
		delay_us(usec);
	}

	// Acknowledge sender if this is not the last byte. 
	SDA=ask_master;
	delay_us(usec);
	SCL=1;
	delay_us(usec); 
	SCL=0;
	delay_us(usec);
	return(i2cData);
}
