/*
 * Modbus_Database.c
 *
 *  Created on: Oct 13, 2026
 *      Author: admin
 */
#include "Modbus_Database.h"

uint16_t MB_Coils[MB_COIL_COUNT] = {
        0x12, 0x34, 0x0D
};
uint16_t MB_DisInputs[MB_DIS_INPUT_COUNT] = {0};

uint16_t MB_HoldingRegs[MB_HOLDING_REG_COUNT] = {
		1000, 12345, 62342, 8954, 0, 65535, 45387, 1, 34, 789,
};

uint16_t MB_InputRegs[MB_INPUT_REG_COUNT] = {0};


extern err_t MB_SendException(struct tcp_pcb *pcb, MB_Request_t *req, uint8_t exception);
/*------------------------------Function Code---------------------------------*/


err_t MB_FC01_ReadCoils(struct tcp_pcb *pcb, MB_Request_t *req)
{
    uint16_t startAddr;
    uint16_t quantity;
    uint16_t i;
    uint8_t tx[260];
    uint16_t txLen = 0;

    startAddr = ((uint16_t)req->Data[0] << 8) | req->Data[1];
    quantity  = ((uint16_t)req->Data[2] << 8) | req->Data[3];

    uint8_t byteCount = (quantity + 7) / 8;

    if((quantity == 0) || (quantity > 2000))
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);

    if((startAddr + quantity) > MB_COIL_COUNT)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);

    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    tx[2] = 0;
    tx[3] = 0;

    uint16_t length = 3 + byteCount;
    tx[4] = length >> 8;
    tx[5] = length;

    tx[6] = req->UnitID;

    tx[7] = MB_FC_READ_COILS;
    tx[8] = byteCount;

    txLen = 9;
    memset(&tx[9], 0, byteCount);

    for(i = 0; i < quantity; i++)
    {
        uint16_t coilAddress = startAddr + i;

        uint16_t sourceByte = coilAddress / 8;
        uint8_t sourceBit   = coilAddress % 8;

        uint16_t destByte = i / 8;
        uint8_t destBit   = i % 8;

        if((MB_Coils[sourceByte] >> sourceBit) & 0x01)
        {
            tx[9 + destByte] |= (1 << destBit);
        }
    }

    txLen = 9 + byteCount;

    if(tcp_write(pcb, tx, txLen, TCP_WRITE_FLAG_COPY) != ERR_OK)
        return ERR_MEM;

    tcp_output(pcb);

    return ERR_OK;
}

