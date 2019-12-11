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

#include <stdint.h>
#include <string.h>
#include <stdbool.h> //TODO: Add this inlcude to RN487X.h and remove here.

#include "LIGHTBLUE_service.h"

// APP Dependencies
#include "../rn4870-1-ble-module/rn487x.h"
#include "BMA253_accel.h"
#include "MCP9844_temp_sensor.h"
#include "include/pin_manager.h"
#include "drivers/uart.h"

#define Hex(x) _hex[(x) & 0xF]
#define Ascii2Decimal(c)  (((c) <= '9') ? (c) - '0' : (c & 0x5f) - 'A' + 10)

#define DataLedOn()                 DATA_LED_SetLow()
#define DataLedOff()                DATA_LED_SetHigh()

#define ErrorLedOn()                ERROR_LED_SetLow()
#define ErrorLedOff()               ERROR_LED_SetHigh()

#define START_BYTE                  '['
#define TERMINATION_BYTE            ']'

#define LIGHTBLUE_OFF               (0x00)

#define DATA_LED_IDENTIFIER         (0x00)
#define ERROR_LED_IDENTIFIER        (0x10)

#define NIBBLE_MASK                 (0x01)

const char * const protocol_version_number      = "1.0.0";

typedef enum
{
    PROTOCOL_VERSION_ID    = 'V',
    LED_STATE_ID           = 'L',
    BUTTON_STATE_ID        = 'P',
    TEMPERATURE_DATA_ID    = 'T',
    ACCEL_DATA_ID          = 'X',
    SERIAL_DATA_ID         = 'S',
    ERROR_ID               = 'R',
    UI_CONFIG_DATA_ID      = 'U'
}PROTOCOL_PACKET_TYPES_t;

typedef enum
{
    IDLE                    = 0,
    SEQUENCE_NUMBER         = 1,
    PACKET_ID               = 2,
    PAYLOAD_SIZE_0          = 3,
    PAYLOAD_SIZE_1          = 4,
    PAYLOAD_0               = 5,
    PAYLOAD_1               = 6        
           
}PACKET_PARSER_STATE_t;

static char _hex[] = "0123456789ABCDEF";
static uint8_t sequenceNumber = 0;

static void LIGHTBLUE_SendPacket(char packetID, char* payload);
static void LIGHTBLUE_SplitWord(char* payload, int16_t value);
static void LIGHTBLUE_SplitByte(char* payload, int8_t value);
static uint8_t LIGHTBLUE_GetButtonValue(void);
static uint8_t LIGHTBLUE_GetDataLedValue(void);
static uint8_t LIGHTBLUE_GetErrorLedValue(void);
static void LIGHTBLUE_PerformAction(char id, uint8_t data);

void LIGHTBLUE_TemperatureSensor(void)
{
    char payload[5];
    int16_t temperature;
    
    *payload = '\0';
    MCP9844_GetTemperatureValue(&temperature);
    
    LIGHTBLUE_SplitWord(payload, temperature);
    
    LIGHTBLUE_SendPacket(TEMPERATURE_DATA_ID, payload);
}

void LIGHTBLUE_AccelSensor(void)
{
    char payload[13];
    BMA253_ACCEL_DATA_t accelData;
    
    *payload = '\0';
    BMA253_GetAccelDataXYZ(&accelData);
    
    LIGHTBLUE_SplitWord(payload, (accelData.x & 0x0FFF)); // Masking to ensure top nibble is always 0 as light blue expects
    LIGHTBLUE_SplitWord(payload, (accelData.y & 0x0FFF)); // TODO: JIRA M8TS-2071 Light Blue seems to be throwing an exception when it is not 0
    LIGHTBLUE_SplitWord(payload, (accelData.z & 0x0FFF));
    
    LIGHTBLUE_SendPacket(ACCEL_DATA_ID, payload);
}

void LIGHTBLUE_PushButton(void)
{
    char payload[3];
    uint8_t button = LIGHTBLUE_GetButtonValue(); 
    
    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, button);
    
    LIGHTBLUE_SendPacket(BUTTON_STATE_ID, payload);
}

void LIGHTBLUE_LedState(void)
{
    char payload[3];
    uint8_t led; 
    
    led = DATA_LED_IDENTIFIER + LIGHTBLUE_GetDataLedValue();
    
    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, led);
    
    LIGHTBLUE_SendPacket(LED_STATE_ID, payload);
    
    led = ERROR_LED_IDENTIFIER + LIGHTBLUE_GetErrorLedValue();
    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, led);
    
    LIGHTBLUE_SendPacket(LED_STATE_ID, payload);
}

