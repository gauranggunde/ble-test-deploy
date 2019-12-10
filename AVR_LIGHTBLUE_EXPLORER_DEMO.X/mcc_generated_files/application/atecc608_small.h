/** @file   atecc608_small.h
 *  @brief  This file contains APIs to to access the minimal features required
 *          by the ECC608 devices, to access device specific information
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

#ifndef ATECC608_SMALL_DRIVER_H
#define	ATECC608_SMALL_DRIVER_H

#include <stdint.h>
/**
@ingroup ATECC608
@def ECC608_KEY_CHECK_BYTE
This is a macro is used to access appropriate byte for valid key check
*/
#define ECC608_KEY_CHECK_BTYE           (0)
/**
@ingroup ATECC608
@def ECC608_KEY_CHECK_BYTE
This is a macro is used to check if key value response is valid
*/
#define ECC608_KEY_CHECK_VALUE          (0x43)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_0
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_0            (1)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_1
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_1            (2)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_2
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_2            (3)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_3
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_3            (4)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_4
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_4            (9)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_5
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_5            (10)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_6
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_6            (11)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_7
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_7            (12)
/**
@ingroup ATECC608
@def ECC608_SERIAL_BYTE_8
This is a macro is used to access appropriate serial number byte location
*/
#define ECC608_SERIAL_BYTE_8            (13)
            
/**
@ingroup ATECC608
@def ECC608_I2C_ADDRESS
This is a macro for ECC608 I2C ADDRESS
*/
#define ECC608_I2C_ADDRESS              (0x58)

/* ATECC608 SMALL Public APIs */

/**
 @ingroup ATECC608
 @brief \n
 * This API is called to read the ATECC608 Serial Number
 @param[in] addr - 16bit hex address value
 @param[in] *serial - 8bit pointer to serial value for exchange
 @return @ref serial (32) bytes Config Zone \n 
 */
void atecc508_read_serial(uint16_t addr, uint8_t * serial);
/**
 @ingroup ATECC608
 @brief \n
 * This API is called to read the ATECC608 Serial Number
 @param[in] addr - 16bit hex address value
 @param[in] addr - 16bit hex ECC SLOT value
 @param[in] *serial - 8bit pointer to serial value for exchange
 @return @ref serial (64) bytes key from GenKey \n 
 */
void atecc508_read_slot(uint16_t addr, uint16_t slot, uint8_t * serial);
/**
 @ingroup ATECC608
 @brief \n
 * This API puts the ECC608 into a low power sleep mode. 
 @param[in] void
 @return @ref void \n 
 */
void atecc508_sleep(uint16_t addr);

#endif	/* ATECC608_SMALL_DRIVER_H */

