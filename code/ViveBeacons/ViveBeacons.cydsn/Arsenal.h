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

#if !defined(ARSENAL_H)
#define ARSENAL_H

#include <cytypes.h>
#include "project.h"
#include "USB_commands_manager.h"
#include "UART_commands_manager.h"

#define ARSENAL_MAX_BUFFER_SIZE 256
#define ARSENAL_MAX_COMMAND_LENGTH 32 // Max argument size for Arsenal messages

// Data structure
typedef struct Arsenal Arsenal;
struct Arsenal
{
    UART_commands_manager* uart_commands_manager;
    USB_commands_manager* usb_commands_manager;
};

// Public methods
Arsenal* Arsenal_create();
void Arsenal_init(Arsenal *Arsenal);
void Arsenal_register_command(Arsenal* arsenal, const char *command, void (*callback_function)());
void Arsenal_check_commands(Arsenal* arsenal);
char* Arsenal_get_next_token(Arsenal* arsenal);
void Arsenal_send_command(Arsenal* arsenal, char command[ARSENAL_MAX_COMMAND_LENGTH + 1], uint16_t ID, char args[ARSENAL_MAX_BUFFER_SIZE - (ARSENAL_MAX_COMMAND_LENGTH + 1) + 1]);

#endif

/* [] END OF FILE */
