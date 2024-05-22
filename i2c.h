#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "DAVE.h"

#define SLAVE_ADDRESS 0x38<<1

void StartMeasurement();
float Get_Temperature();
float Get_Humidity();
void updateTempAndHumidity(float* temp, float* humidity);

#endif /* I2C_H_ */
