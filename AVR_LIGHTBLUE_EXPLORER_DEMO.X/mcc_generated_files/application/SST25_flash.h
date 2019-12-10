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


#ifndef SST25_FLASH_H
#define	SST25_FLASH_H

#include <stdint.h>

/* INSTRUCTION SET OP CODES */

#define SST25_READ_MEMORY                        (0x03)
#define SST25_HIGHSPEED_READ_MEMORY              (0x0B)
#define SST25_WRITE_ENABLE                       (0x06)
#define SST25_PAGE_PROGRAM                       (0x02)
#define SST25_CHIP_ERASE                         (0x60)
#define SST25_4KB_SECTOR_ERASE                   (0xD7)
#define SST25_64KB_BLOCK_ERASE                   (0xD8)
#define SST25_READ_STATUS_REGISTER               (0x05)
#define SST25_READ_ID                            (0xAB)
#define SST25_READ_JEDEC_ID                      (0x9F)

void SST25_Initialize(void);
void SST25_WriteEnable(void);
uint8_t SST25_ReadDeviceID(void);
void SST25_ReadJedecID(uint8_t* jedecID);
uint8_t SST25_ReadStatusRegister(void);
uint8_t SST25_ReadFlashByte(uint32_t memoryAddress);
uint8_t SST25_HighSpeedReadFlashByte(uint32_t memoryAddress);
void SST25_WritePage(uint8_t *data[], uint32_t memoryAddress);
void SST25_ChipErase(void);
void SST25_SectorErase(uint32_t memoryAddress);
void SST25_BlockErase(uint32_t memoryAddress);



#endif	/* SST25_FLASH_H */

