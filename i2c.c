#include "i2c.h"
#include "main.h" // Para o SLAVE_ADDRESS
#include <stdio.h>
#include "DAVE.h"

extern I2C_MASTER_t I2C_MASTER_0; // Declaração do I2C_0

void StartMeasurement() {
    uint8_t tx_buffer[3] = {0xAC, 0x33, 0x00};  // Comando para medir temperatura e humidade
    I2C_MASTER_Transmit(&I2C_MASTER_0, true, SLAVE_ADDRESS, tx_buffer, 3, true);
    while(I2C_MASTER_IsTxBusy(&I2C_MASTER_0));
}

float Get_Temperature() {
    uint8_t rx_buffer[6] = {0x00}; // Buffer para receber dados do sensor
    StartMeasurement();

    I2C_MASTER_Receive(&I2C_MASTER_0, true, SLAVE_ADDRESS, rx_buffer, 6, true, true);
    while(I2C_MASTER_IsRxBusy(&I2C_MASTER_0));

    // Processamento do resultado
    float raw_temp = ((rx_buffer[3] & 0x0f) << 16) + (rx_buffer[4] << 8) + (rx_buffer[5]);

    return (raw_temp / 1048576) * 200 - 50; // Retorna temperatura
}

float Get_Humidity() {
    uint8_t rx_buffer[6] = {0x00}; // Buffer to receive data from the sensor
    StartMeasurement();

    // Receive data from the sensor
    I2C_MASTER_Receive(&I2C_MASTER_0, true, SLAVE_ADDRESS, rx_buffer, 6, true, true);
    while (I2C_MASTER_IsRxBusy(&I2C_MASTER_0));

    // Process the result
    uint32_t raw_hum = (rx_buffer[1] << 12) | (rx_buffer[2] << 4) | (rx_buffer[3] >> 4);
    return (raw_hum / 1048576.0) * 100.0; // Return humidity in percentage
}

void updateTempAndHumidity(float* temp, float* humidity){
    StartMeasurement();
    uint8_t rx_buffer[6] = {0x00}; // Buffer to receive data from the sensor

    // Receive data from the sensor
    I2C_MASTER_Receive(&I2C_MASTER_0, true, SLAVE_ADDRESS, rx_buffer, 6, true, true);
    while (I2C_MASTER_IsRxBusy(&I2C_MASTER_0));

    // Process the results
    float raw_temp = ((rx_buffer[3] & 0x0f) << 16) + (rx_buffer[4] << 8) + (rx_buffer[5]);
    *temp = (raw_temp / 1048576) * 200 - 50;

    uint32_t raw_hum = (rx_buffer[1] << 12) | (rx_buffer[2] << 4) | (rx_buffer[3] >> 4);
    *humidity = (raw_hum / 1048576.0) * 100.0;
}

