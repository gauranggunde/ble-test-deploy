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

#include <stdio.h>
#include "../BMA253_accel.h"
#include "mcc.h"


void BMA253_GetAccelDataX_Example(void)
{
    int16_t xAccelData;

    BMA253_GetAccelDataX(&xAccelData);
    
    if(SW0_GetValue() == 0)
    {
        printf("X Accel Value: %d\r\n", xAccelData);
    }
    
}

void BMA253_GetAccelDataY_Example(void)
{
    int16_t yAccelData;

    BMA253_GetAccelDataY(&yAccelData);
    
    if(SW0_GetValue() == 0)
    {
        printf("Y Accel Value: %d\r\n", yAccelData);
    }
    
}

void BMA253_GetAccelDataZ_Example(void)
{
    int16_t zAccelData;

    BMA253_GetAccelDataZ(&zAccelData);
    
    if(SW0_GetValue() == 0)
    {
        printf("Z Accel Value: %d\r\n", zAccelData);
    }
    
}

void BMA253_GetAccelDataXYZ_Example(void)
{
    BMA253_ACCEL_DATA_t accelData;

    BMA253_GetAccelDataXYZ(&accelData);

    if(SW0_GetValue() == 0)
    {
        printf("Acceleration Values: X = %d, Y = %d, Z = %d\r\n", accelData.x, accelData.y, accelData.z);
    }
}
