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

#include <stdlib.h>
#include <stdbool.h>
#include "project.h"
#include "USB_commands_manager.h"

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

USB_commands_manager* USB_commands_manager_create() {
    USB_commands_manager* usb_commands_manager = (USB_commands_manager*) malloc(1*sizeof(USB_commands_manager));
    
    usb_commands_manager->callbacks = NULL;
    usb_commands_manager->last_token = NULL;
    usb_commands_manager->command_count = 0;
    
    //strcpy(usb_commands_manager->delimiter, " ");
    usb_commands_manager->delimiter = " ";
    
    // Clear buffer
    usb_commands_manager->buffer[0] = '\0';
    usb_commands_manager->buffer_pos = 0;
     
    return usb_commands_manager;
}

void USB_commands_manager_init(USB_commands_manager *usb_commands_manager){
    (void)usb_commands_manager; //just to supress warning
    USB_Serial_Start(0, USB_Serial_3V_OPERATION);
    while(!USB_Serial_GetConfiguration());
    USB_Serial_CDC_Init();
}

void USB_commands_manager_register_command(USB_commands_manager *usb_commands_manager, const char *command, void (*callback_function)()) {
    usb_commands_manager->callbacks = (USB_commands_manager_callback *) realloc(usb_commands_manager->callbacks,
                                                                                (usb_commands_manager->command_count + 1)
                                                                                *sizeof(USB_commands_manager_callback));
    strncpy((usb_commands_manager->callbacks[usb_commands_manager->command_count]).command, command, USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH);
    (usb_commands_manager->callbacks[usb_commands_manager->command_count]).callback_function = callback_function;
    usb_commands_manager->command_count++;
}

void USB_commands_manager_check_commands(USB_commands_manager *usb_commands_manager) {
    
    
    char nb_char = 0;
    
    // If we received some packets from the PC
    if(USB_Serial_GetCount() != 0) {
        nb_char = USB_Serial_GetCount();
        
        if((usb_commands_manager->buffer_pos + nb_char) < USB_COMMAND_MANAGER_MAX_BUFFER_SIZE) {
            // Copy the received data into the buffer and update the nb of char read.
            nb_char = USB_Serial_GetData((uint8 *) (usb_commands_manager->buffer + usb_commands_manager->buffer_pos), nb_char); 
            usb_commands_manager->buffer_pos += nb_char + 1;
            usb_commands_manager->buffer[usb_commands_manager->buffer_pos] = '\0';
        }
    }
    
    // Processing the data
    for(int i = usb_commands_manager->buffer_pos - nb_char; i < usb_commands_manager->buffer_pos; i++)
    {
            
        char character = USB_Serial_GetChar();
        
        if(character == 0)
            return;
        
        if(character == '\n') { //end of the command
            usb_commands_manager->buffer[i] = '\0'; //set the of the string
            char *command = strtok_r(usb_commands_manager->buffer,usb_commands_manager->delimiter, &(usb_commands_manager->last_token));
            command++; //to resole a weird bug where the first char is doubled
            
            if(command != NULL){ 
                
                for(int i = 0; i < usb_commands_manager->command_count; i++) { // For each command in registered commands
                    if (strncmp(command, (usb_commands_manager->callbacks[i]).command ,USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH) == 0) {
                        (*((usb_commands_manager->callbacks[i]).callback_function))();
                        break;
                    }
                }
            }
            // Clear buffer
            usb_commands_manager->buffer[0] = '\0';
            usb_commands_manager->buffer_pos = 0;
        }
        
        usb_commands_manager->buffer[i] = character;
    }
}

char* USB_commands_manager_get_next_token(USB_commands_manager *usb_commands_manager) {
    return strtok_r(NULL, usb_commands_manager->delimiter, &(usb_commands_manager->last_token));
}

void USB_commands_manager_send_command(USB_commands_manager *usb_commands_manager, char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1],
                                       char args[USB_COMMAND_MANAGER_MAX_BUFFER_SIZE - (USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1) + 1]) {
    USB_Serial_PutString(command);
    CyDelay(1);
    USB_Serial_PutString(usb_commands_manager->delimiter);
    CyDelay(1);
    USB_Serial_PutString(args);
    CyDelay(1);
    USB_Serial_PutString("\n");
    CyDelay(1);
}

/* [] END OF FILE */
