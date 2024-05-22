#ifndef CAN_H_
#define CAN_H_
#include "DAVE.h"
#include "customtime.h"
#include <stdint.h>

extern time_struct_t runTime;

// Declarações das funções
void CAN_Init(void);
void CAN_Routine(void);
void CAN_A_RX(void);
void can_routine(void);
void can_transmit(uint16_t id, uint8_t * data, uint8_t length);
#endif /* CAN_H_ */
