/** @file   nonMCC_Memory.h
 *  @brief  Supporting Memory Manipulation Functions External to MCC Memory code
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

#ifndef NON_MCC_MEMORY_H
#define NON_MCC_MEMORY_H
#include <stdint.h>

/**
@ingroup nonMCC
@def MCU_CONFIG_CHIP_ID_REG
This is a macro for accessing PICs Chip ID Register
*/
#define MCU_CONFIG_CHIP_ID_REG          0x8005
/**
@ingroup nonMCC
@def MCU_CONFIG_CHIP_REVISION_REG
This is a macro for accessing PICs Chip Revision Register
*/
#define MCU_CONFIG_CHIP_REVISION_REG    0x8006
/**
@ingroup nonMCC
@def MCU_CONFIG_DEVICE_INFO_AREA_REG
This is a macro for accessing Device Info Area for Unique User Code space
*/
#define MCU_CONFIG_DEVICE_INFO_AREA_REG 0x8100
/**
@ingroup nonMCC
@def MCU_MICROCHIP_UNIQUE_IDENTIFIER_SIZE
This is a macro for the range of words stored for ID stored in DIA space
*/
#define MCU_MICROCHIP_UNIQUE_IDENTIFIER_SIZE (0x09)
/**
@ingroup nonMCC
@def nonMCC_FLASH_ReadCONFIG
* An API used to return values read for FLASH CONFIG memory ranges
*/
uint16_t nonMCC_FLASH_ReadCONFIG(uint16_t flashAddr);

#endif /* NON_MCC_MEMORY_H */