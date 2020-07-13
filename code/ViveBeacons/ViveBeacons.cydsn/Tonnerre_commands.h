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

#if !defined(TONNERRE_COMMANDS_H)
#define TONNERRE_COMMANDS_H

#include <cytypes.h>
#include "project.h"
#include "Arsenal.h"
#include "Tonnerre.h"
#include "position.h"

// RX (callbacks)
void Tonnerre_commands_ack_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_pos_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_ping_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_pong_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_sbpos_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_start_protocol_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_end_protocol_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_start_match_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);
void Tonnerre_commands_end_match_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]);

// TX
void Tonnerre_commands_send_ack(uint16_t vive_ID, uint8_t msg_id);
void Tonnerre_commands_send_pos(uint16_t vive_ID, Position2D *pos);
void Tonnerre_commands_send_ping(uint16_t vive_ID, uint8_t ping_data);
void Tonnerre_commands_send_pong(uint16_t vive_ID, uint8_t msg_id, uint8_t ping_data);
void Tonnerre_commands_send_sbpos(uint16_t vive_ID, Position3D *bpos);
void Tonnerre_commands_send_start_protocol(uint16_t vive_ID, uint32_t protocol_ID);
void Tonnerre_commands_send_end_protocol(uint16_t vive_ID, uint32_t protocol_ID);
void Tonnerre_commands_send_start_match(uint16_t vive_ID);
void Tonnerre_commands_send_end_match(uint16_t vive_ID);

#endif

/* [] END OF FILE */
