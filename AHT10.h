
#ifndef AHT10_H
#define AHT10_H
#include "C8051F380_defs.h"
#include "compiler_defs.h"

/************************** AHT10 ********************************/
/***************************************************************/



bit initAHT10(U8 add);
void triggerMeasurement(U8 add);
void readDataFromAHT10();
float Humidity_data(void);
float Temperature_data(void);

#endif
