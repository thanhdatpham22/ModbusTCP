/*
 * Modbus_Parser.h
 *
 *  Created on: Oct 13, 2026
 *      Author: admin
 */

#ifndef DRIVERS_MODBUS_PARSER_H_
#define DRIVERS_MODBUS_PARSER_H_


#include "lwip/tcp.h"

err_t MB_Parser(struct tcp_pcb *pcb, uint8_t *rxBuf, uint16_t length);

#endif /* DRIVERS_MODBUS_PARSER_H_ */
