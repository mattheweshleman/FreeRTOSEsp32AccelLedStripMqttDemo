#ifndef __ADXL345_H__
#define __ADXL345_H__
#include "stdint.h"
#include "stdbool.h"

#ifdef	__cplusplus
extern "C" {
#endif

void initAcc(uint8_t scl_pin, uint8_t sda_pin);
bool getAccelerometerData(int *result);

#ifdef	__cplusplus
}
#endif


#endif
