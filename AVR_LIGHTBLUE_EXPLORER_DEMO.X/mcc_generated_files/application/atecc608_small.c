/** @file   atecc608_small.c
 *  @brief  This file contains APIs to access features supported by the MCP9844
 */
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

#include "atecc608_small.h"
#include "../mcc_generated_files/delay.h"
//#include "../mcc_generated_files/drivers/i2c_master.h"
#include "../mcc_generated_files/drivers/i2c_simple_master.h"

/* ECC Word Address Definitions */
#define ECC_WA_RESET    (0x00)
#define ECC_WA_SLEEP    (0x01)
#define ECC_WA_IDLE     (0x02)
#define ECC_WA_CMD      (0x03)

/* CRC Polynomial used Definitions */
#define POLYNOMIAL16    (0x8005)

static void atecc_wakeup(uint8_t adr);
static uint16_t crc16_update(uint16_t crc, uint8_t a, uint8_t reverse);

void atecc508_sleep(uint16_t addr)
{
    uint8_t cmd_buf[1] = {ECC_WA_SLEEP};
    i2c_writeNBytes(addr, cmd_buf, 1);
}

void atecc508_read_serial(uint16_t addr, uint8_t * serial)
{
    // Word Address, Count=7, Command=READ, Param1, Param2, Start Address=0x0000, CRC
    uint8_t cmd_buf[8] = {ECC_WA_CMD, 0x07, 0x02, 0x80, 0x00, 0x00, 0x09, 0xAD}; //Read 32 bytes of Config Zone

    atecc_wakeup(0);
    DELAY_milliseconds(10);

    i2c_writeNBytes(addr, cmd_buf, 8);
    DELAY_milliseconds(10);

    i2c_readNBytes(addr, serial, 35);
    // Pass Data up through serial
}

void atecc508_read_slot(uint16_t addr, uint16_t slot, uint8_t * serial)
{
    // Word Address, Count=7, Command=READ, Param1, Param2, Start Address=0x0000, CRC
    uint8_t cmd_buf[8] = {ECC_WA_CMD, 0x07, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00}; //Read 64 bytes key from GenKey

    uint16_t crc = 0;
    for (uint8_t x = 1; x < 6; x++) 
    {
        crc = crc16_update(crc, cmd_buf[x], 1);
    }
    cmd_buf[6] = (uint8_t) crc & 0xFF;
    cmd_buf[7] = (uint8_t) (crc >> 8) & 0xFF;

    atecc_wakeup(0);
    DELAY_milliseconds(10);

    i2c_writeNBytes(addr, cmd_buf, 8);
    DELAY_milliseconds(60);

    i2c_readNBytes(addr, serial, 67);
    // Pass Data up through serial
}

/**
 @ingroup ATECC608
 @brief \n
 * This API is called to wake the atecc device; and allow bus to become ready.
 @param[in] addr - 8bit hex address value used for i2c communication
 @return @ref void \n 
 */
static void atecc_wakeup(uint8_t adr)
{
    //TODO: This should use i2c_simple_master
    
    //transfer_descriptor_t d = {data, size};
//    while (!i2c_open(adr))
//    ; // sit here until we get the bus..
//
//    i2c_setDataCompleteCallback(i2c_returnStop, NULL);
//
//    // Transmit specified number of bytes
//    i2c_setBuffer((void *)&adr, 1);
//    // Start a Write operation
//    i2c_masterWrite();
//    while (I2C_BUSY == i2c_close())
    ; // sit here until the entire chained operation has finished
}

/**
 @ingroup ATECC608
 @brief \n
 * This API is called to compute and return a crc16 result.
 @param[in] crc - 16bit polynomial value starting point
 @param[in] a - seed value passed to polynominal
 @param[in] reverese -  Run the reverse implementation
 @return @ref crc result - 16bit values returned with crc updated value \n 
 */
static uint16_t crc16_update(uint16_t crc, uint8_t a, uint8_t reverse)
{
    // polynomial 0xA001 is the same as 0x8005
    // in reverse implementation
    uint8_t i;
    uint8_t input = 0;

    if (reverse) 
    {
        for (i = 0; i < 8; i++) 
        {
            if (a & (0x01 << i)) 
            {
                input |= (0x80 >> i);
            }
        }
    } 
    else 
    {
        input = a;
    }

    crc ^= (input << 8);
    for (i = 0; i < 8; i++)	
    {
        if (crc & 0x8000) 
        {
            crc = (crc << 1) ^ POLYNOMIAL16;
        } 
        else 
        {
            crc = (crc << 1);
        }
    }

    return crc;
}