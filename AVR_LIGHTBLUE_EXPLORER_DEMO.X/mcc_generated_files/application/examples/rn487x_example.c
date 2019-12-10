/*
 * File:   RN487X_example.c
 * Author: C40924
 *
 * Created on September 6, 2019, 1:20 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "mcc.h"
#include "config/ble_config.h"
#include "rn4870-1-ble-module/rn487x.h"
#include "rn487x_example.h"

static bool RN487X_Init_Example(void);
static void RN487X_SetName_Example(void);
static void RN487X_SetDefaultService_Example(void);
static void RN487X_SetFeaturesBitmap_Example(void);
static void RN487X_SetIOCapability_Example(void);
static void RN487x_SetPinCode_Example(void);
static void RN487X_Reboot_Example(void);
static bool RN487X_WaitForStreamOpenMsg_Example(void);
static void RN487X_GetData_Example(void);
static void RN487X_GetDevName_Example(void);



/*
 *  Public APIs
 */
void RN487X_SetupUpTransparentUart(void)
{
    if (RN487X_Init_Example())
    {
        RN487X_EnterCmdMode();
        // Set RN487x device name
        RN487X_SetName_Example();
        // Set default service bitmap
        RN487X_SetDefaultService_Example();
        // Set features bitmap
        RN487X_SetFeaturesBitmap_Example();
        // Set IO capability
        RN487X_SetIOCapability_Example();
        //Set security pin code
        RN487x_SetPinCode_Example();
        // Reboot RN487x for the above configs to take effect
        RN487X_Reboot_Example();
        RN487X_EnterDataMode();
    }
}

void RN487X_RunTransparentUart(void)
{
    // Read status message and data from RN487X
    RN487X_GetData_Example();

    if (streamOpened)
    {
        // Read user input and send it to remote device
        if (USART0_IsRxReady())
        {
            char sendData = getchar();
            RN487X_SendCmd((uint8_t *) & sendData, sizeof (sendData));
        }
    }
}

/*
 *  Private APIs
 */
static bool RN487X_Init_Example(void)
{
    //Initialize RN487x
    if (RN487X_Init())
    {
        printf("RN487x init succeed\r\n");
    }
    else
    {
        printf("RN487x init failed\r\n");
        return false;
    }

    //Expect CMD> from RN487X after actual response
    RN487X_CmdPromptEnabled();

    return true;
}

static void RN487X_SetName_Example(void)
{
    const char devName[] = RN487X_DEVICE_NAME;

    if (RN487X_SetName(devName, sizeof (devName) - 1)) //Subtract NULL at end
    {
        printf("Set device name succeed\r\n");
    }
    else
    {
        printf("Set device name failed\r\n");
    }
}

static void RN487X_SetDefaultService_Example(void)
{
    if (RN487X_SetServiceBitmap(RN487X_DEFAULT_SERVICE))
    {
        printf("Set default service succeed\r\n");
    }
    else
    {
        printf("Set default service failed\r\n");
    }
}

static void RN487X_SetFeaturesBitmap_Example(void)
{
    if (RN487X_SetFeaturesBitmap(RN487X_DATA_LENGTH_EXTENSION))
    {
        printf("Set features bitmap succeed\r\n");
    }
    else
    {
        printf("Set features bitmap failed\r\n");
    }
}

static void RN487X_SetIOCapability_Example(void)
{
    if (RN487x_SetIOCapability(RN487X_IO_CAPABILITY))
    {
        printf("Set IO capability succeed\r\n");
    }
    else
    {
        printf("Set IO capability failed\r\n");
    }
}

static void RN487x_SetPinCode_Example(void)
{
    const char pinCode[] = RN487X_SEC_PIN_CODE;

    if (RN487x_SetPinCode(pinCode, sizeof (pinCode) - 1))
    {
        printf("Set pin code succeed\r\n");
    }
    else
    {
        printf("Set pin code failed\r\n");
    }
}

static void RN487X_Reboot_Example(void)
{
    if (RN487X_RebootCmd())
    {
        printf("Reboot done\r\n");
    }
    else
    {
        printf("Reboot failed\r\n");
    }
}

static bool RN487X_WaitForStreamOpenMsg_Example(void)
{
    char expectedMsg[] = "%STREAM_OPEN%";
    uint8_t msgLen = sizeof (expectedMsg) - 1; //One less for NULL at end of string
    RN487X_WaitForMsg(expectedMsg, msgLen);
    return true;
}

static void RN487X_GetData_Example(void)
{
    uint8_t rn487xData;

    // Check for data or event from RN487x
    if (RN487X_GetData(&rn487xData))
    {
        printf("%c", rn487xData);
    }
}

static void RN487X_GetDevName_Example(void)
{
    uint8_t index;
    char getCmd[] = "GN\r\n";
    char getCmdResp[22];
    uint8_t getCmdRespLen = RN487X_GetCmd(getCmd, sizeof (getCmd) - 1, getCmdResp);

    printf("Get device name: ");
    for (index = 0; index < getCmdRespLen; index++)
    {
        printf("%c", getCmdResp[index]);
    }
}