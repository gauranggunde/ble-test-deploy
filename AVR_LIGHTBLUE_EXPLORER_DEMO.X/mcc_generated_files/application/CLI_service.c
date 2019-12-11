/** @file   CLI_Service.c
 *  @brief  Command Line Interpreter application service source file.
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
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Header
#include "CLI_service.h"

// APP Dependencies
#include "MCP9844_temp_sensor.h"
#include "BMA253_accel.h"
#include "SST25_flash.h"
//#include "atecc608_small.h"
#include "BLE_service.h"
//#include "nonMCC_Memory.h"
//#include "../mcc_generated_files/memory.h"

// PHY Dependencies
#include "drivers/uart.h"
#include "include/pin_manager.h"

// Generic CLI prompts
#define COMPARE_FOUND           0
#define CLI_LIST "help"
#define NEWLINE "\r\n"
#define NEWLINE_ESCAPE NEWLINE"\4"
#define BAD_FORMAT_MSG      "Bad Format"NEWLINE
#define NOT_SUPPORTED_MSG   "Processor does not support Requested Task"NEWLINE
#define OK_MSK          "ok"NEWLINE
#define UNKNOWN_CMD_MSG "--------------------------------------------"NEWLINE\
                        "Unknown command. Try:" NEWLINE\
                        CLI_LIST\
                        " "\
                        NEWLINE\
                        "--------------------------------------------"NEWLINE_ESCAPE

#define FULL_UNKNOWN_CMD_MSG "--------------------------------------------" NEWLINE\
                        "List of available commands:" NEWLINE\
                        "hello"NEWLINE\
                        "chiprev"NEWLINE\
                        "chipid"NEWLINE\
                        "unique" NEWLINE\
                        "led0 on | led0 off" NEWLINE\
                        "ble2 on | ble2 off" NEWLINE\
                        "dgi init" NEWLINE\
                        "gpio0 low | gpio0 high" NEWLINE\
                        "temp sensor | temp mandev" NEWLINE\
                        "ecc key | ecc serial" NEWLINE\
                        "accel sensor | accel chipid" NEWLINE\
                        "bt mac | bt info | bt status led" NEWLINE\
                        "sst25 id | sst25 jedec" NEWLINE\
                        "i2c probe" NEWLINE\
                        "version" NEWLINE\
                        "help" NEWLINE\
                        "blue" NEWLINE\
                        "--------------------------------------------"NEWLINE_ESCAPE

// Call Back Functions used by Commands
static void hello_cmd(char *pArg);
static void chiprev_cmd(char *pArg);
static void chipid_cmd(char *pArg);
static void unique_cmd(char *pArg);
static void led0_cmd(char *pArg);
static void ble2_cmd(char *pArg);
static void dgi_cmd(char *pArg);
static void gpio0_cmd(char *pArg);
static void temp_cmd(char *pArg);
static void ecc_cmd(char *pArg);
static void accel_cmd(char *pArg);
static void bt_cmd(char *pArg);
static void sst_cmd(char *pArg);
static void i2c_cmd(char *pArg);
static void version_cmd(char *pArg);
static void help_cmd(char *pArg);
static void blue_cmd(char *pArg);

const struct cmd commands[] =
{
    { "hello",          hello_cmd},
    { "chiprev",        chiprev_cmd},
    { "chipid",         chipid_cmd},
    { "unique",         unique_cmd},
    { "led0",           led0_cmd},    
    { "ble2",           ble2_cmd},    
    { "dgi",            dgi_cmd},
    { "gpio0",          gpio0_cmd},   
    { "temp",           temp_cmd},       
    { "ecc",            ecc_cmd},
    { "accel",          accel_cmd},
    { "bt",             bt_cmd},
    { "sst25",          sst_cmd},
    { "i2c",            i2c_cmd},
    { "version",        version_cmd}, 
    { "help",           help_cmd},
    { "blue",           blue_cmd}   
};

#define LIST_ON         "on"
#define LIST_OFF        "off"
#define LIST_LOW        "low"
#define LIST_HIGH       "high"
#define LIST_INIT       "init"
#define LIST_SENSOR     "sensor"
#define LIST_MANDEV     "mandev"
#define LIST_SERIAL     "serial"
#define LIST_KEY        "key"
#define LIST_CHIPID     "chipid"
#define LIST_MAC        "mac"
#define LIST_INFO       "info"
#define LIST_STATUS     "status led"
#define LIST_ID         "id"
#define LIST_JEDEC      "jedec"
#define LIST_PROBE      "probe"

static TASK_LIST_t cliTask = TASK_QUEUED_EMPTY;
static char storedPeek = '\0';

static void hello_cmd(char *pArg)
{
    (void)pArg;
    cliTask = TASK_HELLO;
}

static void chiprev_cmd(char *pArg)
{
    (void)pArg;
    cliTask = TASK_CHIPREV;
}

static void chipid_cmd(char *pArg)
{
    (void)pArg;
    cliTask = TASK_CHIPID;
}

static void unique_cmd(char *pArg)
{
    (void)pArg;

    cliTask = TASK_UNIQUE;
}

static void led0_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_ON) == COMPARE_FOUND)
    {        
        cliTask = TASK_LED0_ON;
    }
    else if (strcmp(argument, LIST_OFF) == COMPARE_FOUND)
    {
        cliTask = TASK_LED0_OFF;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void ble2_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_ON) == COMPARE_FOUND)
    {        
        cliTask = TASK_BLE2_ON;
    }
    else if (strcmp(argument, LIST_OFF) == COMPARE_FOUND)
    {
        cliTask = TASK_BLE2_OFF;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void dgi_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_INIT) == COMPARE_FOUND)
    {
        cliTask = TASK_DGI_INIT;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void gpio0_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_LOW) == COMPARE_FOUND)
    {
        cliTask = TASK_GPIO0_LOW;
    }
    else if (strcmp(argument, LIST_HIGH) == COMPARE_FOUND)
    {      
        cliTask = TASK_GPIO0_HIGH;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void temp_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_SENSOR) == COMPARE_FOUND)
    {
        cliTask = TASK_TEMP_SENSOR;
    }
    else if (strcmp(argument, LIST_MANDEV) == COMPARE_FOUND)
    {
        cliTask = TASK_TEMP_MANDEV;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void ecc_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_KEY) == COMPARE_FOUND)
    {
        cliTask = TASK_ECC_KEY;
    }
    else if (strcmp(argument, LIST_SERIAL) == COMPARE_FOUND)
    {
        cliTask = TASK_ECC_SERIAL;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void accel_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_SENSOR) == COMPARE_FOUND)
    {
        cliTask = TASK_ACCEL_SENSOR;
    }
    else if (strcmp(argument, LIST_CHIPID) == COMPARE_FOUND)
    {
        cliTask = TASK_ACCEL_CHIPID;        
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void bt_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_MAC) == COMPARE_FOUND)
    {
        cliTask = TASK_BT_MAC;
    }
    else if (strcmp(argument, LIST_INFO) == COMPARE_FOUND)
    {
        cliTask = TASK_BT_INFO;
    }
    else if (strcmp(argument, LIST_STATUS) == COMPARE_FOUND)
    {
        cliTask = TASK_BT_STATUS_LED;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void sst_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_ID) == COMPARE_FOUND)
    {
        cliTask = TASK_SST25_ID;
    }
    else if (strcmp(argument, LIST_JEDEC) == COMPARE_FOUND)
    {
        cliTask = TASK_SST25_JEDEC;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }
}

static void i2c_cmd(char *argument)
{
    // Check valid LIST cases for Command
    if (strcmp(argument, LIST_PROBE) == COMPARE_FOUND)
    {
        cliTask = TASK_I2C_PROBE;
    }
    else
    {
        cliTask = TASK_BAD_FORMAT;
    }  
}

static void version_cmd(char *pArg)
{
    (void)pArg;
    cliTask = TASK_VERSION;
}

static void help_cmd(char *pArg)
{
    (void)pArg;
    cliTask = TASK_HELP;
}

static void blue_cmd(char *pArg)
{
    (void)pArg;
    cliTask = TASK_BLUE;
}

void CLI_unknownCommand(void)
{
    printf(UNKNOWN_CMD_MSG);
}

void CLI_tooLongCommand(void)
{
    printf(NEWLINE"Command too long"NEWLINE);
}

uint8_t CLI_cmdListSize(void)
{
    return sizeof(commands)/sizeof(*commands);
}

#define APP_ECC_BUFFER_SIZE     (65)
uint8_t eccBuffer[APP_ECC_BUFFER_SIZE];

void CLI_taskProcessor(void)
{
    // Temporary Variable Mocks
    bool notOK = false;
    int16_t accelID;
    int16_t temperature;
    uint16_t manufactor;
    uint16_t device;
    uint8_t jedecBuffer[4];
    BMA253_ACCEL_DATA_t accelData;

    // Temporary during Iteration
    // Don't NEWLINE when echoing
    if (cliTask != TASK_PEEK)
    {
        printf(NEWLINE); 
    }
    
    switch (cliTask)
    {
        case TASK_HELLO:
            printf("good day ");
            break;
        case TASK_CHIPREV:
//            printf("0x%4x\r\n", nonMCC_FLASH_ReadCONFIG(MCU_CONFIG_CHIP_REVISION_REG));
            break;
        case TASK_CHIPID:
//            printf("0x%2x\r\n", nonMCC_FLASH_ReadCONFIG(MCU_CONFIG_CHIP_ID_REG));
            break;
        case TASK_UNIQUE:
//            for (uint8_t muiRegion = 0; muiRegion < MCU_MICROCHIP_UNIQUE_IDENTIFIER_SIZE; muiRegion++)
//            {
//                printf("%d: 0x%04x\r\n", muiRegion, nonMCC_FLASH_ReadCONFIG(MCU_CONFIG_DEVICE_INFO_AREA_REG + muiRegion));
//            }
            break;
        case TASK_LED0_OFF:
            LED_GREEN_SetHigh();
            break;
        case TASK_LED0_ON:
            LED_GREEN_SetLow();
            break;
        case TASK_BLE2_OFF:
            // ToDo: Read BLE register, Write Command to BLE
            //    bt_read_reg("|O,01,01\r", &buffer[0]);
            //    print_printf("ok\r\n");
            printf("BLE2 OFF ");
            break;
        case TASK_BLE2_ON:
            // ToDo: Read BLE register, Write Command to BLE
            //    bt_read_reg("|O,01,00\r", &buffer[0]);
            //    print_printf("ok\r\n");
            printf("BLE2 ON ");
            break;
        case TASK_DGI_INIT:
            SW0_SetDigitalOutput();
            SW0_SetLow();
            break;
        case TASK_GPIO0_LOW:
            SW0_SetLow();
            break;
        case TASK_GPIO0_HIGH:
            SW0_SetHigh();
            break;
        case TASK_TEMP_SENSOR:
            MCP9844_GetTemperatureValue(&temperature);
            printf("Temperature = %dC\r\n", temperature);
            break;
        case TASK_TEMP_MANDEV:
            manufactor = MCP9844_GetManufacture();
            device = MCP9844_GetDevice();
            printf("0x%04x%04x\r\n", manufactor, device);
            break;
        case TASK_ECC_SERIAL:
//            atecc508_read_serial(ECC608_I2C_ADDRESS, eccBuffer);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_0]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_1]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_2]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_3]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_4]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_5]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_6]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_7]);
//            printf("%02x", eccBuffer[ECC608_SERIAL_BYTE_8]);
            printf(NEWLINE_ESCAPE);
            break;
        case TASK_ECC_KEY:
//            atecc508_read_slot(ECC608_I2C_ADDRESS, 0, eccBuffer);
//            if (eccBuffer[ECC608_KEY_CHECK_BTYE] == ECC608_KEY_CHECK_VALUE) 
//            {
//                for (uint8_t keyByte = 1; keyByte < sizeof(eccBuffer); keyByte++) 
//                {
//                    printf("%02x", eccBuffer[keyByte]);
//                }
//                printf(NEWLINE); 
//            } 
            break;
        case TASK_ACCEL_SENSOR:
            BMA253_GetAccelDataXYZ(&accelData);
            printf("Acceleration Values: X = %d, Y = %d, Z = %d\r\n", accelData.x, accelData.y, accelData.z);
            break;
        case TASK_ACCEL_CHIPID:
            accelID = BMA253_GetAccelChipId();
            printf("0x%02x\r\n", accelID);
            break;
        case TASK_BT_MAC:
            printf("bt mac mac ");
            break;
        case TASK_BT_INFO:
            printf("bt mac info ");
            break;
        case TASK_BT_STATUS_LED:
            printf("bt mac status led ");
            break;
        case TASK_SST25_ID:
            printf("0x%2x\r\n", SST25_ReadDeviceID());
            break;
        case TASK_SST25_JEDEC:
            SST25_ReadJedecID(jedecBuffer);
            printf("0x%2x%2x%2x%2x\r\n", jedecBuffer[3], jedecBuffer[2], jedecBuffer[1], jedecBuffer[0]);
            break;
        case TASK_I2C_PROBE:
            // Test I2C Bus
            printf("i2 probe ");
            break;
        case TASK_VERSION:
            printf("Build Date: %s\r\n", __DATE__);
            break;
        case TASK_HELP:
            printf(FULL_UNKNOWN_CMD_MSG);
            break;
        case TASK_BLUE:
            BLE_TemperatureSensor();
            BLE_AccelSensor();
            BLE_PushButton();
            BLE_LedState();
            BLE_SendProtocolVersion();
            notOK = true;
            break;   
        case TASK_BUTTON:
            printf("button ");
            if (SW0_GetValue() == 0)
            {
                printf("pressed ");
            }
            else
            {
                printf("released ");
            }
            break;
        case TASK_PEEK:
            printf("%c", CLI_getPeekByte());
            notOK = true;
            break;
        case TASK_BAD_FORMAT:
            printf(BAD_FORMAT_MSG);
            notOK = true;
            break;
        default:
            printf(NOT_SUPPORTED_MSG);
            notOK = true;
            break;
    }
    if (notOK == false)
    {
        printf(OK_MSK);
    }
    cliTask = TASK_QUEUED_EMPTY;
}

void CLI_setTask(TASK_LIST_t taskRequest)
{
    cliTask = taskRequest;
}

TASK_LIST_t CLI_getTask(void)
{
    return cliTask;
}

void CLI_sendPeekByte(char peekByte)
{
    storedPeek = peekByte;
    cliTask = TASK_PEEK;
}

char CLI_getPeekByte(void)
{
    return storedPeek;
}