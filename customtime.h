/*
 * time.h
 *
 *  Created on: Mar 6, 2024
 *      Author: Ricardo França
 */

#ifndef CUSTOMTIME_H_
#define CUSTOMTIME_H_

#include "stdint.h"

typedef struct time_struct{
	uint32_t millis;
	unsigned short seconds;
	unsigned short minutes;
	unsigned short hours;
	uint8_t days; // days
} time_struct_t;

void updateTime(time_struct_t *time);



#endif /* CUSTOMTIME_H_ */
