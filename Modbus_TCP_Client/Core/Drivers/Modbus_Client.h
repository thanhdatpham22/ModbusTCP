/*
 * Modbus_Client.h
 *
 *  Created on: Oct 13, 2026
 *      Author: admin
 */

#ifndef DRIVERS_MODBUS_CLIENT_H_
#define DRIVERS_MODBUS_CLIENT_H_


#include "lwip/tcp.h"


err_t TCP_SendRequest(uint8_t *txBuf, uint16_t length);
void Modbus_Client_Process(char *serverIP, uint16_t port);


#endif /* DRIVERS_MODBUS_CLIENT_H_ */
