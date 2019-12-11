/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
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

#include "mcc_generated_files/mcc.h"
#include "application/LIGHTBLUE_service.h"
#include "rn4870-1-ble-module/rn487x_interface.h"
#include "rn4870-1-ble-module/rn487x.h"
#include "drivers/uart.h"

/** MACRO used to reference Periodic Timer overflow flag Set. 
 *  This is used by the application to have a semi-accurate 
 *  periodic task execution rate. 
 *  Strict adherence to time interval is not required.
 */
#define TIMER_FLAG_SET()                (TCA0_IsOverflowInterruptEnabled())
/** MACRO used to reset the Periodic Timer overflow flag.
 *  This is used by the application to reload the semi-accurate
 *  periodic task execution.
 *  The rate allows for a (100%) drift prior to error
 *  Is susceptible to effect by extended BLE communication. 
 */
#define RESET_TIMER_INTERRUPT_FLAG      (TCA0.SINGLE.INTFLAGS = 1)

static char statusBuffer[80];
static char lightBlueSerial[80];
static uint8_t serialIndex;

/*
    Main application
*/
int main(void)
{
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
    RN487X_SetAsyncMessageHandler(statusBuffer, sizeof(statusBuffer));
    
    ENABLE_INTERRUPTS();
    
    RN487X_Init();

    while (1)
    {
        if (RN487X_IsConnected() == true)
        {
            if (TIMER_FLAG_SET() == true)
            {
                RESET_TIMER_INTERRUPT_FLAG;

                LIGHTBLUE_TemperatureSensor();
                LIGHTBLUE_AccelSensor();
                LIGHTBLUE_PushButton();
                LIGHTBLUE_LedState();
                LIGHTBLUE_SendProtocolVersion();
            }
            else 
            {
                while (RN487X_DataReady())
                {
                    LIGHTBLUE_ParseIncomingPacket(RN487X_Read());
                }
                   while (uart[UART_CDC].DataReady())
                {
                    lightBlueSerial[serialIndex] = uart[UART_CDC].Read();
                    if ((lightBlueSerial[serialIndex] == '\r')
                        || (lightBlueSerial[serialIndex] == '\n')
                        || (serialIndex == (sizeof(lightBlueSerial) - 1)))
                    {
                        lightBlueSerial[serialIndex] = '\0';
                        LIGHTBLUE_SendSerialData(lightBlueSerial);
                        serialIndex = 0;
                    }
                    else
                    {
                        serialIndex++;
                    }
                }
            }
        }
        else
        {
            while(RN487X_DataReady())
            {
                uart[UART_CDC].Write(RN487X_Read());
            }
            while (uart[UART_CDC].DataReady())
            {
                RN487X.uartTx(uart[UART_CDC].Read());
            }
        }
    }
}
/**
    End of File
*/