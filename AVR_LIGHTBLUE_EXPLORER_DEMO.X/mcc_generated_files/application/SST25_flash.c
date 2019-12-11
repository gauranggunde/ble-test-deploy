/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/
#include "SST25_flash.h"
#include "mcc.h"

#define DUMMY_BYTE              (0xFF)

void SST25_Initialize(void)
{
    spiMaster[MASTER0].spiClose();
    spiMaster[MASTER0].spiOpen();
}

void SST25_WriteEnable(void)
{
    FLASH_CS_SetLow();
    spiMaster[MASTER0].exchangeByte(SST25_WRITE_ENABLE);
    FLASH_CS_SetHigh();
}

uint8_t SST25_ReadDeviceID(void)
{
    uint8_t deviceID;
    
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_READ_ID);
    spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    deviceID = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    
    FLASH_CS_SetHigh();
    
    return deviceID;   
}

void SST25_ReadJedecID(uint8_t* jedecID)
{
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_READ_JEDEC_ID);
    jedecID[0] = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    jedecID[1] = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    jedecID[2] = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    jedecID[3] = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    
    FLASH_CS_SetHigh();
}

uint8_t SST25_ReadStatusRegister(void)
{
    uint8_t statusRegisterByte = 0;
    
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_READ_STATUS_REGISTER);
    statusRegisterByte = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    
    FLASH_CS_SetHigh();
    
    return statusRegisterByte;
}

uint8_t SST25_ReadFlashByte(uint32_t memoryAddress)
{
    uint8_t readByte;
    
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_READ_MEMORY);
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFFFF) >> 16));
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFF) >> 8));
    spiMaster[MASTER0].exchangeByte(memoryAddress & 0xFF);
    
    readByte = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);

    FLASH_CS_SetHigh();
    
    return readByte;
    
}

uint8_t SST25_HighSpeedReadFlashByte(uint32_t memoryAddress)
{
    uint8_t readByte;
    
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_HIGHSPEED_READ_MEMORY);
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFFFF) >> 16));
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFF) >> 8));
    spiMaster[MASTER0].exchangeByte(memoryAddress & 0xFF);
    spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);
    
    readByte = spiMaster[MASTER0].exchangeByte(DUMMY_BYTE);

    FLASH_CS_SetHigh();
    
    return readByte;   
}

void SST25_WritePage(uint8_t *data[], uint32_t memoryAddress)
{
    uint8_t dataIndex;
    
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_PAGE_PROGRAM);
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFFFF) >> 16));
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFF) >> 8));
    spiMaster[MASTER0].exchangeByte(memoryAddress & 0xFF);
    
    for(dataIndex = 0; dataIndex <= 254; dataIndex++)
    {
        spiMaster[MASTER0].exchangeByte(*data[dataIndex]);
    }
    
    FLASH_CS_SetHigh();
}

void SST25_ChipErase(void)
{
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_CHIP_ERASE);
    
    FLASH_CS_SetHigh();
}

void SST25_SectorErase(uint32_t memoryAddress)
{
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_4KB_SECTOR_ERASE);
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFFFF) >> 16));
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFF) >> 8));
    spiMaster[MASTER0].exchangeByte(memoryAddress & 0xFF);
    
    FLASH_CS_SetHigh();
}

void SST25_BlockErase(uint32_t memoryAddress)
{
    FLASH_CS_SetLow();
    
    spiMaster[MASTER0].exchangeByte(SST25_64KB_BLOCK_ERASE);
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFFFF) >> 16));
    spiMaster[MASTER0].exchangeByte(((memoryAddress & 0xFFFF) >> 8));
    spiMaster[MASTER0].exchangeByte(memoryAddress & 0xFF);
    
    FLASH_CS_SetHigh();
}



