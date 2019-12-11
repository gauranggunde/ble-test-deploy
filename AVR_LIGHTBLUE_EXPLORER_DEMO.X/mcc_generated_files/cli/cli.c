/** @file   cli.c
 *  @brief  Command Line Interpreter source file.
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

#include "cli.h"

#include "drivers/uart.h"
#include "application/CLI_service.h"
#include "application/BLE_service.h"

// CLI Bus Access
#define CLI_BUS                 uart[UART_CDC]
#define CLI_UNKNOWN_CMD_MSG     CLI_unknownCommand()
#define CLI_CMD_TOO_LONG        CLI_tooLongCommand()
#define CLI_CMD_PEEK_BYTE(x)    CLI_sendPeekByte(x)
#define CLI_CMD_LIST_SIZE       CLI_cmdListSize()

// CLI Description MACROs
#define MAX_COMMAND_SIZE        80
#define CLI_MATCH_FOUND         0

// CLI Operation variables
static char command[MAX_COMMAND_SIZE];
static uint8_t index = 0;
static bool commandTooLongFlag = false;

// Local CLI descriptor tags
const char * const cli_version_number             = "3.0.0";
const char * const firmware_version_number        = "1.0.0";

// Processing APIs
static bool endOfCommandTest(char passByte);
static void command_received(char *command_text);

// Optional CallBack configured via Init
static void enableBusRxInterrupts(void);
static void enableBusRxInterrupts(void)
{

}

void CLI_init(void)
{
}

void CLI_parser_task(void)
{
    bool cmd_ready;
    volatile char readCharacter;
    // Initialize local variables
    cmd_ready = false;
    readCharacter = '\0';
    // Read a byte from the CLI Bus if in queue
    if(CLI_BUS.DataReady())
    {
       readCharacter = CLI_BUS.Read();
       // Check for newline \n || return character \r
       if(readCharacter == '\r' || readCharacter == '\n')
       {
          command[index] = '\0';    // Remove characters from command buffer

          if(!commandTooLongFlag)
          {
             if( (endOfCommandTest(readCharacter) == true)
                     && (cmd_ready == false) )
             {
                BLE_SendSerialData((char*)command);
                command_received((char*)command);
                cmd_ready = true;
             }
          }
          if(commandTooLongFlag)
          {
             CLI_CMD_TOO_LONG;
          }
          index = 0;
          commandTooLongFlag = false;
       }
       else // otherwise store the character
       {
          if(index < MAX_COMMAND_SIZE)
          {
             command[index++] = readCharacter;
             CLI_CMD_PEEK_BYTE(readCharacter);
          }
          else
          {
             commandTooLongFlag = true;
          }
       }
    }
}

static bool endOfCommandTest(char passByte)
{
   static char lastByte = '\0';
   bool endOfCommandReached = true;

   if(passByte == '\n')
   {
        if(lastByte == '\r')
        {  // '\r\n' will not terminate command parser
           endOfCommandReached = false;
        }
   }
   lastByte = passByte; // Remember last character
   return endOfCommandReached;
}

static void command_received(char *command_text)
{
    char *argument = strstr(command_text, " ");
    uint8_t compare_string_result;
    uint8_t cmd_text_length;
    uint8_t cmd_cmd_length;
    uint8_t cmd_list_index = 0;
    uint8_t cmd_list_size = 0;

    if (argument != NULL)
    {
        /* Replace the delimiter with string terminator */
        *argument = '\0';
        /* Point after the string terminator, to the actual string */
        argument++;
    }

    // Search list for 1st match. Execute it's commands.handler
    cmd_list_size = CLI_CMD_LIST_SIZE;
    for (cmd_list_index = 0; cmd_list_index < cmd_list_size; cmd_list_index++)
    {
        compare_string_result = strcmp(command_text, commands[cmd_list_index].command);
        cmd_text_length = strlen(command_text);        
        cmd_cmd_length = strlen(commands[cmd_list_index].command);

        if ( (CLI_MATCH_FOUND == compare_string_result) 
                && (cmd_text_length == cmd_cmd_length) )
        {
            if (commands[cmd_list_index].handler != NULL)
            {
                commands[cmd_list_index].handler(argument);
                return;
            }
        }
    }
    // Print Bad Command to Console; unless return was hit.
    CLI_UNKNOWN_CMD_MSG;
}
