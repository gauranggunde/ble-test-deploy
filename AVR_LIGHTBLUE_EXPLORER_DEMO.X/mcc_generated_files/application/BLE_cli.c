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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../rn4870-1-ble-module/rn487x.h"
#include "BLE_service.h"
#include "BLE_cli.h"

#define MAX_PACKET_SIZE     20

// CLI Operation Variables
static char packet[MAX_PACKET_SIZE];
static uint8_t index = 0;

void BLE_cli_parserTask(void)
{
    volatile char readCharacter = '\0';
    static bool incomingPacket = false;
    
    while (RN487X.isRxReady())
    {
        readCharacter = RN487X.uartRx();
        
        if (readCharacter == START_BYTE)
        {
            incomingPacket = true;
        }
        if(incomingPacket)
        {
            if (readCharacter == TERMINATION_BYTE)
            {
                packet[index] = '\0';
                incomingPacket = false;
                BLE_ProcessPacket(packet);
                memset(packet,'\0',sizeof(packet));
                index = 0;
                
            }
            else
            {
                if (readCharacter != START_BYTE)
                {
                    packet[index++] = readCharacter;
                }
            }
        }
    }
    
}