err_t MB_FC02_ReadDisInputs(struct tcp_pcb *pcb, MB_Request_t *req)
{
    uint16_t startAddr;
    uint16_t quantity;
    uint16_t i;
    uint8_t tx[260];
    uint16_t txLen = 0;

    startAddr = ((uint16_t)req->Data[0] << 8) | req->Data[1];
    quantity  = ((uint16_t)req->Data[2] << 8) | req->Data[3];

    uint8_t byteCount = (quantity + 7) / 8;

    if((quantity == 0) || (quantity > 2000))
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);

    if((startAddr + quantity) > MB_DIS_INPUT_COUNT)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);

    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    tx[2] = 0;
    tx[3] = 0;

    uint16_t length = 3 + byteCount;
    tx[4] = length >> 8;
    tx[5] = length;

    tx[6] = req->UnitID;

    tx[7] = MB_FC_READ_DISCRETE_INPUTS;
    tx[8] = byteCount;

    txLen = 9;
    memset(&tx[9], 0, byteCount);

    for(i = 0; i < quantity; i++)
    {
        uint16_t coilAddress = startAddr + i;

        uint16_t sourceByte = coilAddress / 8;
        uint8_t sourceBit   = coilAddress % 8;

        uint16_t destByte = i / 8;
        uint8_t destBit   = i % 8;

        if((MB_DisInputs[sourceByte] >> sourceBit) & 0x01)
        {
            tx[9 + destByte] |= (1 << destBit);
        }
    }

    txLen = 9 + byteCount;

    if(tcp_write(pcb, tx, txLen, TCP_WRITE_FLAG_COPY) != ERR_OK)
        return ERR_MEM;

    tcp_output(pcb);

    return ERR_OK;
}
err_t MB_FC03_ReadHoldingRegisters(struct tcp_pcb *pcb, MB_Request_t *req)
{
    uint16_t startAddr;
    uint16_t quantity;
    uint16_t i;
    uint8_t tx[260];
    uint16_t txLen = 0;

    startAddr = ((uint16_t)req->Data[0] << 8) | req->Data[1];
    quantity  = ((uint16_t)req->Data[2] << 8) | req->Data[3];

    printf("FC03 - Read Holding Registers\r\n");
    printf("Start Address : %u\r\n", startAddr);
    printf("Quantity      : %u\r\n", quantity);

    /* Validate quantity */
    if((quantity == 0) || (quantity > 125))
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);

    /* Validate address range */
    if((startAddr + quantity) > MB_HOLDING_REG_COUNT)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);

    /* MBAP Header */
    // Transaction ID 2 Bytes
    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    // Fixed Protocol ID 0000
    tx[2] = 0;
    tx[3] = 0;

    // Length 2 Bytes
    tx[4] = 0;
    tx[5] = 3 + (quantity * 2);  // Unit ID + FC + NumBytes + 2 bytes per reg

    tx[6] = req->UnitID;  // Unit ID
    tx[7] = MB_FC_READ_HOLDING_REGS;  // FC
    tx[8] = quantity * 2;  // Num Bytes (Data)

    txLen = 9;

    for(i = 0; i < quantity; i++)
    {
        uint16_t value = MB_HoldingRegs[startAddr + i];

        tx[txLen++] = value >> 8;
        tx[txLen++] = value;
    }

    if(tcp_write(pcb, tx, txLen, TCP_WRITE_FLAG_COPY) != ERR_OK)
        return ERR_MEM;

    tcp_output(pcb);

    printf("FC03 Response Sent\r\n");

    return ERR_OK;
}

