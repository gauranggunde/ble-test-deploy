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

#include "BMA253_accel.h"
#include "drivers/i2c_simple_master.h"

#define DATA_READY_BIT_MASK                 (0x01)

static bool BMA253_NewAccelDataRdyX(void);
static bool BMA253_NewAccelDataRdyY(void);
static bool BMA253_NewAccelDataRdyZ(void);

void BMA253_Initialize(void)
{
    i2c_write1ByteRegister(BMA253_ADDR, BMA253_PWR_MODE_ADDR, BMA253_NORMAL_MODE);
    i2c_write1ByteRegister(BMA253_ADDR, BMA253_BW_SELECT_ADDR, BMA253_BW_7_81HZ);
    i2c_write1ByteRegister(BMA253_ADDR, BMA253_RANGE_ADDR, BMA253_2G_RANGE);
}

void BMA253_GetAccelDataX(int16_t *xAccelData)
{
    
    while(!BMA253_NewAccelDataRdyX())
    {
        // Do Nothing until we have new data ready in the register
    }
    
    
    *xAccelData = (((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_X_LSB_ADDR))
                  | ((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_X_MSB_ADDR) << 8) >> 4);
    
}

void BMA253_GetAccelDataY(int16_t *yAccelData)
{

    while(!BMA253_NewAccelDataRdyY())
    {
        // Do Nothing until we have new data ready in the register
    }
    
    
    *yAccelData = (((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Y_LSB_ADDR))
                  | ((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Y_MSB_ADDR) << 8) >> 4);
    
}

void BMA253_GetAccelDataZ(int16_t *zAccelData)
{
    
    while(!BMA253_NewAccelDataRdyZ())
    {
        // Do Nothing until we have new data ready in the register
    }
    
    
    *zAccelData = (((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Z_LSB_ADDR))
                  | ((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Z_MSB_ADDR) << 8) >> 4);
    
}

void BMA253_GetAccelDataXYZ(BMA253_ACCEL_DATA_t *accelData)
{
    
    while (!BMA253_NewAccelDataRdyX())
    {
        // Do Nothing until we have new data ready in the register
    }
    
    accelData->x = (((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_X_LSB_ADDR))
                  | ((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_X_MSB_ADDR) << 8) >> 4);
    
    while (!BMA253_NewAccelDataRdyY())
    {
        // Do Nothing until we have new data ready in the register
    }
    
    accelData->y = (((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Y_LSB_ADDR))
                  | ((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Y_MSB_ADDR) << 8) >> 4);
    
     while (!BMA253_NewAccelDataRdyZ())
    {
        // Do Nothing until we have new data ready in the register
    }
    
    accelData->z = (((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Z_LSB_ADDR))
                  | ((int16_t)i2c_read1ByteRegister(BMA253_ADDR, BMA253_Z_MSB_ADDR) << 8) >> 4);
    
}

uint8_t BMA253_GetAccelChipId(void)
{
    return i2c_read1ByteRegister(BMA253_ADDR, BMA253_CHIP_ID_ADDR);
}

static bool BMA253_NewAccelDataRdyX(void)
{
    bool xNewData;
    
    xNewData = (i2c_read1ByteRegister(BMA253_ADDR, BMA253_X_LSB_ADDR) & DATA_READY_BIT_MASK); //Is new data ready?
    
    return xNewData;
}

static bool BMA253_NewAccelDataRdyY(void)
{
    bool yNewData;
    
    yNewData = (i2c_read1ByteRegister(BMA253_ADDR, BMA253_Y_LSB_ADDR) & DATA_READY_BIT_MASK); //Is new data ready?
    
    return yNewData;
}

static bool BMA253_NewAccelDataRdyZ(void)
{
    bool zNewData;
    
    zNewData = (i2c_read1ByteRegister(BMA253_ADDR, BMA253_Z_LSB_ADDR) & DATA_READY_BIT_MASK); //Is new data ready?
    
    return zNewData;
}

