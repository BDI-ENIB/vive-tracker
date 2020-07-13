/* ============================================================================
 *
 *  Copyright (C) 2019 - Evan Roué
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ============================================================================
*/

/* [] END OF FILE */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "project.h"
#include "UART_commands_manager.h"

/*
    ---------------------------------------------------------------------------
    This file is a PSoC version of an Arduino library. It was trans-coded from
    https://github.com/kroimon/Arduino-SerialCommand
    ---------------------------------------------------------------------------
*/

/*
    --------------
    Public methods
    --------------
*/

UART_commands_manager* UART_commands_manager_create() {
    UART_commands_manager* uart_commands_manager = (UART_commands_manager*) malloc(1*sizeof(UART_commands_manager));
    
    uart_commands_manager->callbacks = NULL;
    uart_commands_manager->last_token = NULL;
    uart_commands_manager->command_count = 0;
    
    strcpy(uart_commands_manager->delimiter, " ");
    
    // Clear buffer
    uart_commands_manager->buffer[0] = '\0';
    uart_commands_manager->buffer_pos = 0;
    
    return uart_commands_manager;
}

void UART_commands_manager_init(UART_commands_manager *uart_commands_manager) {
    UART_TEENSY_Start();
}
void UART_commands_manager_register_command(UART_commands_manager *uart_commands_manager, const char *command, void (*callback_function)()) {
    uart_commands_manager->callbacks = (UART_commands_manager_callback *) realloc(uart_commands_manager->callbacks, (uart_commands_manager->command_count + 1)*sizeof(UART_commands_manager_callback));
    strncpy((uart_commands_manager->callbacks[uart_commands_manager->command_count]).command, command, UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH);
    (uart_commands_manager->callbacks[uart_commands_manager->command_count]).callback_function = callback_function;
    uart_commands_manager->command_count++;
}

void UART_commands_manager_check_commands(UART_commands_manager *uart_commands_manager) {
    while(UART_TEENSY_GetRxBufferSize() > 0) {
        char character = UART_TEENSY_GetChar();
        if(character == 0)
            break;
        
        if(character == ';') {
            char temp[10]; // The end character is also a delimiter FOR strtok_r function. 
            sprintf(temp, "%s%s", uart_commands_manager->delimiter, ";");
            char *command = strtok_r(uart_commands_manager->buffer, temp, &(uart_commands_manager->last_token));
            
            if(command != NULL) {
                
                for(int i = 0; i < uart_commands_manager->command_count; i++) { // For each command in registered commands
                    if (strncmp(command, (uart_commands_manager->callbacks[i]).command, UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH) == 0) {
                        (*((uart_commands_manager->callbacks[i]).callback_function))();
                        break;
                    }
                }
            }
            
            // Clear buffer
            uart_commands_manager->buffer[0] = '\0';
            uart_commands_manager->buffer_pos = 0;
        } else {
            if(uart_commands_manager->buffer_pos < UART_COMMAND_MANAGER_MAX_BUFFER_SIZE) {
                uart_commands_manager->buffer[uart_commands_manager->buffer_pos] = character;
                uart_commands_manager->buffer_pos++;
                uart_commands_manager->buffer[uart_commands_manager->buffer_pos] = '\0';
            }
        }
    }
}

char* UART_commands_manager_get_next_token(UART_commands_manager *uart_commands_manager) {
    char temp[10]; // The end character is also a delimiter FOR strtok_r function. 
    sprintf(temp, "%s%s", uart_commands_manager->delimiter, ";");
    return strtok_r(NULL, temp, &(uart_commands_manager->last_token));
}

void UART_commands_manager_send_command(UART_commands_manager *uart_commands_manager, char command[UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1], char args[UART_COMMAND_MANAGER_MAX_BUFFER_SIZE - (UART_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1) + 1]) {
    UART_TEENSY_PutString(command);
    UART_TEENSY_PutString(uart_commands_manager->delimiter);
    UART_TEENSY_PutString(args);
    UART_TEENSY_PutString(";");
}