err_t MB_FC04_ReadInputRegisters(struct tcp_pcb *pcb, MB_Request_t *req)
{
    uint16_t startAddr;
    uint16_t quantity;
    uint16_t i;
    uint8_t tx[260];
    uint16_t txLen = 0;

    startAddr = ((uint16_t)req->Data[0] << 8) | req->Data[1];
    quantity  = ((uint16_t)req->Data[2] << 8) | req->Data[3];

    printf("FC04 - Read Input Registers\r\n");
    printf("Start Address : %u\r\n", startAddr);
    printf("Quantity      : %u\r\n", quantity);

    /* Validate quantity */
    if((quantity == 0) || (quantity > 125))
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);

    /* Validate address range */
    if((startAddr + quantity) > MB_INPUT_REG_COUNT)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);

    /* MBAP Header */
    // Transaction ID 2 Bytes
    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    // Fixed Protocol ID 0000
    tx[2] = 0;
    tx[3] = 0;

    // Length 2 Bytes
    tx[4] = 0;
    tx[5] = 3 + (quantity * 2);  // Unit ID + FC + NumBytes + 2 bytes per reg

    tx[6] = req->UnitID;  // Unit ID
    tx[7] = MB_FC_READ_INPUT_REGS;  // FC
    tx[8] = quantity * 2;  // Num Bytes (Data)

    txLen = 9;

    for(i = 0; i < quantity; i++)
    {
        uint16_t value = MB_InputRegs[startAddr + i];

        tx[txLen++] = value >> 8;
        tx[txLen++] = value;
    }

    if(tcp_write(pcb, tx, txLen, TCP_WRITE_FLAG_COPY) != ERR_OK)
        return ERR_MEM;

    tcp_output(pcb);

    printf("FC04 Response Sent\r\n");

    return ERR_OK;
}
err_t MB_FC05_WriteSingleCoil(struct tcp_pcb *pcb, MB_Request_t *req)
{
    uint16_t address;
    uint16_t value;
    uint8_t tx[12];

    address = ((uint16_t)req->Data[0] << 8) | req->Data[1];
    value   = ((uint16_t)req->Data[2] << 8) | req->Data[3];

    if(address >= MB_COIL_COUNT)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);

    uint16_t sourceByte = address / 8;
    uint8_t sourceBit   = address % 8;

    if(value == 0xFF00)
    {
        MB_Coils[sourceByte] |= (1 << sourceBit);
    }
    else if(value == 0x0000)
    {
        MB_Coils[sourceByte] &= ~(1 << sourceBit);
    }
    else
    {
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);
    }

    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    tx[2] = 0;
    tx[3] = 0;

    tx[4] = 0;
    tx[5] = 6;

    tx[6] = req->UnitID;

    tx[7] = MB_FC_WRITE_SINGLE_COIL;

    tx[8]  = address >> 8;
    tx[9]  = address;

    tx[10] = value >> 8;
    tx[11] = value;

    if(tcp_write(pcb, tx, sizeof(tx), TCP_WRITE_FLAG_COPY) != ERR_OK)
        return ERR_MEM;

    tcp_output(pcb);

    return ERR_OK;
}
err_t MB_FC06_WriteSingleRegister(struct tcp_pcb *pcb, MB_Request_t *req)
{

    uint16_t address;
    uint16_t value;
    uint8_t tx[12];

    address = (uint16_t)(req->Data[0] << 8 |(req->Data[1]));
    value = ((uint16_t)req->Data[2] << 8) | req->Data[3];

    printf("FC06 - Write Single Holding Register\r\n");
	printf("Address : %u\r\n", address);
	printf("Value   : %u (0x%04X)\r\n", value, value);

	/* Validate address */
	    if(address >= MB_HOLDING_REG_COUNT)
	        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);
	/* Write Register */
	MB_HoldingRegs[address] = value;
	/* MBAP Header */
	tx[0] = req->TransactionID >> 8;
	tx[1] = req->TransactionID;

	tx[2] = 0;
	tx[3] = 0;

	tx[4] = 0;
	tx[5] = 6;      // Unit ID + FC + Address + Value
	tx[6] = req->UnitID;
	tx[7] = MB_FC_WRITE_SINGLE_REG;

	tx[8]  = address >> 8;
	tx[9]  = address;

	tx[10] = value >> 8;
	tx[11] = value;
	if(tcp_write(pcb, tx, sizeof(tx), TCP_WRITE_FLAG_COPY) != ERR_OK)
		return ERR_MEM;

	tcp_output(pcb);

	printf("FC06 Response Sent\r\n");

	return ERR_OK;

}
err_t MB_FC07_ReadExceptionStatus(struct tcp_pcb *pcb, MB_Request_t *req)
{
	return ERR_OK;
}
err_t MB_FC15_WriteMultipleCoils(struct tcp_pcb *pcb, MB_Request_t *req)
{
    uint16_t startAddr;
    uint16_t quantity;
    uint8_t byteCount;
    uint8_t expectedByteCount;
    uint16_t i;

    uint8_t tx[12];

    startAddr = ((uint16_t)req->Data[0] << 8) | req->Data[1];
    quantity  = ((uint16_t)req->Data[2] << 8) | req->Data[3];
    byteCount = req->Data[4];

    if((quantity == 0) || (quantity > 1968))
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);

    expectedByteCount = (quantity + 7) / 8;
    if(byteCount != expectedByteCount)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);

    if((startAddr + quantity) > MB_COIL_COUNT)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);

    for(i = 0; i < quantity; i++)
    {
        uint8_t sourceByte = i / 8;
        uint8_t sourceBit  = i % 8;

        uint8_t state = (req->Data[5 + sourceByte] >> sourceBit) & 0x01;

        uint16_t coilAddress = startAddr + i;

        uint16_t coilByte = coilAddress / 8;
        uint8_t coilBit = coilAddress % 8;

        if(state)
            MB_Coils[coilByte] |= (1 << coilBit);
        else
            MB_Coils[coilByte] &= ~(1 << coilBit);
    }

    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    tx[2] = 0;
    tx[3] = 0;

    tx[4] = 0;
    tx[5] = 6;

    tx[6] = req->UnitID;

    tx[7] = MB_FC_WRITE_MULTI_COILS;

    tx[8] = startAddr >> 8;
    tx[9] = startAddr;

    tx[10] = quantity >> 8;
    tx[11] = quantity;

    if(tcp_write(pcb, tx, sizeof(tx), TCP_WRITE_FLAG_COPY) != ERR_OK)
        return ERR_MEM;

    tcp_output(pcb);

    return ERR_OK;
}
err_t MB_FC16_WriteMultipleRegisters(struct tcp_pcb *pcb, MB_Request_t *req)
{
    uint16_t startAddr;
    uint16_t quantity;
    uint8_t byteCount;
    uint16_t value;
    uint16_t i;

    uint8_t tx[12];

    startAddr = ((uint16_t)req->Data[0] << 8) | req->Data[1];
    quantity  = ((uint16_t)req->Data[2] << 8) | req->Data[3];
    byteCount = req->Data[4];

    printf("FC16 - Write Multiple Holding Registers\r\n");
    printf("Start Address : %u\r\n", startAddr);
    printf("Quantity      : %u\r\n", quantity);
    printf("Byte Count    : %u\r\n", byteCount);

    /* Validate quantity */
    if((quantity == 0) || (quantity > 123))
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_VALUE);

    /* Validate address range */
    if((startAddr + quantity) > MB_HOLDING_REG_COUNT)
        return MB_SendException(pcb, req, MB_EX_ILLEGAL_DATA_ADDRESS);

    /* Write Registers */
    for(i = 0; i < quantity; i++)
    {
        value = ((uint16_t)req->Data[5 + (i * 2)] << 8) | req->Data[6 + (i * 2)];

        MB_HoldingRegs[startAddr + i] = value;

        printf("Reg[%u] = %u (0x%04X)\r\n", startAddr + i, value, value);
    }

    /* MBAP Header */
    tx[0] = req->TransactionID >> 8;
    tx[1] = req->TransactionID;

    tx[2] = 0;
    tx[3] = 0;

    tx[4] = 0;
    tx[5] = 6;      // Unit ID + FC + Address + Quantity

    tx[6] = req->UnitID;
    tx[7] = MB_FC_WRITE_MULTI_REGS;

    tx[8] = startAddr >> 8;
    tx[9] = startAddr;

    tx[10] = quantity >> 8;
    tx[11] = quantity;

    if(tcp_write(pcb, tx, sizeof(tx), TCP_WRITE_FLAG_COPY) != ERR_OK)
        return ERR_MEM;

    tcp_output(pcb);

    printf("FC16 Response Sent\r\n");

    return ERR_OK;
}
extern uint16_t readADC (void);

void MB_UpdateInputRegisters(void)
{
	uint16_t adcVal = readADC();
	MB_InputRegs[0] = HAL_GetTick() / 1000;
    MB_InputRegs[1] = adcVal;
    MB_InputRegs[2] = 0xFFFF-adcVal;
}
void MB_updateCoils(void)
{
    HAL_GPIO_WritePin(GPIOE, LED1_Pin, (MB_Coils[0] >> 0) & 0x01);
    HAL_GPIO_WritePin(GPIOE, LED2_Pin, (MB_Coils[0] >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOE, LED3_Pin, (MB_Coils[0] >> 2) & 0x01);

}