void LIGHTBLUE_SendProtocolVersion(void)
{
    char payload[19];
    uint8_t value;
    uint8_t dataIndex;
    
    *payload = '\0';
    
    for(dataIndex = 0; dataIndex < strlen(protocol_version_number); dataIndex++)
    {
        value = protocol_version_number[dataIndex];
        LIGHTBLUE_SplitByte(payload, value);
    }
    
    LIGHTBLUE_SendPacket(PROTOCOL_VERSION_ID, payload);
}

void LIGHTBLUE_SendSerialData(char* serialData)
{
    uint8_t len = strlen(serialData)*2; 
    
    RN487X.uartTx(START_BYTE);                  
    RN487X.uartTx(Hex(sequenceNumber++));
    RN487X.uartTx(SERIAL_DATA_ID);
    RN487X.uartTx(Hex(len>>4));            
    RN487X.uartTx(Hex(len));
    while (*serialData) 
    {                       
        RN487X.uartTx(Hex(*serialData>>4));
        RN487X.uartTx(Hex(*serialData++));
    }
    RN487X.uartTx(TERMINATION_BYTE);
}

void LIGHTBLUE_ParseIncomingPacket(char receivedByte)
{
    static PACKET_PARSER_STATE_t parserState = IDLE;
    static uint8_t length = 0;
    static uint16_t data = 0;
    static char packetID = '\0';
    
    switch(parserState) 
    {
        case SEQUENCE_NUMBER:
            //ignore sequence
            parserState = PACKET_ID;
            break;
        case PACKET_ID:
            packetID = receivedByte;
            parserState = PAYLOAD_SIZE_0;
            break;
        case PAYLOAD_SIZE_0:
            length = Ascii2Decimal(receivedByte);
            parserState = PAYLOAD_SIZE_1;
            break;
        case PAYLOAD_SIZE_1:
            length = (length << 4) + Ascii2Decimal(receivedByte);
            parserState = PAYLOAD_0;
            break;
        case PAYLOAD_0:
            data = Ascii2Decimal(receivedByte);
            length--;
            if (length == 0)
            {
                parserState = IDLE;
            }
            else
            {
                parserState = PAYLOAD_1;
            }
            break;
        case PAYLOAD_1:
            data = (data << 4) + Ascii2Decimal(receivedByte);
            LIGHTBLUE_PerformAction(packetID, data);
            length--;
            if (length == 0)
            {
                parserState = IDLE;
            }
            else
            {
                parserState = PAYLOAD_0;
            }
            break;
        case IDLE:
        default:
            if (receivedByte == START_BYTE)
            {
                parserState = SEQUENCE_NUMBER;
            }
            break;
    }
}

static void LIGHTBLUE_SendPacket(char packetID, char* payload)
{
    RN487X.uartTx(START_BYTE);
    RN487X.uartTx(Hex(sequenceNumber++));
    RN487X.uartTx(packetID);
    RN487X.uartTx(Hex(strlen(payload) >> 4));
    RN487X.uartTx(Hex(strlen(payload)));
    while (*payload)
    {
        RN487X.uartTx((*(uint8_t *)payload++));
    }
    RN487X.uartTx(TERMINATION_BYTE);
}

static void LIGHTBLUE_SplitWord(char* payload, int16_t value)
{
    LIGHTBLUE_SplitByte(payload, value);
    LIGHTBLUE_SplitByte(payload, value >> 8);
}

static void LIGHTBLUE_SplitByte(char* payload, int8_t value)
{
    payload += strlen(payload);
    *payload++ = Hex(value >> 4);
    *payload++ = Hex(value);
    *payload = '\0';
}

static uint8_t LIGHTBLUE_GetButtonValue(void)
{
    return !(SW0_GetValue()); // This is forcing proper data for LightBlue
}

static uint8_t LIGHTBLUE_GetDataLedValue(void)
{
    return !(DATA_LED_GetValue()); // This is forcing proper data for LightBlue
}

static uint8_t LIGHTBLUE_GetErrorLedValue(void)
{
    return !(ERROR_LED_GetValue());
}

static void LIGHTBLUE_PerformAction(char id, uint8_t data)
{
    uint8_t led;
    
    switch(id)
    {
        case LED_STATE_ID:
            led = (data >> 4) & NIBBLE_MASK;
            if(led == DATA_LED_IDENTIFIER)
            {
                if ((data & NIBBLE_MASK) == LIGHTBLUE_OFF)
                {
                    DataLedOff();
                }
                else 
                {
                    DataLedOn();
                }
            }
            else 
            {
                if ((data & NIBBLE_MASK) == LIGHTBLUE_OFF)
                {
                    ErrorLedOff();
                }
                else
                {
                    ErrorLedOn();
                }
            }
            break;
        case SERIAL_DATA_ID:
            uart[UART_CDC].Write(data); // echo out the terminal for now
            break;
        default:
            break;
    }
}