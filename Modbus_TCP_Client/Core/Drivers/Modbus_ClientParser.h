/*
 * Modbus_ClientParser.h
 *
 *  Created on: Oct 13, 2026
 *      Author: admin
 */

#ifndef DRIVERS_MODBUS_CLIENTPARSER_H_
#define DRIVERS_MODBUS_CLIENTPARSER_H_

#include "lwip/tcp.h"

err_t MB_Client_Parser(struct tcp_pcb*,uint8_t*,uint16_t);
err_t MB_ReadHoldingRegisters(uint16_t txnID, uint8_t unitID, uint16_t addr, uint16_t qty);

#endif /* DRIVERS_MODBUS_CLIENTPARSER_H_ */
