/*
 * Modbus_Database.h
 *
 *  Created on: Oct 13, 2026
 *      Author: admin
 */

#ifndef DRIVERS_MODBUS_DATABASE_H_
#define DRIVERS_MODBUS_DATABASE_H_
#include "main.h"
#include "lwip/tcp.h"
#include "stdint.h"
#include "../Drivers/Modbus.h"
#include <string.h>
#define MB_HOLDING_REG_COUNT    100 //0x3
#define MB_INPUT_REG_COUNT      100

#define MB_COIL_COUNT           20
#define MB_DIS_INPUT_COUNT      20

extern uint16_t MB_HoldingRegs[MB_HOLDING_REG_COUNT];
extern uint16_t MB_InputRegs[MB_INPUT_REG_COUNT];

extern uint16_t MB_Coils[MB_COIL_COUNT];
extern uint16_t MB_DisInputs[MB_DIS_INPUT_COUNT];

err_t MB_FC01_ReadCoils(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC02_ReadDisInputs(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC03_ReadHoldingRegisters(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC04_ReadInputRegisters(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC05_WriteSingleCoil(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC06_WriteSingleRegister(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC07_ReadExceptionStatus(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC15_WriteMultipleCoils(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_FC16_WriteMultipleRegisters(struct tcp_pcb *pcb, MB_Request_t *req);

#endif /* DRIVERS_MODBUS_DATABASE_H_ */
