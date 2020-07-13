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

#if !defined(USB_COMMANDS_MANAGER_H)
#define USB_COMMANDS_MANAGER_H

// Defines
#define USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH 32
#define USB_COMMAND_MANAGER_MAX_BUFFER_SIZE 32

// Data structure
typedef struct USB_commands_manager_callback USB_commands_manager_callback;
struct USB_commands_manager_callback {
    char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1];
    void (*callback_function)();
};

// Attributes
typedef struct USB_commands_manager USB_commands_manager;
struct USB_commands_manager {
    USB_commands_manager_callback *callbacks;
    char buffer[USB_COMMAND_MANAGER_MAX_BUFFER_SIZE];
    int command_count;
    int buffer_pos;
    char *last_token;
    char delimiter[2];
};

// Public methods
USB_commands_manager* USB_commands_manager_create();
void USB_commands_manager_init(USB_commands_manager *usb_commands_manager);
void USB_commands_manager_register_command(USB_commands_manager *usb_commands_manager, const char *command, void (*callback_function)());
void USB_commands_manager_check_commands(USB_commands_manager *usb_commands_manager);
char* USB_commands_manager_get_next_token(USB_commands_manager *usb_commands_manager);
void USB_commands_manager_send_command(USB_commands_manager *usb_commands_manager, char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1],   
                                       char args[USB_COMMAND_MANAGER_MAX_BUFFER_SIZE - (USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1) + 1]);

#endif

/* [] END OF FILE */
