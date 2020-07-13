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

#if !defined(TONNERRE_H)
#define TONNERRE_H

#include <stdbool.h>
#include <cytypes.h>
#include "project.h"
#include "XBee_driver.h"

#define TONNERRE_MAX_ARG_SIZE 24 // Max argument size for Tonnerre messages
#define TONNERRE_MAX_MSG_ID 69
    
#define TONNERRE_MAX_COMMANDS_MANAGEMENT_SYSTEM_ENTRIES 20
#define TONNERRE_MAX_COMMANDS_MANAGEMENT_SYSTEM_RETRIES 3
#define TONNERRE_MAX_COMMANDS_MANAGEMENT_SYSTEM_TIMEOUT 

// Tonnerre message IDs
// -- Mandatory commands --
#define ACK 0
#define POS 1
#define SBPOS 2
#define PING 3
#define PONG 4
#define SBBOX 5
// -- AI-related "protocol" commands --
#define SP 10 // Start
#define EP 11 // End
// -- AI-related match commands --
#define SM 20 // Start
#define EM 21 // End
// -- Various messages types --
#define MSG 30 // Generic message
// -- Qute commands --
#define QT 69 // ;-) See Qute protocol for more details   

// Data structure
typedef struct Tonnerre_callback Tonnerre_callback;
struct Tonnerre_callback {
    uint8_t command;
    void (*callback_function)(uint16_t src_addr, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
};

typedef struct Tonnerre_commands_management_entry Tonnerre_commands_management_entry;
struct Tonnerre_commands_management_entry {
    uint8_t command;
    uint16_t vive_ID;
    uint8_t msg_id;
    
    uint8_t data[TONNERRE_MAX_ARG_SIZE];
    uint8_t data_length;
    
    bool valid;
};

typedef struct Tonnerre Tonnerre;
struct Tonnerre
{
    XBee_driver *xbee_driver;
    Tonnerre_callback callbacks[TONNERRE_MAX_MSG_ID+1];
    Tonnerre_commands_management_entry commands_tabs[TONNERRE_MAX_COMMANDS_MANAGEMENT_SYSTEM_ENTRIES];
    uint8_t msg_id; // Incremented for each message sent
};

// Public methods
Tonnerre* Tonnerre_create();
void Tonnerre_init(Tonnerre *tonnerre, XBee_driver *xbee_driver);
void Tonnerre_check_commands(Tonnerre* tonnerre);
void Tonnerre_send_command(Tonnerre* tonnerre, uint8_t command, uint16_t vive_ID, uint8_t data[TONNERRE_MAX_ARG_SIZE], uint8_t data_length, bool require_response);
void Tonnerre_send_response(Tonnerre* tonnerre, uint8_t command, uint16_t vive_ID, uint8_t msg_id, uint8_t data[TONNERRE_MAX_ARG_SIZE], uint8_t data_length);

// Private methods
void Tonnerre_dispatch(uint8_t *xbee_frame_buffer);
void Tonnerre_register_callback(Tonnerre* tonnerre, uint8_t msg_type, void (*callback_function)(uint16_t src_addr, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]));

// """Protected""" methods - Can only be used by Tonnerre or Tonnerre_commands
int Tonnerre_commands_management_system_get_next_empty_entry(Tonnerre *tonnerre);
void Tonnerre_commands_management_system_remove_entry(Tonnerre *tonnerre, uint8_t command, uint16_t vive_ID, uint8_t msg_id);

#endif

/* [] END OF FILE */
