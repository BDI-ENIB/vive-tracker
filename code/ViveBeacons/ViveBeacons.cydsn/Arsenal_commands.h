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

#if !defined(ARSENAL_COMMANDS_H)
#define ARSENAL_COMMANDS_H

#include <cytypes.h>
#include "project.h"
#include "Arsenal.h"
#include "Tonnerre.h"
#include "position.h"

// RX commands
void Arsenal_commands_set_beacon_position_callback();
void Arsenal_commands_start_protocol_callback();
void Arsenal_commands_end_protocol_callback();
void Arsenal_commands_start_match_callback();
void Arsenal_commands_end_match_callback();

// TX commands
void Arsenal_commands_send_pos(uint16_t ID, Position2D *pos);
void Arsenal_commands_send_start_protocol(uint16_t ID, uint32_t protocol_ID);
void Arsenal_commands_send_end_protocol(uint16_t ID, uint32_t protocol_ID);
void Arsenal_commands_send_start_match(uint16_t ID);
void Arsenal_commands_send_end_match(uint16_t ID);

#endif

/* [] END OF FILE */
