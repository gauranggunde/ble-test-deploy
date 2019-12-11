/** @file   CLI_Service.h
 *  @brief  Command Line Interpreter application service header file.
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

#ifndef CLI_SERVICE_H
#define CLI_SERVICE_H
#include <stdint.h>

/**
@ingroup cliService
@def TASK_LIST_t
* List containing possible Task which application wishes to execute based on
* CLI message received over USB-CDC Bus
*/
typedef enum
{
    TASK_QUEUED_EMPTY   =   -1,
    TASK_HELLO          =   1,
    TASK_CHIPREV        =   2,
    TASK_CHIPID         =   3,
    TASK_UNIQUE         =   4,
    TASK_LED0_OFF       =   5,
    TASK_LED0_ON        =   6,
    TASK_BLE2_OFF       =   7,
    TASK_BLE2_ON        =   8,
    TASK_DGI_INIT       =   9,
    TASK_GPIO0_LOW      =   10,
    TASK_GPIO0_HIGH     =   11,
    TASK_TEMP_SENSOR    =   12,
    TASK_TEMP_MANDEV    =   13,
    TASK_ECC_SERIAL     =   14,
    TASK_ECC_KEY        =   15,
    TASK_ACCEL_SENSOR   =   16,
    TASK_ACCEL_CHIPID   =   17,
    TASK_BT_MAC         =   18,
    TASK_BT_INFO        =   19,
    TASK_BT_STATUS_LED  =   20,
    TASK_SST25_ID       =   21,
    TASK_SST25_JEDEC    =   22,
    TASK_I2C_PROBE      =   23,
    TASK_VERSION        =   24,
    TASK_HELP           =   25,
    TASK_BLUE           =   26,
    TASK_BUTTON         =   27,
    TASK_PEEK           =   100,
    TASK_BAD_FORMAT     =   127
}TASK_LIST_t;

/**
@ingroup cliService
@def cmd
* Prototype Interface used for cli valid 1st word command list
*/
struct cmd
{
    const char * const command;
    void (* const handler)(char *argument);
};
extern const struct cmd commands[];

//typedef void (*subscriptionCallback_t)(uint8_t *topic, uint8_t *payload);

void CLI_unknownCommand(void);
void CLI_tooLongCommand(void);
uint8_t CLI_cmdListSize(void);
void CLI_taskProcessor(void);
void CLI_setTask(TASK_LIST_t taskRequest);
TASK_LIST_t CLI_getTask(void);

void CLI_sendPeekByte(char peekByte);
char CLI_getPeekByte(void);

#endif /* CLI_SERVICE_H */