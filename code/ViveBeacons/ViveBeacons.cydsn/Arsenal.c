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
#include <stdio.h>
#include "configuration.h"
#include "Arsenal.h"
#include "Arsenal_commands.h"

/*
    ---------------------------------------------------------------------------
    A PSoC Library that implements the Arsenal protocol.
    
    See the documentation in docs/reports
    ---------------------------------------------------------------------------
*/

/*
    --------------
    Public methods
    --------------
*/

Arsenal* Arsenal_create() {
    Arsenal* arsenal = (Arsenal *) malloc(1*sizeof(Arsenal));
    
    ISCONNECTED(USB_UART_jumper)
        arsenal->usb_commands_manager = USB_commands_manager_create();
    ISDISCONNECTED(USB_UART_jumper)
        arsenal->uart_commands_manager = UART_commands_manager_create();
    
    return arsenal;
}

void Arsenal_init(Arsenal *arsenal) {
    ISCONNECTED(USB_UART_jumper)
        USB_commands_manager_init(arsenal->usb_commands_manager);
    ISDISCONNECTED(USB_UART_jumper)
        UART_commands_manager_init(arsenal->uart_commands_manager);
    
    // Arsenal callback registration
    Arsenal_register_command(arsenal, "SBPOS", Arsenal_commands_set_beacon_position_callback);
    Arsenal_register_command(arsenal, "STPROTOCOL", Arsenal_commands_start_protocol_callback);
    Arsenal_register_command(arsenal, "ENDPROTOCOL", Arsenal_commands_end_protocol_callback);
    Arsenal_register_command(arsenal, "STMATCH", Arsenal_commands_start_match_callback);
    Arsenal_register_command(arsenal, "ENDMATCH", Arsenal_commands_end_match_callback);
}

void Arsenal_register_command(Arsenal* arsenal, const char *command, void (*callback_function)()) {
    ISCONNECTED(USB_UART_jumper)
        USB_commands_manager_register_command(arsenal->usb_commands_manager, command, callback_function);
    ISDISCONNECTED(USB_UART_jumper)
        UART_commands_manager_register_command(arsenal->uart_commands_manager, command, callback_function);
}

void Arsenal_check_commands(Arsenal* arsenal) {
    ISCONNECTED(USB_UART_jumper)
        USB_commands_manager_check_commands(arsenal->usb_commands_manager);
    ISDISCONNECTED(USB_UART_jumper)
        UART_commands_manager_check_commands(arsenal->uart_commands_manager);
}

char* Arsenal_get_next_token(Arsenal* arsenal) {
    char *ret_val = NULL; // to avoid warning
    
    ISCONNECTED(USB_UART_jumper)
        ret_val = USB_commands_manager_get_next_token(arsenal->usb_commands_manager);
    ISDISCONNECTED(USB_UART_jumper)
        ret_val = UART_commands_manager_get_next_token(arsenal->uart_commands_manager);
    
    return ret_val;
}

void Arsenal_send_command(Arsenal* arsenal, char command[ARSENAL_MAX_COMMAND_LENGTH + 1], uint16_t ID, char args[ARSENAL_MAX_BUFFER_SIZE - (ARSENAL_MAX_COMMAND_LENGTH + 1) + 1]) {
    char buffer[ARSENAL_MAX_BUFFER_SIZE - (ARSENAL_MAX_COMMAND_LENGTH + 1) + 1];
    
    ISCONNECTED(USB_UART_jumper) {
        if(strcmp(args, "") != 0)
            sprintf(buffer, "%d%s%s", ID, arsenal->usb_commands_manager->delimiter, args);
        else
            sprintf(buffer, "%d", ID);
            
        USB_commands_manager_send_command(arsenal->usb_commands_manager, command, buffer);
    }
    ISDISCONNECTED(USB_UART_jumper) {
        if(strcmp(args, "") != 0)
            sprintf(buffer, "%d%s%s", ID, arsenal->uart_commands_manager->delimiter, args);
        else
            sprintf(buffer, "%d", ID);
        
        UART_commands_manager_send_command(arsenal->uart_commands_manager, command, buffer);
    }
}

/* [] END OF FILE */
