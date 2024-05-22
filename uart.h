#ifndef UART_H_
#define UART_H_
#include <stdio.h>
#include "DAVE.h"
uint8_t receivedData;

// Definições dos comandos    //Tarefa A
#define CONFIG_MODE_A 1 //modo a piscar
#define CONFIG_MODE_B 2 //Falling edge
#define CONFIG_MODE_C 3 //Rising edge
//Tarefa B
#define READ_POTENTIOMETER 4 //Variar o LED usando o potenciometro
#define READ_LDR 5 //Variar o LED usando o PWM
//Tarefa C
#define READ_TEMPERATURE 6 //Obter temperatura registada pelo sensor I2C
#define READ_HUMIDITY 7 //Obter humidade registada pelo sensor I2C

// Definições das respostas
char confirm_mode_a[] = "Modo alternando\n";
char confirm_mode_b[] = "Modo rising edge\n";
char confirm_mode_c[] = "Modo falling edge\n";
char confirm_read_potentiometer[] = "Modo Potenciometro\n";
char confirm_read_ldr[] = "Modo LDR\n";
char confirm_read_temperature[] = "Temperatura: ";
char confirm_read_humidity[] = "Humidade: ";
char error[] = "Código inválido\n";

#endif /* UART_H_ */
