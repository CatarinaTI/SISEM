/*
 * main.c
 *
 *  Created on: 2024 Feb 28 14:45:30
 *  Author: Catarina
 */

#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "main.h"
#include "enums.h"
#include "uart.h"
#include "i2c.h"
#include "can.h"
#include "interrupt.h"

extern I2C_MASTER_t I2C_MASTER_0; // Declaração do I2C_0
time_struct_t runTime = {0};

int main(void){
	DAVE_STATUS_t dave_status;
	dave_status = DAVE_Init();           /* Initialization of DAVE APPs  */
	bool ath10read = false;

	static alinea_enum_t alinea = a;
	static alineaB_enum_t alineaB = l;

	//unsigned short int modeTime = 10;    // Duração de cada modo

	if (dave_status != DAVE_STATUS_SUCCESS){
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");
		while(1U){}
	}

	while(1U){  //Application loop
		//Tarefa C
		if(runTime.millis % 100 > 90 && ath10read == false){
			updateTempAndHumidity(&temperature, &humidity);
			ath10read = true;
		}else{
			ath10read = false;
		}


		//Parte A
		if (!UART_IsRXFIFOEmpty(&UART_0)) {
			// Recebe o dado da FIFO de recebimento da UART
			receivedData = UART_GetReceivedWord(&UART_0);

			// Verifica o comando recebido
			switch (receivedData) {
	       	   	   case CONFIG_MODE_A:
	       	   		   // Envia a resposta de confirmação do modo A pela UART
	                    UART_Transmit(&UART_0, (uint8_t*)confirm_mode_a, strlen(confirm_mode_a));
	       	   		   alinea = a;
	       		   break;
	       	   	   case CONFIG_MODE_B:
	       	   		   // Envia a resposta de confirmação do modo B pela UART
	                    UART_Transmit(&UART_0, (uint8_t*)confirm_mode_b, strlen(confirm_mode_b));
	       	   		   alinea = b;
	       		   break;
	       	   	   case CONFIG_MODE_C:
	       	   		   // Envia a resposta de confirmação do modo C pela UART
	                    UART_Transmit(&UART_0, (uint8_t*)confirm_mode_c, strlen(confirm_mode_c));
	       	   		   alinea = c;
	       		   break;
	       	   	   case READ_POTENTIOMETER:
	       	   		   // Envia a resposta de confirmação de leitura do potenciômetro pela UART
	                    UART_Transmit(&UART_0, (uint8_t*)confirm_read_potentiometer, strlen(confirm_read_potentiometer));
	       	   		   alineaB = p;
	       		   break;
	       	   	   case READ_LDR:
	       	   		   // Envia a resposta de confirmação de leitura do LDR pela UART
	                    UART_Transmit(&UART_0, (uint8_t*)confirm_read_ldr, strlen(confirm_read_ldr));
	       	   		   alineaB = l;
	       		   break;
	       	   	   case READ_TEMPERATURE:
	       	   		   // Envia a resposta de confirmação de leitura do LDR pela UART
	                    UART_Transmit(&UART_0, (uint8_t*)confirm_read_temperature, strlen(confirm_read_temperature));
	       	   		    char tempMessage[30];
	                    sprintf(tempMessage, "%.2f C\r\n", temperature); // Formatar resposta
	                    UART_Transmit(&UART_0, (uint8_t*)tempMessage, strlen(tempMessage));

	       	   	   break;
	       	   	   case READ_HUMIDITY:
	       	   			// Envia a resposta de confirmação de leitura do LDR pela UART
	                    UART_Transmit(&UART_0, (uint8_t*)confirm_read_humidity, strlen(confirm_read_humidity));
				        char humMessage[30];
	                    sprintf(humMessage, "%.2f %%\r\n", humidity);
	                    UART_Transmit(&UART_0, (uint8_t*)humMessage, strlen(humMessage));
	               break;
	       	   	   default:
	       	   		   //Erro
	                    UART_Transmit(&UART_0, (uint8_t*)error, strlen(error));
	       		   break;
	       }
	  }

	  //Tarefa B (LED Externo)
	  valor_LDR = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Resultado_LDR);
	  valor_potenciometro = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Resultado_potenciometro);
	  /*
	  if(runTime.seconds % (modeTime*5) >= 0 && runTime.seconds % (modeTime*5) < modeTime){
	  	alineaB = p;
	  }
	  else{
	  	alineaB = l;
	  }
	*/

	  if(alineaB == l){
		  result = valor_LDR;
	  } else if(alineaB == p) {
		  result = valor_potenciometro;
	  }

	  uint32_t dutyCycle = (result * (8000 - 1500)) / 4096 + 1500;
	  PWM_SetDutyCycle(&PWM_0, dutyCycle);


	  //Tarefa_A
	  /*
	  if(runTime.seconds % (modeTime*3) >= 0 && runTime.seconds % (modeTime*3) < modeTime){
		  alinea = a;
	  }else if(runTime.seconds % (modeTime*3) >= modeTime && runTime.seconds % (modeTime*3) < (modeTime*2)){
		  alinea = b;
	  }else{
		  alinea = c;
	  }
	  */
	  switch(alinea){
	  case a:
		  PIN_INTERRUPT_Disable(&PIN_INTERRUPT_0);
		  //timer_status = TIMER_Start(&TIMER_0);
		  if(runTime.seconds % 2 == 0)
			  DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
		  break;
	  case b:
		  //timer_status = TIMER_Stop(&TIMER_0);
		  PIN_INTERRUPT_Enable(&PIN_INTERRUPT_0);
		  PIN_INTERRUPT_SetEdgeSensitivity(&PIN_INTERRUPT_0, PIN_INTERRUPT_EDGE_RISING);
		  break;
	  case c:
		  //timer_status = TIMER_Stop(&TIMER_0);
		  PIN_INTERRUPT_Enable(&PIN_INTERRUPT_0);
		  PIN_INTERRUPT_SetEdgeSensitivity(&PIN_INTERRUPT_0, PIN_INTERRUPT_EDGE_FALLING);
		  break;
	  default:
		  break;
	  }
  }
}
