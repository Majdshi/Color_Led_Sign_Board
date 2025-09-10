#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "i2c.h"
#include "initsys.h"


#define AHT10_ADDRESS 0x38
#define AHT10_RESET_CMD 0xBA
#define AHT10_START_MEASUREMENT_CMD 0xAC

void Init_Device(void);

unsigned int d[7];
bit initAHT10(U8 add){
bit ask=1;
startI2c();
    if (!writeByteI2c(add)) {             // 0011,1000 =0x38 --> AHT10  address
       ask = writeByteI2c(AHT10_RESET_CMD); // sending 1011,1010 = 0xBA doing a soft reset for the AHT110 system
			
    }
    stopI2c();
		delay_ms(20);
   return ask;
}

void triggerMeasurement(U8 add){

startI2c();
    if (!writeByteI2c(add)) {             // 0011,1000 =0x38 --> AHT10  address
       writeByteI2c(AHT10_START_MEASUREMENT_CMD); 			// trigger command
			 writeByteI2c(0x33);                              //normal read config
			 writeByteI2c(0x00);                              //padding
    }
    stopI2c();
		delay_ms(75);
}





void readDataFromAHT10(){
 unsigned char i;
startI2c();
if (!writeByteI2c((AHT10_ADDRESS << 1) | 1)) {


    for (i = 0; i < 7; i++) {
        d[i] = readByteI2c(i==6); // if i==6 ---> NACK on last byte
    }
   
}
stopI2c();

}



float Humidity_data(void){
U32 rawHumidity = ((U32)d[1] << 12) |
                       ((U32)d[2] << 4) |
                       ((U32)(d[3] & 0xF0) >> 4);

return ((rawHumidity * 100.0)/ 1048576.0);	
}

float Temperature_data(void){
U32 rawTemperature = ((U32)(d[3] & 0x0F)<<16) |
                          ((U32)d[4] << 8) |
                          (d[5]);

return (((rawTemperature * 200.0)/ 1048576.0)-50);	
}