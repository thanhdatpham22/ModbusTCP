/*
 * Modbus_Parser.c
 *
 *  Created on: Oct 13, 2026
 *      Author: admin
 */
#include "../Drivers/Modbus_Parser.h"
#include "../Drivers/Modbus.h"
#include "../Drivers/Modbus_Database.h"

#include "lwip/pbuf.h"
#include <string.h>

static err_t MB_ParseRequest(uint8_t *rxBuf, uint16_t length, MB_Request_t *req);
static err_t MB_ProcessFunction(struct tcp_pcb *pcb, MB_Request_t *req);
err_t MB_SendException(struct tcp_pcb *pcb, MB_Request_t *req, uint8_t exception);
static void MB_PrintHex(const uint8_t *buf, uint16_t len);


err_t MB_Parser(struct tcp_pcb *pcb, uint8_t *rxBuf, uint16_t length)
{
    MB_Request_t req;

    printf("\r\n========== Modbus TCP Request ==========\r\n");
    MB_PrintHex(rxBuf, length);

    /* Parse incoming request */
    if(MB_ParseRequest(rxBuf, length, &req) != ERR_OK)
    {
        printf("Request Parsing Failed!\r\n");
        return ERR_VAL;
    }

    printf("Request Parsed Successfully\r\n");

    /* Execute requested function */
    return MB_ProcessFunction(pcb, &req);
}



static err_t MB_ParseRequest(uint8_t *rxBuf, uint16_t length, MB_Request_t *req)
{
    if(length < 8)
    {
        printf("Error: Packet too short (%d bytes)\r\n", length);
        return ERR_VAL;
    }

    req->TransactionID = ((uint16_t)rxBuf[0] << 8) | rxBuf[1];
    req->ProtocolID    = ((uint16_t)rxBuf[2] << 8) | rxBuf[3];
    req->Length        = ((uint16_t)rxBuf[4] << 8) | rxBuf[5];
    req->UnitID        = rxBuf[6];
    req->FunctionCode  = rxBuf[7];

    req->Data = &rxBuf[8];
    req->DataLength = length - 8;

    printf("Transaction ID : %u\r\n", req->TransactionID);
    printf("Protocol ID    : %u\r\n", req->ProtocolID);
    printf("Length         : %u\r\n", req->Length);
    printf("Unit ID        : %u\r\n", req->UnitID);
    printf("Function Code  : 0x%02X\r\n", req->FunctionCode);
    printf("Data Length    : %u\r\n", req->DataLength);

    if(req->ProtocolID != 0)
    {
        printf("Error: Invalid Protocol ID\r\n");
        return ERR_VAL;
    }

    return ERR_OK;
}

static err_t MB_ProcessFunction(struct tcp_pcb *pcb, MB_Request_t *req)
{
    printf("Processing Function Code : 0x%02X\r\n", req->FunctionCode);

    switch(req->FunctionCode)
    {
    	case MB_FC_READ_COILS:
    		return MB_FC01_ReadCoils(pcb, req);
    	case MB_FC_READ_DISCRETE_INPUTS:
			return MB_FC02_ReadDisInputs(pcb, req);
    	case MB_FC_READ_HOLDING_REGS:
    		return MB_FC03_ReadHoldingRegisters(pcb, req);
    	case MB_FC_READ_INPUT_REGS:
			return MB_FC04_ReadInputRegisters(pcb, req);
    	case MB_FC_WRITE_SINGLE_COIL:
    		return MB_FC05_WriteSingleCoil(pcb, req);
    	case MB_FC_WRITE_SINGLE_REG:
			return MB_FC06_WriteSingleRegister(pcb, req);
    	case MB_FC_WRITE_MULTI_COILS:
    		return MB_FC15_WriteMultipleCoils(pcb, req);
		case MB_FC_WRITE_MULTI_REGS:
			return MB_FC16_WriteMultipleRegisters(pcb, req);

        default:
            printf("Unsupported Function Code\r\n");
            return MB_SendException(pcb, req, MB_EX_ILLEGAL_FUNCTION);
    }
}

err_t MB_SendException(struct tcp_pcb *pcb, MB_Request_t *req, uint8_t exception)
{
    uint8_t tx[9];

    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    tx[2] = 0;
    tx[3] = 0;

    tx[4] = 0;
    tx[5] = 3;

    tx[6] = req->UnitID;

    tx[7] = req->FunctionCode | 0x80;

    tx[8] = exception;

    printf("Sending Exception Response\r\n");
    printf("Exception Code : 0x%02X\r\n", exception);
    printf("TX Response (%u bytes):\r\n", (unsigned)sizeof(tx));
    MB_PrintHex(tx, sizeof(tx));
    if(tcp_write(pcb, tx, sizeof(tx), TCP_WRITE_FLAG_COPY) != ERR_OK)
    {
        printf("tcp_write() Failed\r\n");
        return ERR_MEM;
    }

    tcp_output(pcb);

    printf("Response Sent Successfully\r\n");

    return ERR_OK;
}


static void MB_PrintHex(const uint8_t *buf, uint16_t len)
{
    printf("RX Data (%d Bytes):\r\n", len);

    for(uint16_t i = 0; i < len; i++)
    {
        printf("%02X ", buf[i]);

        if((i + 1) % 16 == 0)
            printf("\r\n");
    }

    printf("\r\n");
}
