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
#include "main.h"
#include "configuration.h"
#include "position.h"
#include "Arsenal.h"
#include "Arsenal_commands.h"
#include "Tonnerre.h"
#include "Tonnerre_commands.h"

/*
    ---------------------------------------------------------------------------
    Implementation of Tonnerre commands
    
    See the documentation in docs/reports
    ---------------------------------------------------------------------------
*/

/*
    -----------
    RX commands
    -----------
*/

void Tonnerre_commands_ack_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    (void) args;
    Tonnerre_commands_management_system_remove_entry(tonnerre, ACK, vive_ID, msg_id);
}

void Tonnerre_commands_pos_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    (void)(msg_id); //just to supress warning
    
    Position_serialized *pos_s = (Position_serialized *) malloc(1*sizeof(Position_serialized));
    Position2D *pos = (Position2D *) malloc(1*sizeof(Position2D));
    
    memcpy(&(pos_s->i), args, 4);
    POS_UNSERIALIZE_DEFAULT(pos, pos_s);
    
    Arsenal_commands_send_pos(vive_ID, pos);
    
    free(pos_s);
    free(pos);
}

void Tonnerre_commands_ping_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    Tonnerre_commands_send_pong(vive_ID, msg_id, args[0]);
}

void Tonnerre_commands_pong_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    (void) args;
    Tonnerre_commands_management_system_remove_entry(tonnerre, PING, vive_ID, msg_id);
}

void Tonnerre_commands_sbpos_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    Float_serialized f_s;
    uint8_t *p = args;
    
    if(args == NULL)
        return;
    
    memcpy(f_s.c, p, 4); p += 4; beacon_position.x = f_s.f;
    memcpy(f_s.c, p, 4); p += 4; beacon_position.y = f_s.f;
    memcpy(f_s.c, p, 4); p += 4; beacon_position.z = f_s.f;
    
    memcpy(f_s.c, p, 4); p += 4; beacon_position.alpha = f_s.f;
    memcpy(f_s.c, p, 4); p += 4; beacon_position.beta = f_s.f;
    memcpy(f_s.c, p, 4); beacon_position.gamma = f_s.f;
    
    Tonnerre_commands_send_ack(vive_ID, msg_id);
    beacon_position_initialized = true;
}

void Tonnerre_commands_start_protocol_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    Position_serialized ser;
    memcpy(ser.c, args, 4);
    
    Arsenal_commands_send_start_protocol(vive_ID, ser.i);
    
    Tonnerre_commands_send_ack(vive_ID, msg_id);
}

void Tonnerre_commands_end_protocol_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    Position_serialized ser;
    memcpy(ser.c, args, 4);
    
    Arsenal_commands_send_end_protocol(vive_ID, ser.i);
    
    Tonnerre_commands_send_ack(vive_ID, msg_id);
}

void Tonnerre_commands_start_match_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    (void) args;
    Arsenal_commands_send_start_match(vive_ID);
    
    Tonnerre_commands_send_ack(vive_ID, msg_id);
}

void Tonnerre_commands_end_match_callback(uint16_t vive_ID, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE]) {
    (void) args;
    Arsenal_commands_send_end_match(vive_ID);
    
    Tonnerre_commands_send_ack(vive_ID, msg_id);
}

/*
    -----------
    TX commands 
    -----------
*/

void Tonnerre_commands_send_ack(uint16_t vive_ID, uint8_t msg_id) {
    Tonnerre_send_response(tonnerre, ACK, vive_ID, msg_id, NULL, 0);
}

void Tonnerre_commands_send_pos(uint16_t vive_ID, Position2D *pos) {
    Position_serialized pos_s;
    pos_s.i = POS_SERIALIZE_DEFAULT(pos);
    
    Tonnerre_send_command(tonnerre, PING, vive_ID, pos_s.c, 4, false);
}

void Tonnerre_commands_send_ping(uint16_t vive_ID, uint8_t ping_data) {
    Tonnerre_send_command(tonnerre, PING, vive_ID, &ping_data, 1, true);
}

void Tonnerre_commands_send_pong(uint16_t vive_ID, uint8_t msg_id, uint8_t ping_data) {
    Tonnerre_send_response(tonnerre, PONG, vive_ID, msg_id, &ping_data, 1);
}

void Tonnerre_commands_send_sbpos(uint16_t vive_ID, Position3D *bpos) {
    Float_serialized f_s;
    uint8_t buffer[24] = {0};
    uint8_t *p = buffer;
    
    if(bpos == NULL)
        return;
    
    f_s.f = bpos->x; memcpy(p, f_s.c, 4); p += 4;
    f_s.f = bpos->y; memcpy(p, f_s.c, 4); p += 4;
    f_s.f = bpos->z; memcpy(p, f_s.c, 4); p += 4;
    
    f_s.f = bpos->alpha; memcpy(p, f_s.c, 4); p += 4;
    f_s.f = bpos->beta; memcpy(p, f_s.c, 4); p += 4;
    f_s.f = bpos->gamma; memcpy(p, f_s.c, 4);
    
    Tonnerre_send_command(tonnerre, SBPOS, vive_ID, buffer, 24, true);
}

void Tonnerre_commands_send_start_protocol(uint16_t vive_ID, uint32_t protocol_ID) {
    Position_serialized ser;
    ser.i = protocol_ID;
    
    Tonnerre_send_command(tonnerre, SP, vive_ID, ser.c, 4, true);
}

void Tonnerre_commands_send_end_protocol(uint16_t vive_ID, uint32_t protocol_ID) {
    Position_serialized ser;
    ser.i = protocol_ID;
    
    Tonnerre_send_command(tonnerre, EP, vive_ID, ser.c, 4, true);
}

void Tonnerre_commands_send_start_match(uint16_t vive_ID) {
    Tonnerre_send_command(tonnerre, SM, vive_ID, NULL, 0, true);
}

void Tonnerre_commands_send_end_match(uint16_t vive_ID) {
    Tonnerre_send_command(tonnerre, EM, vive_ID, NULL, 0, true);
}

/* [] END OF FILE */
