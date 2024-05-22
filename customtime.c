/*
 * time.c
 *
 *  Created on: Mar 6, 2024
 *      Author: Ricardo França
 */

#include <customtime.h>

void updateTime(time_struct_t *time){
	if(time->millis == 1000){
		time->seconds++;
		time->millis = 0;
	}
	if(time->seconds == 60){
		time->minutes++;
		time->seconds = 0;
	}
	if(time->minutes == 60){
		time->hours++;
		time->minutes = 0;
	}
	if(time->hours == 24){
		time->days++;
		time->hours = 0;
	}
}
