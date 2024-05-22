#include <stdio.h>
#include "DAVE.h"
#include "interrupt.h"
#include "customtime.h"

extern time_struct_t runTime;

void interrupcao(){
	//DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
	runTime.millis++;
	updateTime(&runTime);
}

void btn_interrupt(){
	DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
}
