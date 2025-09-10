
#ifndef _I2C_H
#define _I2C_H
#include "C8051F380_defs.h"
#include "compiler_defs.h"

/************************** I2C ********************************/
/***************************************************************/
/* use this bits to control which bits connected to the I2C bus */

sbit SDA=P1^1;               
sbit SCL=P1^0;
void startI2c(void);
void stopI2c(void);
bit writeByteI2c(unsigned char outchar);
unsigned char readByteI2c(bit ask_master);

#endif



