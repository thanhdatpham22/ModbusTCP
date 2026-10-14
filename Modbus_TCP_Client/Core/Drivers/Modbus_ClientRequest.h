/*
 * Modbus_ClientRequest.h
 *
 *  Created on: Oct 13, 2026
 *      Author: admin
 */

#ifndef DRIVERS_MODBUS_CLIENTREQUEST_H_
#define DRIVERS_MODBUS_CLIENTREQUEST_H_

#include "lwip/tcp.h"

typedef struct
{
    uint16_t StartAddress;
    uint16_t Quantity;

} MB_Request_t;

extern MB_Request_t MB_CurrentRequest;

err_t MB_ReadHoldingRegisters(uint16_t txnID, uint8_t unitID, uint16_t addr, uint16_t qty);
err_t MB_ReadInputRegisters(uint16_t txnID, uint8_t unitID, uint16_t addr, uint16_t qty);
err_t MB_ReadCoils(uint16_t txnID, uint8_t unitID, uint16_t addr, uint16_t qty);
err_t MB_ReadDiscreteInputs(uint16_t txnID, uint8_t unitID, uint16_t addr, uint16_t qty);


#endif /* DRIVERS_MODBUS_CLIENTREQUEST_H_ */
