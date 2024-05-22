/*
 * main.h
 *
 *  Created on: Mar 6, 2024
 *      Author: Ricardo França
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "customtime.h"

int main(void);
void interrupcao();
void btn_interrupt();

XMC_VADC_RESULT_SIZE_t result;
XMC_VADC_RESULT_SIZE_t valor_LDR;
XMC_VADC_RESULT_SIZE_t valor_potenciometro;
float humidity, temperature;

#endif /* MAIN_H_ */
