/*
 * CAN.c
 *
 *  Created on: 2024 May 22
 *  Author: RGoncalves
 */

#include "CAN.h"
#include "DAVE.h"
#include "Main.h"
#include "customtime.h"
#include "string.h"

// Variável global para eventos
uint8_t trigger_event = 0;

void CAN_Init(void) {
    DAVE_STATUS_t status;

    status = DAVE_Init();  // Inicialização dos APPs DAVE

    if (status != DAVE_STATUS_SUCCESS) {
        XMC_DEBUG("DAVE APPs initialization failed\n");
        while(1U) {}
    }
}

void CAN_Routine(void) {
    if(trigger_event > 0) {
        can_routine();
        trigger_event--;
    }
}



void can_routine(void) {
    uint16_t can_id;
    uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t length = 8;

    // Enviar mensagem a cada 100ms
    if(runTime.millis % 100 == 2) {
        length = 7;
        can_id = 0x310;
        data[0] = 0x12;
        can_transmit(can_id, data, length);
    }

    // Enviar mensagem a cada 500ms
    if(runTime.millis % 500 == 0) {
        length = 3;
        can_id = 0x330;
        data[0] = 0x14;
        can_transmit(can_id, data, length);
    }
}



void CAN_A_RX(void) {
    CAN_NODE_STATUS_t receive_status;
    CAN_NODE_STATUS_t status;
    XMC_CAN_MO_t *MO_Ptr;
    const CAN_NODE_t *HandlePtr1 = &CAN_NODE_A;
    MO_Ptr = HandlePtr1->lmobj_ptr[0]->mo_ptr;

    status = CAN_NODE_MO_GetStatus(HandlePtr1->lmobj_ptr[0]);
    // Verificar o estado pendente de recepção
    if (status & XMC_CAN_MO_STATUS_RX_PENDING) {
        // Limpar a flag
        XMC_CAN_MO_ResetStatus(MO_Ptr, XMC_CAN_MO_RESET_STATUS_RX_PENDING);
        // Ler o objecto de mensagem recebido
        receive_status = CAN_NODE_MO_Receive(HandlePtr1->lmobj_ptr[0]);
        if (receive_status == CAN_NODE_STATUS_SUCCESS) {
            uint8_t data[8] = {0};
            memcpy(data, &MO_Ptr->can_data[0], sizeof(MO_Ptr->can_data[0]));
            memcpy(data + sizeof(MO_Ptr->can_data[0]), &MO_Ptr->can_data[1], sizeof(MO_Ptr->can_data[1]));

            can_transmit(MO_Ptr->can_identifier, data, MO_Ptr->can_data_length);
        }
    }
}

void can_transmit(uint16_t id, uint8_t *data, uint8_t length) {
    XMC_CAN_MO_t *MO_Ptr;
    const CAN_NODE_t *HandlePtr1 = &CAN_NODE_A;
    MO_Ptr = HandlePtr1->lmobj_ptr[1]->mo_ptr;

    // Código da aplicação
    MO_Ptr->can_mo_type = XMC_CAN_MO_TYPE_TRANSMSGOBJ;  // Configurar tipo do objecto de mensagem como tipo de transmissão
    MO_Ptr->can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS;  // Configurar tipo do identificador do MO
    MO_Ptr->can_data_length = length;  // Configurar campo de comprimento dos dados do MO de transmissão CAN
    MO_Ptr->can_identifier = id;
    MO_Ptr->can_data[0] = data[0] + (data[1]<<8) + (data[2]<<16) + (data[3]<<24);  // Configurar os 4 bytes superiores dos dados
    MO_Ptr->can_data[1] = data[4] + (data[5]<<8) + (data[6]<<16) + (data[7]<<24);  // Configurar os 4 bytes inferiores dos dados

    // Alterar a configuração do MO em tempo de execução
    CAN_NODE_MO_Init(HandlePtr1->lmobj_ptr[1]);

    CAN_NODE_STATUS_t mo_tranmit_status;
    CAN_NODE_STATUS_t status;

    MO_Ptr = HandlePtr1->lmobj_ptr[1]->mo_ptr;
    mo_tranmit_status = CAN_NODE_MO_Transmit(HandlePtr1->lmobj_ptr[1]);

    if (mo_tranmit_status == CAN_NODE_STATUS_SUCCESS) {
        // Sucesso na transmissão do objecto de mensagem
        status = CAN_NODE_MO_GetStatus(HandlePtr1->lmobj_ptr[1]);

        if (status & XMC_CAN_MO_STATUS_TX_PENDING) {
            // Limpar a flag de transmissão OK
            XMC_CAN_MO_ResetStatus(MO_Ptr, XMC_CAN_MO_RESET_STATUS_TX_PENDING);
        }
    }

    __NOP();
}
